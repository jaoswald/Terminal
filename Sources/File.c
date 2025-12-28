/*
	Terminal 2.2
	"File.c"
*/

#ifdef THINK_C
#include "MacHeaders"
#endif
#ifdef applec
#pragma load ":(Objects):MacHeadersMPW"
#pragma segment Main2
#endif

#include "File.h"
#include "Text.h"
#include "Main.h"
#include "Strings.h"
#include "Utilities.h"
#include "MySF.h"
#include "Document.h"
#include "Port.h"
#include "CancelDialog.h"
#include "XModem.h"
#include "ZModem.h"
#include "MacBinary.h"
#include "FormatStr.h"
#include "Interp.h"

/* ----- Static data --------------------------------------------------- */

static jmp_buf env;
static short ScriptRef;			/* Script file reference */
static short ScriptUnget;		/* Script file unget buffer */
static Byte *ScriptBuffer;		/* Script file buffer */
static long ScriptEob;			/* Size of script file buffer */
static long ScriptMark;			/* Next position in script file buffer */
extern INTRINSIC Intrinsics[];	/* Script intrinsic functions */

/* ----- Text capture -------------------------------------------------- */

void TextCapture(short options)
{
	register DocumentPeek window = TerminalWindow;
	SFReply sfr;
	short r;

	/* If capture is on the off */

	if (window->file) {
		if (window->length) {
			if (FSWrite(window->file, &window->length, window->record))
				SysBeep(1);
		}
		FSClose(window->file);
		FlushVol(0, window->volume);
		window->file = window->volume = 0;
		window->length = 0;
		SetItemStyle(GetMenu(FILE), CAPTURE, 0);
		return;
	}

	/* If capture is off then on */

	if (options & (optionKey | cmdKey | shiftKey | controlKey)) {
		/* Append to existing file */
		MySFGetFile(MyString(STR_G, G_APPEND), 0, 1, &TEXT, &sfr, 0);
		if (!sfr.good)	/* Cancel */
			return;
		if (OpenFile(sfr.vRefNum, 0, (Byte *)sfr.fName, &r)) {
			SysBeep(1);
			return;
		}
		if (SetFPos(r, fsFromLEOF, 0)) {
			FSClose(r);
			SysBeep(1);
			return;
		}
	} else {
		/* Open new file */
		MySFPutFile(MyString(STR_G, G_SAVE), MyString(STR_G, G_CTFILE),
			MyString(STR_G, G_CAPTURE), &sfr);
		if (!sfr.good)	/* Cancel */
			return;
		DeleteFile(sfr.vRefNum, 0, (Byte *)sfr.fName);
		if (CreateFile(sfr.vRefNum, 0, (Byte *)sfr.fName,
				Settings.textCreator, TEXT) ||
				OpenFile(sfr.vRefNum, 0, (Byte *)sfr.fName, &r)) {
			SysBeep(1);
			return;
		}
	}
	window->file = r;
	window->volume = sfr.vRefNum;
	SetItemStyle(GetMenu(FILE), CAPTURE, ACTIVE);
}

/* ----- Save one byte in capture file --------------------------------- */

void ByteCapture(register Byte b)
{
	register DocumentPeek window = TerminalWindow;
	register short err;

	if (window->file) {
		window->record[(window->length)++] = b;
		if (window->length == RECORD) {
			err = FSWrite(window->file, &window->length, window->record);
			window->length = 0;
			if (err) {
				TextCapture(0);	/* Close file */
				Error(err, EmptyStr);
			}
		}
	}
}

/* ----- Save text of window ------------------------------------------- */

