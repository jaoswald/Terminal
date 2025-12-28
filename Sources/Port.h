/*
	Terminal 2.2
	"Serial.h"
*/

typedef Boolean (*SDPROC)(short, Byte *, Byte *, Byte *);

Boolean Serial8Bits(short);
void SerialAbort (void);
void SerialBinary (short);
long SerialCheck (void);
void SerialClose (void);
Boolean SerialCTS (void);
Boolean SerialDCD (void);
void SerialDevice(SDPROC);
void SerialDropDTR (Boolean);
void SerialDTR (Boolean);
long SerialDuration (short, short);
void SerialFastRead (Byte *, long);
void SerialFlush (void);
short SerialGetSetup (short, short *, short *, short *, short *);
void SerialHandshake (short);
short SerialOpen (Byte *, short, short);
long SerialRead (Byte *, long);
void SerialReadDeblock (short);
void SerialReadStop (short);
void SerialReset (short);
short SerialSetSetup (short, short, short, short, short *);
void SerialSend (Byte *, long, Boolean *);
void SerialWriteDeblock (void);
