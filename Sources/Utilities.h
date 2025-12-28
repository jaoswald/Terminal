/*
	Terminal 2.2
	"Utilities.h"
*/

void ActivateDeactivate (DialogPtr, short, Boolean);
Byte *Append (Byte *, Byte *);
Boolean CheckSuffix(Byte *, Byte *);
void CenterDialog (long, short);
short CreateFile (short, long, Byte *, long, long);
short DeleteFile (short, long, Byte *);
Boolean DialogKeydown(DialogPtr, EventRecord *, short *);
void DisposeBuffer (Handle, Boolean);
pascal void DrawUserFrame (WindowPtr, short);
pascal void DrawUserLine (WindowPtr, short);
short FileRename (short, long, Byte *, Byte *);
short Filler (short, long);
short GetCheck (DialogPtr, short);
short GetRadioButton (DialogPtr, short, short);
void GetDlogOrigin (short, Point *);
void GetEText (DialogPtr, short, Byte *);
short InfoFile (short, long, Byte *, OSType *, OSType *, long *, long *);
unsigned long MaxBuffer (Boolean *);
Byte *MyString (short, short);
Handle NewBuffer (unsigned long, Boolean);
short OpenFile (short, long, Byte *, short *);
short OpenResource (short, long, Byte *, short *);
short Pathname (Byte *, short, long);
void SetCheck (DialogPtr, short, short);
void SetEText (DialogPtr, short, Byte *);
void SetHilite (DialogPtr, short, short);
short SetInfoFile (short, long, Byte *, OSType, OSType, long, long);
Boolean	SetRadioButton (DialogPtr, short, short, short);
void SetUserItem (DialogPtr, short, ProcPtr);
void ToggleCheckBox	(DialogPtr, short);
short VolumeId (Byte *, short *);

#ifdef OUTLINE
void OutLine (DialogPtr, short, short);
pascal Boolean OutlineFilter (DialogPtr, EventRecord *, short *);
#else
#define OutlineFilter 0
#endif
