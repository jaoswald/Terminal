/*
	Terminal 2.2
	"CisB.c"
*/

#ifdef THINK_C
#include "MacHeaders"
#endif
#ifdef applec
#pragma load ":(Objects):MacHeadersMPW"
#pragma segment CisB
#endif

#include "Text.h"
#include "Main.h"
#include "CisB.h"
#include "Strings.h"
#include "Utilities.h"
#include "CancelDialog.h"
#include "Port.h"
#include "MacBinary.h"
#include "CRC.h"

#define ETX			0x03
#define DLE 		0x10
#define XON			0x11
#define XOFF		0x13
#define NAK			0x15
#define ESC			0x1B

#define MAXBUFSIZE	1032	/* Largest data block we can handle */
#define MAXSA		2		/* Maximum number of waiting packets */
#define DEFWS		1		/* I can send 2 packets ahead */
#define DEFWR		1		/* I can receive send ahead packets */
#define DEFBS		8		/* I can handle 1024 bytes */
#define DEFCM		1		/* I can handle CRC */
#define DEFDQ		1		/* I can handle non-quoted NUL */
#define DEFBUFSIZE	511		/* Default data block */
#define MAXERRORS	10		/* Maximal number of errors allowed */
#define MAXABORT	3		/* Max errors if aborting */
#define MAXTIME		10		/* Timeout (seconds) */
#define ENQDELAY	2		/* Before answering ENQ (seconds) */

#define CLOSER		if (Vers) BinCloseRead(); else FSClose(ref)
#define CLOSEW		if (Vers) BinCloseWrite();\
					else { FSClose(ref); FlushVol(0, volume); }
#define GETEOF		if (Vers) BinGetEOF(&Eof); else GetEOF(ref,&Eof)
#define OPENR(v,d,n)	(Vers ? BinOpenRead(v,d,n) : OpenFile(v,d,n,&ref))
#define READ(c,b)	(Vers ? BinRead(&c,b) : FSRead(ref,&c,b))
#define TIME		((Eof && Mark) ? ((Time-Ctime)*(Eof-Mark))/Mark : 0)
#define WRITE(c,b)	(Vers ? BinWrite(&c,b) : FSWrite(ref,&c,b))
#define MIN(a,b)	(((a) < (b)) ? (a) : (b))

typedef struct {
	short seq;				/* Sequence number */
	short num;				/* Number of data bytes */
	Byte buf[MAXBUFSIZE];	/* Actual data */
} PACKET;

typedef struct {
	Byte ws;				/* Window send */
	Byte wr;				/* Window receive */
	Byte bs;				/* Block size */
	Byte cm;				/* Check method */
} PARM;

PACKET SABuf[MAXSA + 1];		/* Send-ahead buffers */
static Byte RBuffer[MAXBUFSIZE];/* Receive buffer */
static PARM His;				/* Other End's Parameters */
static PARM Our;				/* Negotiated Parameters */
static short SeqNum;			/* Block sequence number */
static short BufferSize;		/* Our buffer size */
static short Rsize;				/* Size of receiver buffer */
static short SAMax;				/* Send ahead blocks */
static short AckSA;				/* Which SABuf is waiting for ACK */
static short FillSA;			/* Which SABuf is ready for new data */
static short SAWaiting;			/* Number of SABuf's waiting for ACK */
static Boolean QuickB;			/* TRUE if QuickB in effect */
static Boolean UseCRC;			/* TRUE if CRC in effect */
static Boolean SAEnabled;		/* TRUE if send ahead in effect */
static Boolean Aborting;		/* TRUE if aborting transfer */

static Boolean Binary;		/* Binary (or ASCII) */
static long Mark;			/* Current file mark */
static long Eof;			/* Length of file (if known) */
static long Ctime;			/* Current time in (seconds) */
static short Vers;			/* MacBinary version */
static long Blk;			/* Block counter */

void SendFailure(Byte);

/* ----- Send a byte to the host --------------------------------------- */

static void SendByte(Byte ch)
{
	SerialSend(&ch, 1, &Busy);
	while (Busy)
		;
}

/* ----- Send a masked byte to the host -------------------------------- */

