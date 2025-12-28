/*
	Terminal 2.2
	"SendOptions.c"
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

#define B_OK			1		/* "Ok" button */
#define B_CANCEL		2		/* "Cancel" button */
#define E_PROMPT		3		/* Wait after each line sent */
#define E_LINE_DELAY	4		/* Delay after each line sent */
#define E_CHAR_DELAY	5		/* Delay after each character sent */
#define U_TITLELINE		6		/* Underline */

/* ----- TEXT file send options dialog --------------------------------- */

void SendOptions(void)
{
	register DialogPtr dialog;
	register Byte str[256];
	short number;

	CenterDialog('DLOG', DLOG_SEND);
	if (!(dialog = GetNewDialog(DLOG_SEND, 0, (WindowPtr)-1L)))
		return;
	SetEText(dialog, E_PROMPT, Settings.prompt);
	NumToString(Settings.linedelay, str);
	SetEText(dialog, E_LINE_DELAY, str);
	NumToString(Settings.chardelay, str);
	SetEText(dialog, E_CHAR_DELAY, str);
	SelIText(dialog, E_PROMPT, 0, 32767);
	SetUserItem(dialog, U_TITLELINE, (ProcPtr)DrawUserLine);
	ShowWindow(dialog);
	do {
		ModalDialog(OutlineFilter, &number);
		switch(number) {
			case B_OK:
				{
					long linedelay, chardelay;

					GetEText(dialog, E_LINE_DELAY, str);
					StringToNum(str, &linedelay);
					if (linedelay < 0 || linedelay > 300) {
						SysBeep(1);
						SelIText(dialog, E_LINE_DELAY, 0, 32767);
						number = 0;
						break;
					}
					GetEText(dialog, E_CHAR_DELAY, str);
					StringToNum(str, &chardelay);
					if (chardelay < 0 || chardelay > 300) {
						SysBeep(1);
						SelIText(dialog, E_CHAR_DELAY, 0, 32767);
						number = 0;
						break;
					}
					GetEText(dialog, E_PROMPT, str);
					TextsendSetup(str, linedelay, chardelay);
				}
				break;
			case B_CANCEL:
				break;
		}
	} while (number != B_OK && number != B_CANCEL);
	DisposDialog(dialog);
}