void SaveBuffer(short options)
{
	register DocumentPeek window = TerminalWindow;
	register Byte *p;
	register short err;
	SFReply sfr;
	short r;
	long count;

	if (window->buf.newChar == window->buf.firstChar)	/* Empty */
		return;
	if (options & (optionKey | cmdKey | shiftKey | controlKey)) {
		/* Append to existing file */
		MySFGetFile(MyString(STR_G, G_APPEND), 0, 1, &TEXT, &sfr, 0);
		RedrawDocument();
		if (!sfr.good)	/* Cancel */
			return;
		if (OpenFile(sfr.vRefNum, 0, (Byte *)sfr.fName, &r))
			goto done2;
		if (SetFPos(r, fsFromLEOF, 0))
			goto done1;
	} else {
		/* Create new file */
		MySFPutFile(MyString(STR_G, G_SAVE), MyString(STR_G, G_SCBUFFER),
			MyString(STR_G, G_BUFFER), &sfr);
		RedrawDocument();
		if (!sfr.good)	/* Cancel */
			return;
		DeleteFile(sfr.vRefNum, 0, (Byte *)sfr.fName);
		if ((err = CreateFile(sfr.vRefNum, 0, (Byte *)sfr.fName,
				Settings.textCreator, TEXT)) ||
				(err = OpenFile(sfr.vRefNum, 0, (Byte *)sfr.fName, &r)))
			goto done2;
	}
	p = window->buf.text;
	if (window->buf.newChar > window->buf.firstChar) {	/* One part */
		count = window->buf.newChar - window->buf.firstChar;
		err = FSWrite(r, &count, p + window->buf.firstChar);
	} else {											/* Two parts */
		count = window->buf.size - window->buf.firstChar;
		if (err = FSWrite(r, &count, p + window->buf.firstChar))
			goto done1;
		if (count = window->buf.newChar)
			err = FSWrite(r, &count, p);
	}
done1:
	FSClose(r);
	FlushVol(0, sfr.vRefNum);
done2:
	if (err)
		Error(err, EmptyStr);
}

/* ----- Send text file ------------------------------------------------ */

short pascal SendTextFile(
	short ref,					/* File reference number */
	Byte *name)					/* File name */
{
	register short err;			/* Error code */
	register long count;		/* File read count */
	register Byte buffer[256];	/* Transmit buffer */
	HParamBlockRec p;			/* Used for PBRead() */
	long mark = 0;				/* Current file position */
	long time;					/* How long it takes */

	if (SendFileRef || Sending || Transfer)
		return fBsyErr;
	SendFileRef = ref;
	Sending = TRUE;
	Control_X = FALSE;
	SetItemStyle(GetMenu(FILE), SEND, ACTIVE);
	MakeMessage(TerminalWindow,
		FormatStr(buffer, MyString(STR_P, P_STEXT), name));
	/* SerialHandshake(Settings.handshake); */
	p.ioParam.ioCompletion = 0;
	p.ioParam.ioRefNum = ref;
	p.ioParam.ioBuffer = (Ptr)buffer;
	p.ioParam.ioReqCount = (Settings.chardelay) ? 1 : sizeof(buffer) - 1;
	p.ioParam.ioPosMode =
		(Settings.linedelay || Settings.prompt[0] || Settings.autoLF) ?
		(0x0D80 | fsAtMark) : fsAtMark;	/* CR as newline (if necessary) */
	time = Time;
	do {
		err = PBRead((ParmBlkPtr)&p, FALSE);
		count = p.ioParam.ioActCount;
		if ((err == noErr || err == eofErr) && count) {
			while (Busy)	/* Other send in progress */
				;
			if (buffer[count-1] == '\015') {
				if (Settings.autoLF)
					buffer[count++] = '\012';
				SerialSend(buffer, count, &Busy);
				if (Settings.localEcho)
					NewCharacters(buffer, count, FALSE);
				if (Settings.linedelay || Settings.prompt[0])
					Loop(Settings.linedelay,Settings.prompt,1);
				else
					CheckEvents();
			} else {
				SerialSend(buffer, count, &Busy);
				if (Settings.localEcho)
					NewCharacters(buffer, count, FALSE);
				if (Settings.chardelay)
					Loop(Settings.chardelay, 0, 0);
				else
					CheckEvents();
			}
			mark += count;
		}
	} while (err == noErr && Sending);

	if (err == eofErr)
		err = noErr;
	if (!Sending)
		err = Control_X ? ABORT : CANCEL;
	Statistics(mark, time = Time - time, err);
	SetItemStyle(GetMenu(FILE), SEND, 0);
	FSClose(ref);
	SendFileRef = 0;
	Sending = FALSE;
	Control_X = FALSE;
	/* SerialHandshake(0); */
	return err;
}

