/*
	Terminal 2.2
	"Document.c"
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
#include "Document.h"
#include "Scroll.h"
#include "File.h"
#include "Port.h"
#include "CisB.h"
#include "ZModem.h"

#define BS		0x08	/* Backspace key */
#define ESC		'`'		/* Used as escape key */

/* ----- Draw cursor --------------------------------------------------- */

static void DrawCursor(register Point where, Rect frame)
{
	OffsetRect(&frame, where.h, where.v);
	PaintRect(&frame);
}

/* ----- Erase character ----------------------------------------------- */

static void EraseCharacter(register Point where, Rect frame)
{
	OffsetRect(&frame, where.h, where.v);
	EraseRect(&frame);
}

/* ----- Check to see if display window is entirely visible ------------ */

static Boolean Visible(register DocumentPeek window)
{
	register RgnHandle rgn1 = NewRgn(), rgn2 = NewRgn();
	register Boolean v;

	RectRgn(rgn1, &window->rect);
	SectRgn(rgn1, ((WindowPtr)window)->visRgn, rgn2);
	v = EqualRgn(rgn1, rgn2);
	DisposeRgn(rgn2);
	DisposeRgn(rgn1);
	return v;
}

/* ----- Draw message -------------------------------------------------- */

static void DrawMessage(register DocumentPeek window)
{
	EraseRect(&window->messRect);
	if (window->mess[0]) {
		MoveTo(window->messRect.left + 3, window->messRect.top + 13);
		DrawString(window->mess);
		FrameRect(&window->messRect);
	}
}

/* ----- Make new message ---------------------------------------------- */

void MakeMessage(
	register DocumentPeek window,
	register Byte *message)
{
	SetPort((GrafPtr)window);
	memcpy(window->mess, message, message[0] + 1);
	ShowControl(window->messOk);
	DrawMessage(window);
}

/* ----- Draw document window ------------------------------------------ */

void DrawDocument(
	register DocumentPeek window,
	short delta)					/* Number of lines to scroll */
{
	register long line1, line2, n;
	register short v;
	Byte s[256];
	Boolean lastLine;

	SetPort((GrafPtr)window);

	n = GetCtlValue(window->vs);			/* First visible line */
	if (delta > 0) {						/* Scroll back */
		line1 = n;							/* First lines in window */
		line2 = line1 + Min(delta, window->linesPage);
	} else {
		if (delta < 0) {					/* Scroll forward */
			line2 = n + window->linesPage;	/* Last lines in window */
			line1 = line2 - Min(-delta, window->linesPage);
		} else {							/* Draw all lines in window */
			MoveTo(window->rect.left - 2, window->rect.top - 3);
			LineTo(window->rect.right + 1, window->rect.top - 3);
			DrawMessage(window);
			line1 = n;
			line2 = line1 + window->linesPage;
		}
	}
	MoveTo(window->cursor0.h,
		v = window->cursor0.v + (line1 - n) * window->height);
	TextMode(srcOr);
	while (line1 < line2) {
		lastLine = FindLine(&window->buf, line1, s);
		DrawString(s);
		if (lastLine) {
			DrawCursor(window->cursor, window->character);
			break;
		}
		MoveTo(window->cursor0.h, v += window->height);
		++line1;
	}
}

/* ----- Draw document window after new line received ------------------ */

static void DrawDocumentX(register DocumentPeek window)
{
	register long line1, line2;
	register short v;
	register Boolean lastLine;
	register Byte s[256];
	Rect r;

	line2 = (line1 = GetCtlValue(window->vs)) + window->linesPage;
	MoveTo(r.left = window->cursor0.h, v = window->cursor0.v);
	TextMode(srcOr);
	r.right = r.left + (window->rect.right - window->rect.left);
	r.top = window->character.top;
	r.bottom = window->character.bottom;
	OffsetRect(&r, 0, v);
	while (line1 < line2) {
		EraseRect(&r);
		r.top += window->height;
		r.bottom += window->height;
		lastLine = FindLine(&window->buf, line1, s);
		DrawString(s);
		if (lastLine)
			break;
		MoveTo(window->cursor0.h, v += window->height);
		++line1;
	}
}

/* ----- Clear capture buffer ------------------------------------------ */

void ClearBuffer(void)
{
	register DocumentPeek window = TerminalWindow;
	register TextRecord *p;

	if (window->file)
		SysBeep(1);		/* Not while capture file is open! */
	else {
		p = &window->buf;
		p->lines = 1;
		p->firstChar = p->newChar = p->length =
			p->viewChar = p->viewLine = 0;
		(p->text)[0] = '\015';
		window->cursor = window->cursor0;
		SetCtlValue(window->vs, 0);
		AdjustScrollBar(window);
		SetPort((GrafPtr)window);
		EraseRect(&window->rect);
		DrawDocument(window, 0);
	}
}

/* ----- Redraw document ----------------------------------------------- */

void RedrawDocument(void)
{
	register DocumentPeek window = TerminalWindow;

	BeginUpdate((WindowPtr)window);
	DrawDocument(window, 0);
	DrawControls((WindowPtr)window);
	EndUpdate((WindowPtr)window);
}

/* ----- Activate document window -------------------------------------- */

void ActivateDocument(
	register DocumentPeek window,
	register short activate)
{
	HiliteControl(window->vs, activate ? 0 : 255);
	InitCursor();
}

/* ----- Go to next line ----------------------------------------------- */

