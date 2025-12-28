/*
	Terminal 2.2
	"Scroll.c"
*/

#ifdef THINK_C
#include "MacHeaders"
#endif
#ifdef applec
#pragma load ":(Objects):MacHeadersMPW"
#pragma segment Main2
#endif

#include "Text.h"
#include "Main.h"
#include "Scroll.h"
#include "Document.h"

static WindowPtr Window;	/* Used by ScrollText(): Current window */
static short Page;			/* Used by ScrollText(): Page size */
static short Row;			/* Used by ScrollText(): Char height/width */

/* ----- Scroll window data -------------------------------------------- */

void Scroll(
	register DocumentPeek w,
	register short delta)
{
	RgnHandle updateRgn;

	updateRgn = NewRgn();
	ScrollRect(&w->rect, 0, delta * w->height, updateRgn);
	DrawDocument(w, delta);
	DisposeRgn(updateRgn);
}

/* ------ Scroll text within window (called by TrackControl()) --------- */

static pascal void ScrollText(
	register ControlHandle ch,
	register short part)
{
	register short delta;
	register short ex;

	switch (part) {
		case inUpButton:
			delta = -Row;
			break;
		case inDownButton:
			delta = Row;
			break;
		case inPageUp:
			delta = -Page;
			break;
		case inPageDown:
			delta = Page;
			break;
		default:
			return;
	}
	SetCtlValue(ch, (ex = GetCtlValue(ch)) + delta);
	if (ex -= GetCtlValue(ch))
		Scroll((DocumentPeek)Window, ex);
}

/* ----- Handle click in document window ------------------------------- */

void DocumentClick(
	register DocumentPeek window,
	register Point where,
	short modifiers)
{
#pragma unused(modifiers)
	register short part;
	register short ex;
	ControlHandle control;

	if ((part = FindControl(where, (WindowPtr)window, &control)) &&
			window->vs == control) {
		Row = 1;
		Page = window->linesPage - 1;
		if (part == inThumb) {
			ex = GetCtlValue(control);
			part = TrackControl(control, where, 0);
			SetCtlValue(control, GetCtlValue(control));
			if (ex -= GetCtlValue(control))
				Scroll(window, ex);
		} else {
			Window = (WindowPtr)window;
			part = TrackControl(control, where, (ProcPtr)ScrollText);
		}
		return;
	}
	if (control == window->messOk && TrackControl(control, where, 0)) {
		SetPort((GrafPtr)window);
		HideControl(control);
		EraseRect(&window->messRect);
		window->mess[0] = 0;
	} else {
		/* If click in document window, then redraw it */
		EraseRect(&window->rect);
		DrawDocument(window, 0);
	}
}

/* ----- Adjust scroll bar to size of data ----------------------------- */

void AdjustScrollBar(register DocumentPeek window)
{
	register short a;
	register short max;
	register ControlHandle c;

	a = (FrontWindow() == (WindowPtr)window) ? 0 : 255;
	c = window->vs;
	max = window->buf.lines - window->linesPage;
	if (max <= 0) {
		max = 0;
		HiliteControl(c, 255);
	} else
		HiliteControl(c, a);
	(**c).contrlMax = max;
	SetCtlValue(c, max);
}
