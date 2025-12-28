/*
	Terminal 2.2
	"Interp.h"
*/

typedef long INTEGER;		/* Pointers and integers are mixed */

/* ----- Intrinsic function table (provided by shell) ------------------ */

typedef INTEGER (*IFUNC)(...);

typedef struct {
	Byte *fname;
	IFUNC fn;	/* Parameter is (INTEGER *) */
} INTRINSIC;

/* ----- Functions provided by interpreter ----------------------------- */

void SI_Load(INTRINSIC *, Byte *, long);	/* Load and link */
INTEGER SI_Interpret(void);					/* Start the interpreter */
INTEGER SI_stack(INTEGER *);				/* Remaining stack space */

/* ----- Functions provided by the shell ------------------------------- */

short SI_GetSource(void);					/* Get next char from source */
void SI_UngetSource(short);					/* char not needed now */
void SI_Error(short, Byte *, short);		/* Error from interpreter */