static void NextLine(register DocumentPeek window)
{
	Rect r;

	AddNewCharacter(&window->buf, '\015');
	ByteCapture('\015');
	AdjustScrollBar(window);	/* New value/max for scroll bar */
	r = window->rect;
	window->cursor.h = window->cursor0.h;
	if (window->cursor.v < (r.bottom - window->height)) {
		window->cursor.v += window->height;
	} else {					/* We must scroll */
	
		/* If window->rect is entirely visible, use ScrollRect(),
		which is much faster than the redraw function. */
 		if (Visible(window)) {
			register RgnHandle rgn = NewRgn();
			ScrollRect(&r, 0, -window->height, rgn);
			DisposeRgn(rgn);
		} else
			DrawDocumentX(window);
	}
	MoveTo(window->cursor.h, window->cursor.v);
}

/* ----- Handle new characters ----------------------------------------- */

void NewCharacters(
	register Byte *buffer,
	register short count,
	register Boolean autorx)
{
	register DocumentPeek window = TerminalWindow;
	register Byte *start = buffer;
	register Byte b;
	register short n;
	short width;
	Rect r;

	SetPort((GrafPtr)window);
	r = window->rect;
	width = window->character.right - window->character.left;

	/* Scroll to end of document if necessary */

	{
		ControlPtr c = *(window->vs);

		if (n = c->contrlValue - c->contrlMax) {
			AdjustScrollBar(window);
			Scroll(window, n);
		}
	}

	/* Erase cursor and set pen position */

	MoveTo(window->cursor.h, window->cursor.v);
	EraseCharacter(window->cursor, window->character);

	/* Check all characters */

	while (count--) {
		b = *buffer++;

		/* Check for CompuServe-B */

		if (b == ENQ && autorx && Settings.protocol == 1 && !Transfer &&
				!Sending) {
			if ((n = buffer - start - 1) > 0) {
				DrawText(start, 0, n);
				GetPen(&window->cursor);
			}
			start = buffer;
			CisB();
			UnloadSeg(CisB);
			/* Meanwhile progress window was up */
			SetPort((GrafPtr)window);
			MoveTo(window->cursor.h, window->cursor.v);
			continue;
		}

		/* Check for ZModem autoreceive string */

		if (Settings.ZAutoReceive && autorx && !Transfer && !Sending) {
			memcpy(ZLastRx, ZLastRx + 1, ZAUTORX - 1);
			ZLastRx[ZAUTORX - 1] = b;
			if (!strcmp((char *)ZLastRx, (char *)ZAutoReceiveString)) {
				if ((n = buffer - start - 1) > 0) {
					DrawText(start, 0, n);
					GetPen(&window->cursor);
				}
				start = buffer;
				ZReceive();
				UnloadSeg(ZReceive);
				/* Meanwhile progress window was up */
				SetPort((GrafPtr)window);
				MoveTo(window->cursor.h, window->cursor.v);
				continue;
			}
		}

		/* Handle backspace character */

		if (b == Settings.backspace) {
			if ((n = buffer - start - 1) > 0) {
				DrawText(start, 0, n);
				GetPen(&window->cursor);
			}
			start = buffer;
			if (b = RemoveCharacter(&window->buf)) {
				ByteCapture(Settings.backspace);
				window->cursor.h -= width;	/* faster than CharWidth(b) */
				MoveTo(window->cursor.h, window->cursor.v);
				EraseCharacter(window->cursor, window->character);
			}
			continue;
		}

		/* Handle carriage return */

		if (b == '\015') {
			if ((n = buffer - start - 1) > 0) {
				DrawText(start, 0, n);
				GetPen(&window->cursor);
			}
			start = buffer;
			NextLine(window);
			continue;
		}

		/* Filter out control characters */

		if (b != '\t' && (b < 0x20 || b > 0x7E)) {
			if (b == 0x07 && Settings.beep)	/* Check for BEL */
				SysBeep(1);
			if ((n = buffer - start - 1) > 0) {
				DrawText(start, 0, n);
				GetPen(&window->cursor);
			}
			start = buffer;
			continue;
		}

		/* Handle printable characters */

		if (window->cursor.h >= r.right) {
			if ((n = buffer - start - 1) > 0) {
				DrawText(start, 0, n);
				GetPen(&window->cursor);
			}
			start = buffer - 1;
			NextLine(window);			/* Insert additional CR */
		} else
			window->cursor.h += width;	/* Faster than CharWidth(b) */
		AddNewCharacter(&window->buf, b);
		ByteCapture(b);
	}

	/* Make sure everything is drawn */

	if ((n = buffer - start) > 0) {
		DrawText(start, 0, n);
		GetPen(&window->cursor);
	}

	/* Draw cursor */

	if (window->cursor.h >= r.right)
		NextLine(window);			/* Insert additional CR */
	DrawCursor(window->cursor, window->character);
}

/* ----- Handle new character from keyboard ---------------------------- */

void NewKey(register Byte key)
{
	register Byte buffer[2];
	register long count;

	if (key == BS)
		key = Settings.backspace;
	else if (key == ESC)
		key = Settings.escape;
	buffer[0] = key;
	count = 1;
	if (key == '\015' && Settings.autoLF) {
		buffer[1] = '\012';
		count++;
	}
	while (Busy)	/* Other send in progress */
		;
	SerialSend(buffer, count, &Busy);
	if (Settings.localEcho)
		NewCharacters(buffer, 1, FALSE);
	CheckEvents();
}
