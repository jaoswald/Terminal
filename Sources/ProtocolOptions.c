/*
	Terminal 2.2
	"ProtocolOptions.c"
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
#include "MySF.h"
#include "Procedure.h"

#define B_OK		1		/* OK button */
#define B_CANCEL	2		/* Cancel button */
#define C_BIN		3		/* MacBinary check box */
#define C_CISB		4		/* CIS-B check box */
#define	R_XYMODEM	5		/* X/Y-Modem radio button */
#define R_ZMODEM	6		/* Z-Modem radio button */
#define C_ZAUTORX	7		/* Z-Modem auto receive checkbox */
#define B_FOLDER	8		/* Button to select new path */
#define T_FOLDER	9		/* Path name for up- and downloads */
#define U_TITLELINE	10		/* Underline */
#define T_PROTOCOL	11		/* Protocol frame */
#define U_PROTOCOL	12

/* ----- Protocol options dialog --------------------------------------- */

void ProtocolOptions(void)
{
	register DialogPtr dialog;
	short number;
	Byte str[256];
	short volume = Settings.volume;
	long directory = Settings.directory;

	CenterDialog('DLOG', DLOG_TRANSFER);
	if (!(dialog = GetNewDialog(DLOG_TRANSFER, 0, (WindowPtr)-1L)))
		return;
	SetUserItem(dialog, U_TITLELINE, (ProcPtr)DrawUserLine);
	SetUserItem(dialog, U_PROTOCOL, (ProcPtr)DrawUserFrame);
	SetRadioButton(dialog, R_XYMODEM, R_ZMODEM, R_XYMODEM+Settings.ZModem);
	SetCheck(dialog, C_BIN, Settings.Binary);
	SetCheck(dialog, C_CISB, Settings.protocol);
	SetCheck(dialog, C_ZAUTORX, Settings.ZAutoReceive);
	Pathname(str, volume, directory);
	SetEText(dialog, T_FOLDER, str);
	ShowWindow(dialog);
	for (;;) {
		ModalDialog(0, &number);
		switch(number) {
			case B_OK:
				if (volume != Settings.volume ||
						directory != Settings.directory) {
					Settings.volume = volume;
					Settings.directory = directory;
					VolumeId(Settings.volumeName, &Settings.volume);
					Settings.dirty = TRUE;
				}
				TransferSetup(
					GetCheck(dialog, C_BIN),
					GetCheck(dialog, C_CISB),
					GetRadioButton(dialog,R_XYMODEM,R_ZMODEM) - R_XYMODEM,
					GetCheck(dialog, C_ZAUTORX));
			case B_CANCEL:
				DisposDialog(dialog);
				return;
			case C_BIN:
			case C_CISB:
			case C_ZAUTORX:
				ToggleCheckBox(dialog, number);
				break;
			case B_FOLDER:
			case T_FOLDER:
				/* Standard file dialog to select folder */
				if (SelectDirectory(&volume, &directory)) {
					Pathname(str, volume, directory);
					SetEText(dialog, T_FOLDER, str);
				}
				break;
			case R_XYMODEM:
			case R_ZMODEM:
				SetRadioButton(dialog, R_XYMODEM, R_ZMODEM, number);
				break;
		}
	}
}
