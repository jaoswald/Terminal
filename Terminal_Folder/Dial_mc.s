/*
	Terminal 2.0
	Script to dial "mc"

	This shows how a script can dial a service that uses two or more phone
	numbers. All phone numbers are checked in sequence several times until
	a connection is made.

	"Dial mc.s"
*/

int Numbers = 2;
char *Number[] = { "0049 89 598423", "0049 89 596422" };
int Retries = 3;

char Prefix[] = "ATD";
char Setup[] = "AT S7=60 S10=14 S9=12\r";
char Ok[] = "OK\r";

int FINE = 0, TIMEOUT = 1, CANCEL = 2, ABORT = 3;

/* ----- Hang-up and put modem into command mode ----------------------- */

Hangup()
{
	setdtr(0);	/* Negate DTR: modem hangs up */
	pause(60);
	setdtr(1);	/* Assert DTR: now back in command mode */
	pause(60);
}

/* ----- Modem reset --------------------------------------------------- */

Reset()
{
	int err;

	type("ATZ\r");				/* Reset modem */
	if (err = prompt(Ok, 60))
		return err;				/* No modem connected */
	pause(30);
	return FINE;
}

/* ----- Dial and wait for "CONNECT" from modem ------------------------ */

Dial(int i)
{
	int err;

	type(Setup);						/* Modem setup */
	if (err = prompt(Ok, 60))
		return err;						/* No modem connected */
	pause(30);
	type("%s%s\r", Prefix, Number[i]);	/* Dial command */
	if (err = prompt("CONNECT", 1800))
		return err;						/* No modem connected */
	return FINE;
}

/* ----- main program -------------------------------------------------- */

main()
{
	int i, j, err;

	if (Reset())
		return;
	for (j = 0; j < Retries; ++j) {
		err = TIMEOUT;
		for (i = 0; err == TIMEOUT && i < Numbers; ++i)
			if (err = Dial(i))	/* Error, back into command mode */
				Hangup();
			else {				/* No error, we are connected */
				beep();
				return;
			}
	}
}
