/*
	Terminal 2.2
	"Procedure.c"
*/

#ifdef THINK_C
#include "MacHeaders"
#endif
#ifdef applec
#pragma load ":(Objects):MacHeadersMPW"
#pragma segment Main2
#endif

#include "Procedure.h"
#include "Text.h"
#include "Main.h"
#include "File.h"
#include "Port.h"
#include "Document.h"
#include "Utilities.h"
#include "XModem.h"
#include "ZModem.h"

/* ----- Send string --------------------------------------------------- */

short Type(register Byte *s)
{
	register short err;
	register Byte *p;
	register long count;
	register short n;
	Byte buffer[256];

	if (Sending)
		return CANCEL;
	Control_X = FALSE;
	/* SerialHandshake(Settings.handshake); */
	while (Busy)
		;
	if (Settings.autoLF) {
		n = *(Byte *)s++;	/* Length of pascal string */
		p = (Byte *)s; 
		while (n--) {
			if (*s++ == '\015') {	/* CR */
				memcpy(buffer, p, count = (Byte *)s - p);
				buffer[count++] = '\012';	/* LF */
				Sending = TRUE;
				Control_X = FALSE;
				SerialSend(buffer, count, &Busy);
				if (Settings.localEcho)
					NewCharacters(buffer, count, FALSE);
				CheckEvents();
				while (Busy)
					;
				if (!Sending) {		/* CAN received */
					err = Control_X ? ABORT : CANCEL;
					goto done;
				}
				p = (Byte *)s;
			}
		}
		count = (Byte *)s - p;
	} else {
		count = *s++;	/* Length of pascal string */
		p = (Byte *)s;
	}
	if (count) {
		Sending = TRUE;
		Control_X = FALSE;
		SerialSend(p, count, &Busy);
		if (Settings.localEcho)
			NewCharacters(p, count, FALSE);
		CheckEvents();
		while(Busy)
			;
		if (!Sending) {		/* CAN received */
			err = Control_X ? ABORT : CANCEL;
			goto done;
		}
	}
	err = FINE;
done:
	/* SerialHandshake(0); */
	Sending = FALSE;
	Control_X = FALSE;
	return err;
}

/* ----- Display character string in terminal window ------------------- */

short Display(register Byte *s)
{
	NewCharacters((Byte *)s + 1, *s, FALSE);
	return FINE;
}

/* ----- Change communication settings --------------------------------- */

short PortSetUp(
	short baud,		/*	0=300, 1=600, 2=1200, 3=2400, 4=4800,
						5=9600, 6=19200, 7=38400, 8=57600 */
	short data,		/* 0=7, 1=8 data bits */
	short parity,	/* 0=noParity, 1=evenParity, 2=oddParity */
	short stop,		/* 0=1, 1=2 stop bit */
	Byte *port,		/* Port name */
	short dtr,		/* 1=don't drop DTR when quitting */
	short hs)		/* 0=none, 1=XON/XOFF, 2=CTS, 3=DTR, 4=CTS/DTR */
{
	register short err;
	short b, d, p, s;
	short setup;

	SerialGetSetup(Settings.portSetup, &b, &d, &p, &s);
	if (baud < 0 || baud > 8)
		baud = b;		/* No change */
	if (data < 0 || data > 1)
		data = d;		/* No change */
	if (parity < 0 || parity > 2)
		parity = p;		/* No change */
	if (stop < 0 || stop > 1)
		stop = s;		/* No change */
	SerialSetSetup(baud, data, parity, stop, &setup);
	if (hs < 0 || hs > 4)
		hs = Settings.handshake;
	if (!port)			/* No change */
		port = Settings.portName;
	if (setup != Settings.portSetup ||
			!EqualString(port, Settings.portName, FALSE, TRUE) ||
			hs != Settings.handshake) {
		Settings.portSetup = setup;
		Settings.handshake = hs;
		Settings.dirty = TRUE;
		SerialClose();
		if (err = SerialOpen(port,Settings.portSetup,Settings.handshake))
			return err;
	}
	if ((dtr >= 0 && dtr <= 1) && dtr != Settings.dropDTR) {
		Settings.dropDTR = dtr;
		Settings.dirty = TRUE;
	}
	return noErr;
}

/* ----- Get volume/directory for up- and downloads -------------------- */

short Folder(
	register short *volume,
	register long *directory)
{
	*volume = Settings.volume;
	*directory = Settings.directory;
	return FINE;
}

/* ----- Set echo modes and autoLF ------------------------------------- */

short TerminalSetup(
	register short lEcho,
	register short rEcho,
	register short autoLF,
	register short save)
{
	if (lEcho >= 0 && lEcho <= 1 && lEcho != Settings.localEcho) {
		Settings.localEcho = lEcho;
		Settings.dirty = TRUE;
	}
	if (rEcho >= 0 && rEcho <= 1 && rEcho != Settings.echo) {
		Settings.echo = rEcho;
		Settings.dirty = TRUE;
	}
	if (autoLF >= 0 && autoLF <= 1 && autoLF != Settings.autoLF) {
		Settings.autoLF = autoLF;
		Settings.dirty = TRUE;
	}
	if (save >= 0 && save <= 1 && save != Settings.save) {
		Settings.save = save;
		Settings.dirty = TRUE;
	}
	return FINE;
}

/* ----- Set binary file transfer options ------------------------------ */