/* ----- Get text file name and send text file ------------------------- */

void SendText(void)
{
	register short err;
	SFReply sfr;
	short r;

	if (Transfer) {
		SysBeep(1);
		return;
	}
	if (!Sending) {
		MySFGetFile(MyString(STR_G, G_SEND), 0, 1, &TEXT, &sfr, 0);
		RedrawDocument();
		if (!sfr.good)
			return;
		if (err = OpenFile(sfr.vRefNum, 0, (Byte *)sfr.fName, &r)) {
			Error(err, EmptyStr);
			return;
		}
		err = SendTextFile(r, (Byte *)sfr.fName);
	} else {
		SerialAbort();
		Sending = FALSE;
	}
}

/* ----- Send text ----------------------------------------------------- */

static short TypeText(
	register Byte *s,			/* -> text to send */
	register long n)			/* Number of characters in text */
{
	register long count;		/* File read count */
	register Byte buffer[256];	/* Transmit buffer */
	short req;					/* Max byte count */
	Boolean cr;					/* Look for CR */
	short err;					/* Error code */
	long mark = 0;				/* Current file position */
	long time;					/* How long it takes */

	Sending = TRUE;
	Control_X = FALSE;
	SetItemStyle(GetMenu(EDIT), PASTE, ACTIVE);
	MakeMessage(TerminalWindow,
		FormatStr(buffer, MyString(STR_P, P_SSCRAP)));
	/* SerialHandshake(Settings.handshake); */
	if (Settings.chardelay) {
		req = 1;
		cr = FALSE;
	} else {
		req = sizeof(buffer) - 1;
		cr = Settings.linedelay || Settings.prompt[0] || Settings.autoLF;
	}
	time = Time;
	do {
		count = (req < n) ? req : n;
		if (cr) {
			Byte *s1;
			if (s1 = memchr(s, 13, count))
				count = s1 - s + 1;
		}
		n -= count;
		memcpy(buffer, s, count);
		s += count;
		if (count) {
			while (Busy)	/* Other send in progress */
				;
			if (buffer[count-1] == '\015') {
				if (Settings.autoLF)
					buffer[count++] = '\012';
				SerialSend(buffer, count, &Busy);
				if (Settings.localEcho)
					NewCharacters(buffer, count, FALSE);
				if (Settings.linedelay || Settings.prompt[0])
					Loop(Settings.linedelay, Settings.prompt, 1);
				else
					CheckEvents();
			} else {
				SerialSend(buffer, count, &Busy);
				if (Settings.localEcho)
					NewCharacters(buffer, count, FALSE);
				if (Settings.chardelay)
					Loop(Settings.chardelay, 0, 0);
				else
					CheckEvents();
			}
			mark += count;
		}
	} while (n && Sending);
	if (!n)
		err = noErr;
	if (!Sending)
		err = Control_X ? ABORT : CANCEL;
	/* Statistics(mark, time = Time - time, err); */
	SetItemStyle(GetMenu(EDIT), PASTE, 0);
	Sending = FALSE;
	Control_X = FALSE;
	/* SerialHandshake(0); */
	return err;
}

/* ----- Send TEXT from scrap ------------------------------------------ */

void SendScrap(void)
{
	register Handle h;
	register long err;
	long offset;

	if (Transfer) {
		SysBeep(1);
		return;
	}
	if (Sending) {
		SerialAbort();
		Sending = FALSE;
	} else {
		if (h = NewHandle(0)) {
			if ((err = GetScrap(h,TEXT,&offset)) > 0) {
				HLock(h);
				TypeText((Byte *)*h, err);
				err = noErr;
			}
			DisposHandle(h);
		} else
			err = memFullErr;
		if (err)
			Error(err, EmptyStr);
	}
}

/* ----- Receive file -------------------------------------------------- */

