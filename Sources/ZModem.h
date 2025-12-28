/*
	Terminal 2.2
	"ZModem.h"
*/

#define ZAUTORX 18
extern Byte ZAutoReceiveString[];
extern Byte ZLastRx[];

short ZReceive (void);
short ZTransmit	(Byte *, short, long);