static Byte QuoteTable[] = {
	0, 0, 0, 1, 0, 1, 0, 0,	/* nul soh stx ETX eot ENQ ack bel */
	0, 0, 0, 0, 0, 0, 0, 0,	/* bs  ht  lf  vt  ff  cr  so  si  */
	1, 1, 0, 1, 0, 1, 0, 0,	/* DLE XON dc2 XOF dc4 NAK syn etb */
	0, 0, 0, 0, 0, 0, 0, 0	/* can em  sub esc fs  gs  rs  us  */
};

static void SendMaskedByte(register Byte ch)
{
	if (ch < 32 && QuoteTable[ch]) {
		SendByte(DLE);
		SendByte(ch + '@');
	} else
		SendByte(ch);
}

/* ----- Send an ACK to the host --------------------------------------- */

static void SendACK(register short seqnum)
{
	SendByte(DLE);
	SendByte(seqnum + '0');
}

/* ----- Read byte from the host --------------------------------------- */

static short ReadByte(register Byte *ch)
{
	register long start;
	register long count;

	/* If a character is available don't check for cancel,
	this speeds up reception */

	if (!SerialRead(ch, 1)) {
    	start = Time;
		do {
			if ((Time - start) > MAXTIME)
				return TIMEOUT;
			if (!(count = SerialRead(ch, 1)) && CheckCancel())
				return CANCEL;
        } while (count == 0);
	}
    return FINE;
}

/* ----- Read masked byte from the host -------------------------------- */

static short ReadMaskedByte(
	register Byte *ch,
	register Boolean *masked)
{
	register short err;

	*masked = FALSE;
	if (err = ReadByte(ch))
    	return err;
	if (*ch == DLE) {
		if (err = ReadByte(ch))
			return err;
		*masked = TRUE;
		*ch &= 0x1F;
	}
	return err;
}

/* ----- Increment Sequence Number ------------------------------------- */

static short IncrSeq(register short value)
{
	return (value == 9) ? 0 : value + 1;
}

/* ----- Increment Send Ahead ------------------------------------------ */

static short IncrSA(register short value)
{
	return (value == MAXSA) ? 0 : value + 1;
}

/* ----- Calculate checksum or CRC ------------------------------------- */

static unsigned short DoChecksum(
	register Byte ch,
	register unsigned short cksm)
{
	if (QuickB && UseCRC)
		return UpdateCRC(ch, cksm);
	cksm <<= 1;
	if (cksm > 255)
		cksm = (cksm & 0xFF) + 1;
	cksm += ch;
	if (cksm > 255)
		cksm = (cksm & 0xFF) + 1;
	return cksm;
}


/* ----- Send data to the host ----------------------------------------- */

static void SendData(short buffernumber)
{
	register short n;
	register PACKET	*p;
	register Byte b, *q;
	register unsigned short cksm;

	p = &SABuf[buffernumber];
	cksm = (QuickB && UseCRC) ? 0xFFFF : 0;
	SendByte(DLE);
	SendByte('B');
	SendByte(b = p->seq + '0');
	cksm = DoChecksum(b, cksm);
	n = p->num;
	q = p->buf;
	while (n--) {
		SendMaskedByte(b = *q++);
		cksm = DoChecksum(b, cksm);
	}
	SendByte(ETX);
	cksm = DoChecksum(ETX, cksm);
	if (QuickB && UseCRC)
		SendMaskedByte((cksm >> 8) & 0xFF);
	SendMaskedByte(cksm & 0xFF);
}

/* ----- Restablish syncronism with the remote ------------------------- */

enum {
	GETFIRSTDLE,
	GETFIRSTDIGIT,
	GETSECONDDLE,
	GETSECONDDIGIT
};

static short ReSync(register Byte *digit)
{
	register short state, err;
	Byte c;

	SendByte(ENQ);
	SendByte(ENQ);
	state = GETFIRSTDLE;
	for (;;) {
		if (err = ReadByte(&c))
			return err;
		switch (state) {
			case GETFIRSTDLE:
				if (c == DLE)
					state = GETFIRSTDIGIT;
				break;
			case GETFIRSTDIGIT:
				if (c >= '0' && c <= '9') {
					*digit = c;
					state = GETSECONDDLE;
				}
				break;
			case GETSECONDDLE:
				if (c == DLE)
					state = GETSECONDDIGIT;
				break;
			case GETSECONDDIGIT :
				if (c >='0' && c <='9') {
					if (*digit == c)
						return FINE;
					*digit = c;
				}
				state = GETSECONDDLE;
				break;
		}
	}
}

/* ----- Read a packet ------------------------------------------------- */