void FileReceive(void)
{
	register short err;
	SFReply sfr;

	if (Sending) {
		SysBeep(1);
		return;
	}
	if (!Transfer) {
		if (Settings.ZModem) {
			err = ZReceive();
			UnloadSeg(ZReceive);
		} else
			if (Settings.batch) {
				err = XReceive(EmptyStr, 0, 0);
				UnloadSeg(XReceive);
			} else {
				MySFPutFile(MyString(STR_G, G_RECEIVE),
					MyString(STR_G, G_SFRECEIVE),
					MyString(STR_G, G_DOWNLOAD), &sfr);
				RedrawDocument();
				if (sfr.good) {
					DeleteFile(sfr.vRefNum, 0, (Byte *)sfr.fName);
					err = XReceive((Byte *)sfr.fName, sfr.vRefNum, 0);
					UnloadSeg(XReceive);
				}
			}
	} else
		Transfer = 0;	/* Cancel transfer */
}

/* ----- Transmit file ------------------------------------------------- */

void FileTransmit(void)
{
	register short err;
	SFReply sfr;

	if (Sending) {
		SysBeep(1);
		return;
	}
	if (!Transfer) {
		MySFGetFile (MyString(STR_G, G_TRANSMIT), 0, -1, 0, &sfr, 0);
		RedrawDocument();
		if (sfr.good) {
			if (Settings.ZModem) {
				err = ZTransmit((Byte *)sfr.fName, sfr.vRefNum, 0);
				UnloadSeg(ZTransmit);
			} else {
				err = XTransmit((Byte *)sfr.fName, sfr.vRefNum, 0);
				UnloadSeg(XTransmit);
			}
		}
	} else
		Transfer = 0;	/* Cancel transfer */
}

/* ----- Make MacBinary file ------------------------------------------- */

short Convert(
	short volume1, long dir1, Byte *name1,
	short volume2, long dir2, Byte *name2)
{
	register short err, err2;
	register Handle buf;
	register unsigned long size;
	Boolean mf;
	short ref2;
	long count;

	if ((err = BinOpenRead(volume1, dir1, name1)) ||
			(err = CreateFile(volume2, dir2, name2, Application.signature,
				Application.btype)) ||
			(err = OpenFile(volume2, dir2, name2, &ref2)))
		return err;
	if ((size = MaxBuffer(&mf)) < 128 || !(buf = NewBuffer(size, mf)))
		return memFullErr;
	err2 = 0;
	do {
		count = size;
		if (!(err = BinRead(&count, (Byte *)*buf)) || err == eofErr)
			err2 = FSWrite(ref2, &count, *buf);
	} while(!err && !err2);
	DisposeBuffer(buf, mf);
	BinCloseRead();
	FSClose(ref2);
	FlushVol(0, volume2);
	if (err == eofErr)
		err = 0;
	return err2 ? err2 : err;
}

void Make(void)
{
	SFReply r1, r2;
	register Byte *suffix;
	register Byte name[256];
	register long n;
	register short err;

	suffix = MyString(STR_G, G_BIN);
	MySFGetFile(MyString(STR_G, G_MAKE), 0, -1, 0, &r1, 0);
	if (!r1.good)
		return;
	RedrawDocument();
	memcpy(name, r1.fName, n = (r1.fName[0] + 1));
	memcpy(name + n, suffix + 1, suffix[0]);
	name[0] += suffix[0];
	MySFPutFile(MyString(STR_G, G_SAVE), MyString(STR_G, G_MAKENAME),
		name, &r2);
	if (!r2.good)
		return;
	RedrawDocument();
	DeleteFile(r2.vRefNum, 0, (Byte *)r2.fName);
	if (err = Convert(r1.vRefNum, 0, (Byte *)r1.fName,
			r2.vRefNum, 0, (Byte *)r2.fName))
		Error(err, EmptyStr);
}

/* ----- Extract from MacBinary file ----------------------------------- */

