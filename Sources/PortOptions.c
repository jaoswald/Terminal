/*
	Terminal 2.2
	"PortOptions.c"
*/

#ifdef THINK_C
#include "MacHeaders"
#endif
#ifdef applec
#pragma load ":(Objects):MacHeadersMPW"
#pragma segment Options
#endif

#include <Menus.h>
#include <StringCompare.h>

#include "Options.h"
#include "Strings.h"
#include "Utilities.h"
#include "Text.h"
#include "Main.h"
#include "Port.h"
#include "Procedure.h"
#include "Popup.h"

#define B_OK		1	/* OK button */
#define B_CANCEL	2	/* Cancel button */

#define U_POPUP1	3	/* Id of first popup menu user item */
#define U_PORT		3	/* Port name */
#define U_BAUD		4	/* Baud rate */
#define U_DATA		5	/* Data bits */
#define U_PARITY	6	/* Parity */
#define U_STOP		7	/* Stop bits */
#define U_HANDSHAKE	8	/* Handshake mode */
#define U_POPUP2	8	/* Id of last popup menu user item */

#define C_DROP_DTR	9	/* Don't drop DTR when quitting */
#define U_TITLELINE	10	/* Underline */

#define M_PORT		200	/* Menu, port */
#define M_BAUD		201	/* Menu, baud */
#define M_DATA		202	/* Menu, data bits */
#define M_PARITY	203	/* Menu, parity */
#define M_STOP		204	/* Menu, stop bits */
#define M_HANDSHAKE	205	/* Menu, handshake */

static POPUP Popup[] = {	/* Popup menu info */
	{ U_PORT, M_PORT, 0, 0 },
	{ U_BAUD, M_BAUD, 0, 0 },
	{ U_DATA, M_DATA, 0, 0 },
	{ U_PARITY, M_PARITY, 0, 0 },
	{ U_STOP, M_STOP, 0, 0 },
	{ U_HANDSHAKE, M_HANDSHAKE, 0, 0 },
	{ 0, 0, 0, 0 }
};

#define I_PORT		0
#define I_BAUD		1
#define I_DATA		2
#define I_PARITY	3
#define I_STOP		4
#define I_HANDSHAKE	5

/* ----- Filter function for dialog ------------------------------------ */

static pascal Boolean Filter(
	register DialogPtr dialog,
	register EventRecord *event,
	register short *item)
{
	SetPort(dialog);
	*item = 0;
	switch (event->what) {
		case keyDown:
			return DialogKeydown(dialog, event, item);
		case mouseDown:
			return PopupMousedown(dialog, event, item);
	}
	return FALSE;
}

/* ----- Loop thru registered serial ports ----------------------------- */

static short CurrentMenuItem;

static short LoopSerial(short index, Byte *name, Byte *input, Byte *output)
{
#pragma unused(input, output)
	register POPUP *p = &Popup[I_PORT];

	/* This AppendMenu() followed by SetItem() is necessary if there are
	any of the menu meta characters ( ';' '^' '!' '<' '/' '(' ) in the
	character string. These characters may be a valid part of the port
	name. */

	if (!p->h)
		return TRUE;	/* Stop */
	AppendMenu(p->h, "\p ");
	SetMenuItemText(p->h, ++CurrentMenuItem, name);
	if (EqualString(name, Settings.portName, FALSE, TRUE))
		p->choice = index + 1;
	return FALSE;		/* Continue */
}

/* ----- Port options dialog ------------------------------------------- */

void PortOptions(void)
{
	register DialogPtr dialog;
	short number, err;
	short baud, data, parity, stop;
	Byte port[256];

	CenterDialog('DLOG', DLOG_PORT);
	if (!(dialog = GetNewDialog(DLOG_PORT, 0, (WindowPtr)-1L)))
		return;

	/* Build port menu and setup popup menus */
	if (Popup[I_PORT].h = GetMenu(M_PORT)) {
		Popup[I_PORT].choice = CurrentMenuItem = 1;
		SerialDevice((SDPROC)LoopSerial);
	}
	PopupInit(dialog, Popup);

	SetUserItem(dialog, U_TITLELINE, (ProcPtr)DrawUserLine);
	SerialGetSetup(Settings.portSetup, &baud, &data, &parity, &stop);
	Popup[I_BAUD].choice = baud + 1;
	Popup[I_DATA].choice = data + 1;
	Popup[I_PARITY].choice = parity + 1;
	Popup[I_STOP].choice = stop + 1;
	Popup[I_HANDSHAKE].choice = Settings.handshake + 1;
	SetCheck(dialog, C_DROP_DTR, Settings.dropDTR);
	ShowWindow(dialog);

	for (;;) {
		ModalDialog(Filter, &number);
		switch(number) {
			case B_OK:
				if (Popup[I_PORT].choice > 1)
					GetMenuItemText(Popup[I_PORT].h, Popup[I_PORT].choice,
						port);
				else
					port[0] = 0;
				if (err = PortSetUp(
						Popup[I_BAUD].choice - 1,
						Popup[I_DATA].choice - 1,
						Popup[I_PARITY].choice - 1,
						Popup[I_STOP].choice - 1,
						port,
						GetCheck(dialog,C_DROP_DTR),
						Popup[I_HANDSHAKE].choice - 1)) {
					Error(err, EmptyStr);
				}
			case B_CANCEL:
				PopupCleanup();
				DisposeDialog(dialog);
				return;
			case C_DROP_DTR:
				ToggleCheckBox(dialog, number);
				break;
		}
	}
}
