/*
	Terminal 2.2
	"ZOptions.c"
*/

#ifdef THINK_C
#include "MacHeaders"
#endif
#ifdef applec
#pragma load ":(Objects):MacHeadersMPW"
#pragma segment Options
#endif

#include "Options.h"
#include "Strings.h"
#include "Utilities.h"
#include "Text.h"
#include "Main.h"
#include "Procedure.h"

#define B_OK		1		/* OK button */
#define B_CANCEL	2		/* Cancel button */
#define C_ESCCTL	3		/* Escape control characters check box */
#define E_TIMEOUT	4		/* Receive timeout */
#define E_RETRIES	5		/* Max receive retries */
#define E_BUFFER	6		/* Receive buffer size */
#define E_PACKET	7		/* Transmit sub-packet length */
#define E_WINDOW	8		/* Transmit window */
#define E_ZCRCQ		9		/* Transmit ZCRCQ spacing */
#define U_TITLELINE	10		/* Underline */
#define T_RECEIVE	11		/* Receive frame */
#define U_RECEIVE	12
#define T_TRANSMIT	13		/* Transmit frame */
#define U_TRANSMIT	14

/* ----- Verify entry value -------------------------------------------- */

static Boolean Verify(
	register DialogPtr dialog,
	register short number,
	register long *value,
	register long min,
	register long max)
{
	register Byte str[256];

	GetEText(dialog, number, str);
	StringToNum(str, value);
	if (*value < min || *value > max) {
		SysBeep(1);
		SelIText(dialog, number, 0, 32767);
		return TRUE;
	}
	return FALSE;
}

/* ----- Z-Modem options dialog ---------------------------------------- */

void ZOptions(void)
{
	register DialogPtr dialog;
	register Byte str[256];
	short number;
	long timeout, buffer, retries, packet, window, zcrcq;

	CenterDialog('DLOG', DLOG_ZMODEM);
	if (!(dialog = GetNewDialog(DLOG_ZMODEM, 0, (WindowPtr)-1L)))
		return;
	SetUserItem(dialog, U_TITLELINE, (ProcPtr)DrawUserLine);
	SetUserItem(dialog, U_RECEIVE, (ProcPtr)DrawUserFrame);
	SetUserItem(dialog, U_TRANSMIT, (ProcPtr)DrawUserFrame);
	SetCheck(dialog, C_ESCCTL, Settings.ZEscapeCtl);
	NumToString(Settings.ZTimeout / 60, str);
		SetEText(dialog, E_TIMEOUT, str);
	NumToString(Settings.ZBuffer, str);
		SetEText(dialog, E_BUFFER, str);
	NumToString(Settings.ZRetries, str);
		SetEText(dialog, E_RETRIES, str);
	NumToString(Settings.ZPacket, str);
		SetEText(dialog, E_PACKET, str);
	NumToString(Settings.ZWindow, str);
		SetEText(dialog, E_WINDOW, str);
	NumToString(Settings.Zcrcq, str);
		SetEText(dialog, E_ZCRCQ, str);
	ShowWindow(dialog);
	for (;;) {
		ModalDialog(0, &number);
		switch(number) {
			case B_OK:
					if (Verify(dialog, E_TIMEOUT, &timeout, 0, 35791394) ||
						Verify(dialog, E_BUFFER, &buffer, 0, 0x7FFF) ||
						Verify(dialog, E_RETRIES, &retries, 3, 30) ||
						Verify(dialog, E_PACKET, &packet, 128, 1024) ||
						Verify(dialog, E_WINDOW, &window, 0, 0x7FFF) ||
						Verify(dialog, E_ZCRCQ, &zcrcq, 0, 0x7FFF))
						break;
					ZModemSetup(
						GetCheck(dialog, C_ESCCTL),
						timeout * 60,
						retries,
						buffer,
						packet,
						window,
						zcrcq);
			case B_CANCEL:
				DisposDialog(dialog);
				return;
			case C_ESCCTL:
				ToggleCheckBox(dialog, number);
				break;
		}
	}
}