short CopyFile(register short ref1)
{
	register short err, err2;
	long count;
	register Handle buf;
	register unsigned long size;
	Boolean mf;

	if ((size = MaxBuffer(&mf)) < 128 || !(buf = NewBuffer(size, mf)))
		return memFullErr;
	err2 = 0;
	do {
		count = size;
		if (!(err = FSRead(ref1, &count, *buf)) || err == eofErr)
			err2 = BinWrite(&count, (Byte *)*buf);
	} while(!err && !err2);
	DisposeBuffer(buf, mf);
	if (err == eofErr)
		err = 0;
	return err ? err : err2;
}

void Extract(void)
{
	SFReply r1, r2;
	short ref1;
	Byte name[64];
	Byte header[BinHeaderLength];
	long count, data, resource;
	register short err, err2;

	MySFGetFile(MyString(STR_G, G_EXTRACT), 0, -1, 0, &r1, 0);
	if (!r1.good)
		return;
	RedrawDocument();

	if (err = OpenFile(r1.vRefNum, 0, (Byte *)r1.fName, &ref1))
		goto sorry1;

	count = sizeof(header);
	if ((err = FSRead(ref1, &count, header)) || count != sizeof(header))
		goto sorry;

	if (!BinCheckHeader(header, name, &data, &resource)) {
		err = 1;
		goto sorry;
	}

	MySFPutFile(MyString(STR_G, G_SAVE), MyString(STR_G, G_SFEXTRACT),
		name, &r2);
	if (!r2.good)
		goto sorry;
	RedrawDocument();

	DeleteFile(r2.vRefNum, 0, (Byte *)r2.fName);
	if (err = CreateFile(r2.vRefNum, 0, (Byte *)r2.fName, '????', '????'))
		goto sorry;

	if (err = BinOpenWrite(r2.vRefNum, 0, (Byte *)r2.fName, header))
		goto sorry;

	err = CopyFile(ref1);
	err2 = BinCloseWrite();
	if (!err)
		err = err2;

sorry:
	FSClose(ref1);
sorry1:
	if (err)
		Error(err, EmptyStr);
}

/* ----- Get text file name and execute script ------------------------- */

#define SCRIPTSIZE	512				/* Script file buffer size */
#define SCRIPTMAX	Config.script	/* Memory size for script */

static Boolean Script(void)
{
	register short err;
	register Boolean quit = FALSE;
	Options save;

	ScriptBuffer = 0;
	if (!(ScriptBuffer = (Byte *)NewPtr(SCRIPTSIZE + SCRIPTMAX))) {
		err = MemError();
		goto done;
	}
	ScriptUnget = -1;			/* No unget character yet */
	ScriptMark = ScriptEob = SCRIPTSIZE;	/* Buffer is still empty */

	save = Settings;			/* Save settings */
	if (setjmp(env) == 0) {
		SI_Load(Intrinsics, ScriptBuffer + SCRIPTSIZE, SCRIPTMAX);
		FSClose(ScriptRef);		/* Script file no longer needed */
		ScriptRef = 0;
		err = SI_Interpret();	/* Call main() in script */
	} else {					/* Come here after error */
		err = 1;				/* Always restore settings if error */
		SysBeep(1);
	}
	/*
		The script main() can return the following values:
			0	: Don't restore saved settings, continue application
			1	: Restore saved settings, continue application
			256	: Don't restore saved settings, quit application
			257	: Restore saved settings, quit application
	*/
	if (err == 1 || err == 257) {	/* Restore settings */
		if (save.portSetup != Settings.portSetup ||
				!EqualString(save.portName, Settings.portName, FALSE, TRUE) ||
				save.handshake != Settings.handshake) {
			SerialClose();
			SerialOpen(save.portName, save.portSetup, save.handshake);
		}
		Settings = save;
	}
	if (err == 256 || err == 257)	/* Quit application */
		quit = TRUE;
	err = noErr;

done:
	if (ScriptBuffer)
		DisposPtr((Ptr)ScriptBuffer);
	if (ScriptRef)
		FSClose(ScriptRef);
	if (err) {
		SysBeep(1);
		Error(err, EmptyStr);
	}
	return quit;
}

static Boolean ScriptFlag = 0;