enum {			/* Receive states */
	RGETDLE,
	RGETB,
	RGETSEQ,
	RGETDATA,
	RGETCHECKSUM,
	RSENDACK,
	RTIMEDOUT,
	RSUCCESS
};

static short ReadPacket(
	Boolean leadinseen,
	Boolean fromsendpacket)
{
	register short state, err, n;
	register unsigned short cks;
	short nextseq, blocknum, errors, newcks;
	Byte c;
	Boolean masked;
	register Byte *message;	/* Message for progress dialog */

	for (n = 0; n < BufferSize; ++n)
		RBuffer[n] = 0;
	nextseq = IncrSeq(SeqNum);
	errors = 0;
	state = leadinseen ? RGETSEQ : RGETDLE;
	for (;;) {
		switch (state) {
			case RGETDLE:	/* Wait for DLE */
				message = EmptyStr;
				err = ReadByte(&c);
				if (CheckCancel())
					err = CANCEL;
				switch (err) {
					case CANCEL:
						SendFailure('A');
						return err;
					case TIMEOUT:
						message = MyString(STR_P, P_TIMEOUT);
						state = RTIMEDOUT;
						break;
					case FINE:
						switch (c) {
							case DLE:
								state = RGETB;
								break;
							case ENQ:
								state = RSENDACK;
								break;
						}
				}
				break;
			case RGETB:		/* Wait for 'B' */
				message = EmptyStr;
				err = ReadByte(&c);
				switch (err) {
					case TIMEOUT:
					case CANCEL:
						message = MyString(STR_P, P_TIMEOUT);
						state = RTIMEDOUT;
						break;
					case FINE:
						switch (c) {
							case 'B':
								state = RGETSEQ;
								break;
							case ENQ:
								state = RSENDACK;
								break;
							default:
								state = RGETDLE;
								break;
						}
				}
				break;
			case RGETSEQ:	/* Wait for block sequence number */
				message = EmptyStr;
				err = ReadByte(&c);
				switch (err) {
					case TIMEOUT:
					case CANCEL:
						message = MyString(STR_P, P_TIMEOUT);
						state = RTIMEDOUT;
						break;
					case FINE:
						switch (c) {
							case ENQ:
								state = RSENDACK;
								break;
							default:
								cks = (QuickB && UseCRC) ? 0xFFFF : 0;
								blocknum = c - '0';
								cks = DoChecksum(c, cks);
								n = 0;
								state = RGETDATA;
								break;
						}
						break;
				}
				break;
			case RGETDATA:	/* Wait for next data byte */
				message = EmptyStr;
				err = ReadMaskedByte(&c, &masked);
				switch (err) {
					case TIMEOUT:
					case CANCEL:
						message = MyString(STR_P, P_TIMEOUT);
						state = RTIMEDOUT;
						break;
					case FINE:
						cks = DoChecksum(c, cks);
						if (c == ETX && !masked)
							state = RGETCHECKSUM;
						else
							RBuffer[n++] = c;
						break;
				}
				break;
			case RGETCHECKSUM:	/* Wait for checksum, or CRC */
				message = EmptyStr;
				err = ReadMaskedByte(&c, &masked);
				switch (err) {
					case TIMEOUT:
					case CANCEL:
						message = MyString(STR_P, P_TIMEOUT);
						state = RTIMEDOUT;
						break;
					case FINE:
						if (QuickB && UseCRC) {
							cks = UpdateCRC(c, cks);
							err = ReadMaskedByte(&c, &masked);
							switch (err) {
								case TIMEOUT:
								case CANCEL:
									newcks = cks ^ 0xFF;
									break;
								case FINE:
									cks = UpdateCRC(c, cks);
									newcks = 0;
									break;
							}
						} else
							newcks = c;
						if (newcks != cks) {
							message = MyString(STR_P, P_CRCERR);
							state = RTIMEDOUT;
						} else if (RBuffer[0] == 'F')
							state = RSUCCESS;
						else if (blocknum == SeqNum)
							state = RSENDACK;
						else if (blocknum != nextseq) {
							message = MyString(STR_P, P_BLOCKERR2);
							state = RTIMEDOUT;
						} else
							state = RSUCCESS;
						break;
				}
				break;
			case RTIMEDOUT:		/* Send NAK */
				++errors;
				UpdateProgress(Mark,Eof,TIME,Blk,errors,message);
				if (errors > MAXERRORS || fromsendpacket)
					return TIMEOUT;
				SendByte(NAK);
				state = RGETDLE;
				break;
			case RSENDACK:		/* Send ACK */
				SendACK(SeqNum);
				state = RGETDLE;
				break;
			case RSUCCESS:		/* Success */
				UpdateProgress(Mark,Eof,TIME,Blk,errors,
					MyString(STR_P,P_VERIFY));
				SeqNum = blocknum;
				Rsize = n;
				return FINE;
		}
	}
}

