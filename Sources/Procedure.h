/*
	Terminal 2.2
	"Procedure.h"
*/

short Display (Byte *);
short Download (Byte *, Boolean, short);
short Folder (short *, long *);
short PortSetUp (short, short, short, short, Byte *, short, short);
short TerminalSetup (short, short, short, short);
short TextsendSetup (Byte *, long, long);
short TransferSetup (short, short, short, short);
short Type (Byte *);
short Upload (Byte *, Boolean, short);
short XYModemSetup (Boolean, short, short, long);
short ZModemSetup (Boolean, long, long, long, long, long, long);