short TransferSetup(
	short binary,
	short cis,
	short zmodem,
	short zautorx)
{
	if (binary >= 0 && binary <= 1 && binary != Settings.Binary) {
		Settings.Binary = binary;
		Settings.dirty = TRUE;
	}
	if (cis >= 0 && cis <= 1 && cis != Settings.protocol) {
		Settings.protocol = cis;
		Settings.dirty = TRUE;
	}
	if (zmodem >= 0 && zmodem <= 1 && zmodem != Settings.ZModem) {
		Settings.ZModem = zmodem;
		Settings.dirty = TRUE;
	}
	if (zautorx >= 0 && zautorx <= 1 && zautorx != Settings.ZAutoReceive) {
		Settings.ZAutoReceive = zautorx;
		Settings.dirty = TRUE;
	}
	return FINE;
}

/* ----- Set XYModem options ------------------------------------------- */

short XYModemSetup(
	Boolean crc,
	short block,
	short batch,
	long timeout)
{
	if (crc >= 0 && crc <= 1 && crc != Settings.XModemCRC) {
		Settings.XModemCRC = crc;
		Settings.dirty = TRUE;
		if (!crc)
			Settings.XModem1K = 0;
	}
	if (block >= 0 && block <= 2 && block != Settings.XModem1K) {
		Settings.XModem1K = block;
		Settings.dirty = TRUE;
	}
	if (batch >= 0 && batch <= 2 && batch != Settings.batch) {
		Settings.batch = batch;
		Settings.dirty = TRUE;
	}
	if (timeout >= 0 && timeout != Settings.XModemtimeout) {
		Settings.XModemtimeout = timeout;
		Settings.dirty = TRUE;
	}
	return FINE;
}

/* ----- Set ZModem options -------------------------------------------- */

short ZModemSetup(
	Boolean escctl,
	long timeout,
	long retries,
	long buffer,
	long packet,
	long window,
	long crcq)
{
	if (escctl >= 0 && escctl <= 1 && escctl != Settings.ZEscapeCtl) {
		Settings.ZEscapeCtl = escctl;
	}
	if (timeout >= 0 && timeout != Settings.ZTimeout) {
		Settings.ZTimeout = timeout;
		Settings.dirty = TRUE;
	}
	if (buffer >= 0 && buffer <= 0x7FFF && buffer != Settings.ZBuffer) {
		Settings.ZBuffer = buffer;
		Settings.dirty = TRUE;
	}
	if (retries >= 3 && retries <= 30 && retries != Settings.ZRetries) {
		Settings.ZRetries = retries;
		Settings.dirty = TRUE;
	}
	if (packet >= 128 && packet <= 1024 && packet != Settings.ZPacket) {
		Settings.ZPacket = packet;
		Settings.dirty = TRUE;
	}
	if (window >= 0 && window <= 0x7FFF && window != Settings.ZWindow) {
		Settings.ZWindow = window;
		Settings.dirty = TRUE;
	}
	if (crcq >= 0 && crcq <= 0x7FFF && crcq != Settings.Zcrcq) {
		Settings.Zcrcq = crcq;
		Settings.dirty = TRUE;
	}
	return FINE;
}

/* ----- Text file send parameters setup ------------------------------- */

short TextsendSetup(
	register Byte *prompt,
	register long linedelay,
	register long chardelay)
{
	if ((long)prompt) {
		Byte s[256];
		memcpy(s, prompt, *prompt + 1);
		if (*s > sizeof(Settings.prompt) - 1)
			*s = sizeof(Settings.prompt) - 1;
		if (!EqualString(s, Settings.prompt, FALSE, FALSE)) {
			memcpy(Settings.prompt, s, *s + 1);
			Settings.dirty = TRUE;
		}
	}
	if (linedelay >= 0 && linedelay != Settings.linedelay) {
		Settings.linedelay = linedelay;
		Settings.dirty = TRUE;
	}
	if (chardelay >= 0 && chardelay != Settings.chardelay) {
		Settings.chardelay = chardelay;
		Settings.dirty = TRUE;
	}
	return FINE;
}

/* ----- Receive binary file ------------------------------------------- */

short Download(
	register Byte *name,			/* File name */
	register Boolean MacBinary,
	short protocol)
{
	register short err;
	register Boolean binary;

	binary = Settings.Binary;			/* Save MacBinary flag */
	Settings.Binary = MacBinary;

	if (protocol == 1) {				/* ZModem */
		err = ZReceive();
		UnloadSeg(ZReceive);
	} else {							/* XYModem */
		SerialBinary(Settings.portSetup);
		DeleteFile(Settings.volume, Settings.directory, name);
		err = XReceive(name, Settings.volume, Settings.directory);
		UnloadSeg(XReceive);
		SerialReset(Settings.portSetup);
	}

	Settings.Binary = binary;			/* Restore MacBinary flag */
	FlushVol(0, Settings.volume);
	return err;
}

/* ----- Transmit binary file ------------------------------------------ */

short Upload(
	register Byte *name,			/* File name */
	register Boolean MacBinary,
	short protocol)
{
	register short err;
	register Boolean binary;

	binary = Settings.Binary;			/* Save MacBinary flag */
	Settings.Binary = MacBinary;

	if (protocol == 1) {				/* ZModem */
		err = ZTransmit(name, Settings.volume, Settings.directory);
		UnloadSeg(ZTransmit);
	} else {							/* XYModem */
		SerialBinary(Settings.portSetup);
		err = XTransmit(name, Settings.volume, Settings.directory);
		UnloadSeg(XTransmit);
		SerialReset(Settings.portSetup);
	}

	Settings.Binary = binary;			/* Restore MacBinary flag */
	return err;
}