/* ----- Wait until the SABuf indicated by AckSA has been ACKed -------- */

enum {			/* Send states */
	SGETDLE,
	SGETNUM,
	SHAVEACK,
	SGETPACKET,
	STIMEDOUT,
	SSENDNAK,
	SSENDENQ,
	SSENDDATA
};

static short GetACK(void)
{
	register short state, err, errors;
	short blocknum;
	Boolean SentENQ;
	short SAIndex;
	Byte c;

	errors = 0;
	SentENQ = FALSE;
	state = SGETDLE;
	for (;;) {
		switch (state) {
			case SGETDLE:	/* Wait for DLE */
				err = ReadByte(&c);
				if (CheckCancel())
					err = CANCEL;
				switch (err) {
					case CANCEL:
						if (!Aborting) {
							SendFailure('A');
							return err;
						}
						/* Fall thru to TIMEOUT */
					case TIMEOUT:
						state = STIMEDOUT;
						break;
					case FINE:
						switch (c) {
							case DLE:
								state = SGETNUM;
								break;
							case NAK:
								++errors;
								UpdateProgress(Mark,Eof,TIME,Blk,errors,
									MyString(STR_P,P_REPEAT));
								if (errors > MAXERRORS)
									return TIMEOUT;
								state = SSENDENQ;
								break;
							case ETX:
								state = SSENDNAK;
								break;
						}
						break;
				}
				break;
			case SGETNUM:	/* Get block sequence number */
				err = ReadByte(&c);
				switch (err) {
					case TIMEOUT:
					case CANCEL:
						state = STIMEDOUT;
						break;
					case FINE:
						switch (c) {
							case 'B':
								state = SGETPACKET;
								break;
							case NAK:
								++errors;
								UpdateProgress(Mark,Eof,TIME,Blk,errors,
									MyString(STR_P,P_REPEAT));
								if (errors > MAXERRORS)
									return TIMEOUT;
								state = SSENDENQ;
								break;
							default:
								state = (c >= '0' && c <= '9') ?
									SHAVEACK : STIMEDOUT;
						}
				}
				break;
			case SGETPACKET:	/* Get packet */
				if (!(err = ReadPacket(TRUE, TRUE))) {
					if (RBuffer[0] == 'F') {
						SendACK(SeqNum);
						return CANCEL;
					}
					return FINE;
				}
				state = STIMEDOUT;
				break;
			case SHAVEACK:
				blocknum = c - '0';
				if (SABuf[AckSA].seq == blocknum) {
					UpdateProgress(Mark,Eof,TIME,Blk,errors,
						MyString(STR_P,P_VERIFY));
					AckSA = IncrSA(AckSA);
					--SAWaiting;
					return FINE;
				}
				if (SABuf[IncrSA(AckSA)].seq == blocknum && SAWaiting == 2) {
					UpdateProgress(Mark,Eof,TIME,Blk,errors,
						MyString(STR_P,P_VERIFY));
					AckSA = IncrSA(AckSA);
					AckSA = IncrSA(AckSA);
					SAWaiting -= 2;
					return FINE;
				}
				UpdateProgress(Mark,Eof,TIME,Blk,errors,
					MyString(STR_P,P_BLOCKERR2));
				if (SABuf[AckSA].seq == IncrSeq(blocknum))
					state = SentENQ ? SSENDDATA : SGETDLE;
				else
					state = Aborting ? SGETDLE : STIMEDOUT;
				SentENQ = FALSE;
				break;
			case STIMEDOUT:
				++errors;
				UpdateProgress(Mark,Eof,TIME,Blk,errors,
					MyString(STR_P,P_TIMEOUT));
				if (errors > MAXERRORS || (Aborting && errors > MAXABORT))
					return TIMEOUT;
				state = SSENDENQ;
				break;
			case SSENDNAK:
				++errors;
				if (errors > MAXERRORS)
					return TIMEOUT;
				SendByte(NAK);
				state = SGETDLE;
				break;
			case SSENDENQ:
				++errors;
				if (errors > MAXERRORS)
					return TIMEOUT;
				state = (ReSync(&c)) ? SSENDENQ : SHAVEACK;
				SentENQ = TRUE;
				break;
			case SSENDDATA:
				SAIndex = AckSA;
				for (err = 1; err <= SAWaiting; ++err) {
					SendData(SAIndex);
					SAIndex = IncrSA(SAIndex);
				}
				state = SGETDLE;
				SentENQ = FALSE;
				break;
		}
	}
}

