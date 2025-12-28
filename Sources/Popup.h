/*
	Terminal 2.2
	"Popup.h"
*/

typedef struct {
	short item;		/* Dialog item id */
	short menu;		/* Popup menu id */
	MenuHandle h;	/* Popup menu handle */
	short choice;	/* Current choice (1...) */
} POPUP;

#define NEWPOPUP	/* Use downward pointing triangle */

Boolean PopupMousedown (DialogPtr, EventRecord *, short *);
void PopupInit (DialogPtr, POPUP *);
void PopupCleanup (void);
