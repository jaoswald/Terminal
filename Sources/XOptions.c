/*
	Terminal 2.2
	"XYOptions.c"
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
#define C_CRC		3		/* X-Modem CRC check box */
#define R_1K_N		4		/* No 1K radio button */
#define R_1K_A		5		/* Automatic 1K radio button */
#define R_1K_CK		6		/* "CK" type 1K radio button */
#define R_BATCH_N	7		/* No batch radio button */
#define R_BATCH_Y	8		/* Y-Modem batch radio button */
#define R_BATCH_R	9		/* RR batch radio button */
#define R_5			10		/* X-Modem timeout 5 seconds radio button */
#define R_10		11		/* X-Modem timeout 10 seconds radio button */
#define R_15		12		/* X-Modem timeout 15 seconds radio button */
#define R_20		13		/* X-Modem timeout 20 seconds radio button */
#define U_TITLELINE	14		/* Underline */
#define T_1K		15		/* 1K frame */
#define U_1K		16
#define T_BATCH		17		/* Batch frame */
#define U_BATCH		18
#define T_TIMEOUT	19		/* Timeout frame */
#define U_TIMEOUT	20

static Byte frames[] = { U_1K, U_BATCH, U_TIMEOUT };

/* ----- Activate/deactivate XModem options ---------------------------- */

static void ActivateX(
	register DialogPtr dialog,
	register Boolean active)
{
	ActivateDeactivate(dialog, R_1K_N, active);
	ActivateDeactivate(dialog, R_1K_A, active);
	ActivateDeactivate(dialog, R_1K_CK, active);
}

/* ----- X/Y-Modem options dialog -------------------------------------- */

void XYOptions(void)
{
	register DialogPtr dialog;
	register short i;
	register long n;
	register Boolean active;
	short number;

	CenterDialog('DLOG', DLOG_XYMODEM);
	if (!(dialog = GetNewDialog(DLOG_XYMODEM, 0, (WindowPtr)-1L)))
		return;
	SetUserItem(dialog, U_TITLELINE, (ProcPtr)DrawUserLine);
	for (i = 0; i < sizeof(frames); i++)
		SetUserItem(dialog, frames[i], (ProcPtr)DrawUserFrame);
	if (Settings.XModemCRC)
		active = TRUE;
	else {
		active = FALSE;
		Settings.XModem1K = 0;
	}
	ActivateX(dialog, active);
	switch(Settings.XModemtimeout) {
		case 5*60:
			i = R_5;
			break;
		case 15*60:
			i = R_15;
			break;
		case 20*60:
			i = R_20;
			break;
		default:
			i = R_10;
	}
	SetRadioButton(dialog, R_5, R_20, i);
	SetRadioButton(dialog, R_1K_N, R_1K_CK, R_1K_N + Settings.XModem1K);
	SetRadioButton(dialog, R_BATCH_N, R_BATCH_R, R_BATCH_N+Settings.batch);
	SetCheck(dialog, C_CRC, Settings.XModemCRC);
	ShowWindow(dialog);
	for (;;) {
		ModalDialog(0, &number);
		switch(number) {
			case B_OK:
				switch (GetRadioButton(dialog, R_5, R_20)) {
					case R_5:
						n = 5*60;
						break;
					case R_15:
						n = 15*60;
						break;
					case R_20:
						n = 20*60;
						break;
					default:
						n = 10*60;
				}
				XYModemSetup(
					GetCheck(dialog, C_CRC),
					GetRadioButton(dialog, R_1K_N, R_1K_CK) - R_1K_N,
					GetRadioButton(dialog, R_BATCH_N, R_BATCH_R) -
						R_BATCH_N,
					n);	/* Timeout */
			case B_CANCEL:
				DisposDialog(dialog);
				return;
			case C_CRC:
				if (active)
					SetRadioButton(dialog, R_1K_N, R_1K_CK, R_1K_N);
				ActivateX(dialog, active = !active);
				ToggleCheckBox(dialog, number);
				break;
			case R_1K_N:
			case R_1K_A:
			case R_1K_CK:
				SetRadioButton(dialog, R_1K_N, R_1K_CK, number);
				break;
			case R_5:
			case R_10:
			case R_15:
			case R_20:
				SetRadioButton(dialog, R_5, R_20, number);
				break;
			case R_BATCH_N:
			case R_BATCH_Y:
			case R_BATCH_R:
				SetRadioButton(dialog, R_BATCH_N, R_BATCH_R, number);
				break;
		}
	}
}