/* ----- Send a packet to the host ------------------------------------- */

static short SendPacket(register short size)
{
	register PACKET *p;
	register short err;

	if (SAWaiting == SAMax && (err = GetACK()))
		return err;
	SeqNum = IncrSeq(SeqNum);
	p = &SABuf[FillSA];
	p->seq = SeqNum;
	p->num = size;
	SendData(FillSA);
	FillSA = IncrSA(FillSA);
	++SAWaiting;
	return FINE;
}

/* ----- Get host's ACKs on outstanding packets ------------------------ */

static short SAFlush(void)
{
	register short err;

	while (SAWaiting > 0)
		if (err = GetACK())
			return err;
	return FINE;
}

/* ----- Send a failure packet to the host ----------------------------- */

static void SendFailure(register Byte code)
{
	register Byte *p;

	AckSA = 0;
	FillSA = 0;
	SAWaiting = 0;
	Aborting = TRUE;		/* Inform GetACK we're aborting */
	p = SABuf[0].buf;
	p[0] = 'F';
	p[1] = code;
	if (!SendPacket(2))
		SAFlush();			/* Gotta wait for the host to ACK it */
}

/* ----- Make info string for progress dialog -------------------------- */

static void MakeInfo(
	register short id,				/* P_UPLOAD or P_DOWNLOAD */
	register short vers)			/* MacBinary version */
{
	register Byte *p;
	Byte s[256];

	p = MyString(STR_P, id);
	memcpy(s, p, p[0] + 1);
	if (QuickB) {
		Append(s, MyString(STR_P, P_QUICKB));
		if (UseCRC)
			Append(s, MyString(STR_P, P_CRC));
		if (Our.ws)
			Append(s, MyString(STR_P, P_SENDAHEAD));
	}
	switch(vers) {
		case 0:
			Append(s, MyString(STR_P, P_TEXT));
			break;
		case 1:
			Append(s, MyString(STR_P, P_BINARY1));
			break;
		case 2:
			Append(s, MyString(STR_P, P_BINARY2));
			break;
	}
	InfoProgress(s);
}

/* ----- Download the specified file from the host --------------------- */

