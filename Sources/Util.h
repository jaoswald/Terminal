
#define topLeft(r)		(((Point *) &(r))[0])
#define botRight(r)		(((Point *) &(r))[1])

/* The QuickDraw global variables structure does not
exist in THINK C, so use a macro to generate the names */

#define QD(x)			(qd.x)

/* Low-memory globals */

#define Time			(*(long *)0x20C)
#define Ticks			(*(long *)0x16A)
#define SCCRd			(*(Ptr *)0x1D8)
#define SFSaveDisk		(*(short *)0x214)
#define CurDirStore		(*(long *)0x398)
#define CurStackBase	(*(Ptr *)0x908)
#define GrayRgn			(*(RgnHandle *)0x9EE)
#define MemErr			(*(short *)0x220)
#define MBarHeight		(*(short *)0xBAA)

#define FALSE 0
#define TRUE 1