Boolean RunScript(
	short volume,
	long directory,
	Byte *name,
	short item)
{
	register short err;
	register Byte *n;
	register Boolean quit = FALSE;
	SFReply sfr;

	if (Transfer) {
		SysBeep(1);
		return quit;
	}
	if (ScriptFlag) {		/* Cancel script */
		Abort = TRUE;
		Sending = FALSE;
		SerialAbort();
	} else {				/* Execute script file */
		if (name)
			err = OpenFile(volume, directory, n = name, &ScriptRef);
		else {
			MySFGetFile(MyString(STR_G,G_SCRIPT),
				MyString(STR_G,G_SUFFIX),
				1, &TEXT, &sfr, 0);
			RedrawDocument();
			if (!sfr.good)
				return quit;
			err = OpenFile(sfr.vRefNum, 0, n = (Byte *)sfr.fName,
				&ScriptRef);
		}
		if (err) {
			Error(err, EmptyStr);
			return quit;
		}
		Abort = FALSE;
		ScriptFlag = item;
		SetWTitle((WindowPtr)TerminalWindow, n);
		SetItemStyle(GetMenu(SCRIPT), item, ACTIVE);
		quit = Script();
		SetItemStyle(GetMenu(SCRIPT), item, 0);
		Abort = TRUE;
		ScriptFlag = 0;
	}
	return quit;
}

Boolean DoMenuScript(register short item)
{
	register Byte name[256];

	GetItem(GetMenu(SCRIPT), item, name);
	Append(name, MyString(STR_G, G_SUFFIX));
	return RunScript(Settings.scriptVolume, Settings.scriptDirectory, name, item);
}

/* ----- Functions called by interpreter ------------------------------- */

void SI_Error(
	register short erno,
	register Byte *s,
	register short line)
{
	register Byte message[256];

	erno += 2;
	if (*s) {
		CtoPstr((char *)s);
		FormatStr(message,MyString(STR_S,1),s,MyString(STR_S,erno),line);
		PtoCstr((Byte *)s);
	} else
		FormatStr(message,MyString(STR_S,2),MyString(STR_S,erno),line);
	MakeMessage(TerminalWindow, message);
	longjmp(env, 0);	/* Return to Script(), ignore link chain */
}

short SI_GetSource(void)
{
	register Byte c;

	if (ScriptUnget >= 0) {			/* The was a previous unget */
		c = ScriptUnget;			/* So use the unget buffer */
		ScriptUnget = -1;			/* Unget buffer is empty now */
		return c;
	}
	if (ScriptMark < ScriptEob)		/* Can still use buffer */
		return ScriptBuffer[ScriptMark++];
	if (ScriptEob < SCRIPTSIZE)		/* Was last (partial) buffer */
		return -1;					/* EOF */
	ScriptEob = SCRIPTSIZE;			/* Read next buffer from file */
	FSRead(ScriptRef, &ScriptEob, ScriptBuffer);
	if (ScriptEob > 0) {			/* Ok, at least one character */
		ScriptMark = 0;
		return SI_GetSource();
	}
	return -1;						/* Error or end of file */
}

void SI_UngetSource(register short c)
{
	ScriptUnget = c;				/* That's easy */
}

/* ----- Kiss script file ---------------------------------------------- */

void Kiss(short options)
{
	SFReply r;
	ParamBlockRec p;
	register OSType creator;

	creator = (options & (optionKey | cmdKey | shiftKey | controlKey)) ?
		Settings.textCreator : Application.signature;
	do {
		MySFGetFile(MyString(STR_G,G_KISS), MyString(STR_G,G_SUFFIX),
			1, &TEXT, &r, creator);
		if (r.good) {
			memset(&p, 0, sizeof(p));
			p.fileParam.ioNamePtr = r.fName;
			p.fileParam.ioVRefNum = r.vRefNum;
			if (!PBGetFInfo(&p, FALSE)) {
				p.fileParam.ioFlFndrInfo.fdCreator = creator;
				PBSetFInfo(&p, FALSE);
			}
		}
	} while (FALSE /* r.good */);
}