static short ReceiveFile(
	Byte *name,
	short volume,
	long directory)
{
	register short err;
	register short n;
	register short headerLength = 0;/* Number of bytes in header */
	Byte header[BinHeaderLength];	/* The first 128 bytes received */
	short ref = 0;
	long count;						/* Number of bytes to write to file */
	Byte name2[64];					/* Name from MacBinary header */
	long df, rf;					/* Data fork, resource fork length */
	Byte *usedName;					/* File name used */

	UpdateProgress(0,0,0,0,0,EmptyStr);
	SendACK(SeqNum);
	for (;;) {
		if (err = ReadPacket(FALSE, FALSE))
			break;			/* Receive error */
		switch (RBuffer[0]) {
			case 'N':			/* Data packet */
				count = Rsize - 1;
				Mark += count;
				Blk++;
				if (headerLength < sizeof(header)) {
					n = MIN(count, sizeof(header) - headerLength);
					memcpy(header + headerLength, RBuffer + 1, n);
					headerLength += n;
					if (headerLength == sizeof(header)) {
						/* The header is complete. Try to use the file	*/
						/* name from the MacBinary header. If this file	*/
						/* exists already use the first name.			*/
						if (Settings.Binary)		/* MacBinary check */
							Vers = BinCheckHeader(header, name2, &df, &rf);
						if (Vers) {
							err = CreateFile(volume, directory,
								usedName = name2, 
								Settings.binCreator, Settings.binType);
							if (err == dupFNErr)
								err = CreateFile(volume, directory,
									usedName = name,  
									Settings.binCreator, Settings.binType);
						} else
							err = CreateFile(volume, directory,
									usedName = name, 
									Settings.binCreator, Settings.binType);
						if (err) {
							SendFailure('E');
							goto done;			/* Create error */
						}
						if (Vers) {				/* Valid MacBinary */
							NameProgress(usedName);
							if (err = BinOpenWrite(volume, directory,
									usedName, header)) {
								SendFailure('E');
								goto done;		/* Open error */
							}
							ref = 1;			/* File is open */
							BinGetEOF(&Eof);
							/* Skip MacBinary header, write the rest */
							if (count -= n)
								err = BinWrite(&count, RBuffer + 1 + n);
						} else {				/* Not MacBinary */
							if (err = OpenFile(volume, directory,
									usedName, &ref)) {
								SendFailure('E');
								goto done;		/* Open error */
							}
							/* The first 128 bytes are no MB header,	*/
							/* so they must be written as data block	*/
							err = FSWrite(ref, &count, RBuffer + 1);
						}
						MakeInfo(P_DOWNLOAD, Vers);
					}
				} else					/* Not the first 128 bytes */
					err = WRITE(count, RBuffer + 1);
				if (err) {
					SendFailure('E');
					goto done;			/* Disk write error */
				}
				SendACK(SeqNum);
				break;
			case 'T':			/* Transfer packet */
				if (RBuffer[1] == 'C') {		/* Close file */
					SendACK(SeqNum);
					err = 0;
				} else {
					/* Unexpected "T" packet. Something is rotten	*/
					/* on the other end. Send a failure packet to	*/
					/* kill the transfer cleanly.					*/
					SendFailure('E');
					err = 3;
				}
				goto done;
			case 'F':			/* Failure packet */
				SendACK(SeqNum);
				err = 4;
				goto done;
		}
	}
done:
	if (ref) {					/* Close file if necessary */
		CLOSEW;
	}
	return err;
}

/* ----- Send the specified file to the host --------------------------- */

static short SendFile(
	Byte *name,
	short volume,
	long directory)
{
	register Byte *p;
	register short err;
	short ref;
	long n;
	OSType creator, type;
	long create, modif;

	/* Only send non-TEXT files as MacBinary if requested */

	Vers = (Binary && Settings.Binary &&
		!InfoFile(volume, directory, name,
			&creator, &type, &create, &modif) &&
		type != TEXT) ? 2 : 0;
	if (err = OPENR(volume, directory, name)) {
		SendFailure('E');
		return err;				/* Open error */
	}
	MakeInfo(P_UPLOAD, Vers);
	GETEOF;
	UpdateProgress(0,Eof,0,0,0,EmptyStr);

	do {
		p = SABuf[FillSA].buf;
		p[0] = 'N';
		n = BufferSize - 1;
		err = READ(n, &p[1]);
		Mark += n;
		Blk++;
		if (err = eofErr)
			err = 0;
		if (err) {				/* Disk read error */
			SendFailure('E');
			CLOSER;
			return err;
		}
		if (n > 0 && (err = SendPacket(n + 1))) {
			CLOSER;
			return err;
		}
	} while (n > 0);
	CLOSER;

	/* Inform host that the file was sent */

	p = SABuf[FillSA].buf;
	p[0] = 'T';
	p[1] = 'C';
	if (err = SendPacket(2))
		return err;
	return SAFlush();
}

/* ----- Handle T packet ----------------------------------------------- */

static short TransferFile(void)
{
    register short err;
    register Byte name[64];

	if ((RBuffer[1] != 'D' && RBuffer[1] != 'U') ||
			(RBuffer[2] != 'A' && RBuffer[2] != 'B')) {
		NameProgress((Byte *)"\p<wrong>");
		SendFailure('N');	/* Wrong direction or file type */
		return 1;
	}

	/* Extract file type and name */

	Binary = (RBuffer[2] == 'B');
	name[0] = (Rsize - 3 > 63) ? 63 : Rsize - 3;
	memcpy(name + 1, RBuffer + 3, name[0]);
	NameProgress(name);

	/* Do the transfer */

	err = (RBuffer[1] == 'U') ?
		SendFile(name, Settings.volume, Settings.directory) :
		ReceiveFile(name, Settings.volume, Settings.directory);
	return err;
}

/* ----- Called when a + packet is received ---------------------------- */

/*
	Compuserve sends the following packet:
	DLE 'B' '1' '+'
	DLE 41			01 = window send
	DLE 41			01 = window receive
	DLE 48			08 = buffer size
	DLE 41			01 = check method

	DLE 43			03 = dq
	DLE 40			00 = no transport layer

	DLE 54			14 = Quote set	--- --- --- ETX --- ENQ --- ---
	DLE 40			00				--- --- --- --- --- --- --- ---
	D4				D4				DLE XON --- XOF --- NAK --- ---
	DLE 40			00				--- --- --- --- --- --- --- ---
	DLE 40			00				--- --- --- --- --- --- --- ---
	DLE 40			00				--- --- --- --- --- --- --- ---
	DLE 40			00				--- --- --- --- --- --- --- ---
	DLE 40			00				--- --- --- --- --- --- --- ---

	DLE 42			02 = download resume (2 = auto)
	DLE 40			00 = upload recovery
	DLE 41			01 = file information
	ETX 3D
*/

static short TransportParameters(void)
{
	register Byte *p;
	register short err;

	His.ws = RBuffer[1];
	His.wr = RBuffer[2];
	His.bs = RBuffer[3];
	His.cm = RBuffer[4];
	p = SABuf[FillSA].buf;
	p[0] = '+';
	p[1] = DEFWS;
	p[2] = DEFWR;
	p[3] = DEFBS;
	p[4] = DEFCM;
	p[5] = DEFDQ;
	if (err = SendPacket(6))
		return err;
	if (err = SAFlush())
		return err;
	Our.wr = MIN(His.ws, DEFWR);
	Our.ws = MIN(His.wr, DEFWS);
	Our.bs = MIN(His.bs, DEFBS);
	Our.cm = MIN(His.cm, DEFCM);
	if (!Our.bs)
		Our.bs = 4;
	BufferSize = Our.bs * 128;
	QuickB = TRUE;
	if (Our.cm == 1)
		UseCRC = TRUE;
	if (Our.ws) {
		SAEnabled = TRUE;
		SAMax = MAXSA;
	}
	return FINE;
}

/* ----- Called when a ? packet is received ---------------------------- */

/* Note: never received such a packet */

static short ApplicationParameters(void)
{
	register Byte *p;
	register short err;

	p = SABuf[FillSA].buf;
	p[0] = '?';
	p[1] = 1;
	if (err = SendPacket(2))
		return err;
	return SAFlush();
}

/* ----- Sleep a while ------------------------------------------------- */

static void Sleep(register unsigned long sec)
{
	register unsigned long start = Time;

	while((Time - start) < sec)
		CheckCancel();
}

/* ----- Intercept characters received for QuickB protocol ------------- */

void CisB(void)
{
	register short err;
	static Byte enquiry[] = {	/* Answer if ENQ received */
		DLE, '+', DLE, '0'
	};

	/* Come here if ENQ was received */

	while (Busy)
		;
	Sleep(ENQDELAY);
	SerialSend(enquiry, sizeof(enquiry), &Busy);
	while (Busy)
		;

	/* Set up defaults */

	SeqNum = 0;
	BufferSize = DEFBUFSIZE;
	QuickB = UseCRC = SAEnabled = FALSE;
	SAMax = 1;

	/* Go into B-protocol mode */

	SerialBinary(Settings.portSetup);
	if (Settings.handshake == 1)	/* XON/OFF must be off */
		SerialHandshake(0);
	Transfer = Transfer_B;
	DrawProgressDialog(P_CISB, EmptyStr);
	AckSA = FillSA = SAWaiting = 0;
	Aborting = FALSE;
	Mark = Blk = Eof = 0;
	Vers = 0;
	Ctime = Time;

	do {
		if (err = ReadPacket(FALSE, FALSE))
			break;
		switch (RBuffer[0]) {
			case 'T':			/* File transfer */
				err = TransferFile();
				Ctime = Time - Ctime;
				Statistics(Mark, Ctime, err);
				err = 1;
				break;
			case '+':			/* Transport parameters packet */
				NameProgress((Byte *)"\p<+>");
				err = TransportParameters();
				break;
			case '?':			/* Application parameters packet */
				NameProgress((Byte *)"\p<?>");
				err = ApplicationParameters();
				break;
			default:			/* Unkown packet */
				NameProgress((Byte *)"\p<unkown>");
				SendFailure('E');
				err = 1;
		}
	} while (!err);

	RemoveCancelDialog();
	Transfer = 0;
	SerialReset(Settings.portSetup);
	SerialHandshake(Settings.handshake);
}
