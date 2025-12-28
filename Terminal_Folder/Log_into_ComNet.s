/*
	Terminal 2.0
	Script to dial and log into ComNet

	"Log into ComNet.s"
*/

int TIMEOUT = 1800;	/* 30 seconds */

/* ----- Send command and wait for reply ------------------------------- */

check(char *command, char *reply, int timeout)
{
	type(command);
	return prompt(reply, timeout);
}

/* ----- Wait for prompt and send command ------------------------------ */

answer(char *hint, char *command)
{
	int result;

	if (result = prompt(hint, TIMEOUT))
		return result;	/* Timeout or cancel */
	pause(30);			/* Don't type immediatly */
	type(command);
	return 0;
}

/* ----- main program -------------------------------------------------- */

main()
{
	int go = 1;

	setup(
		2,		/* 1200 baud */
		1,		/* 8 data */
		0,		/* no parity */
		0,		/* 1 stop */
		-1,		/* port: no change */
		-1,		/* DTR: no change */
		0);		/* Handshake: none */
	type("AT\r");
	pause(60);

	while (go) {
		if (check("AT\r", "OK", 60))
			break;
		pause(30);
		/*
		if (check("ATS7=60S10=14S9=12\r", "OK", 60))
			break;
		*/
		pause(30);
		if (check("ATDP22534\r", "CONNECT", TIMEOUT))	/* Dial */
			break;
		if (answer("Return", "\r"))				/* Wait for "Hit Return" */
			break;
		if (answer("<RETURN>", "\r"))			/* Select Graphics Style */
			break;
		if (answer("User", "2001604\r"))		/* Enter ID Number */
			break;
		if (answer("Password", "...\r"))		/* Enter password */
			break;
		go = 0;			/* Ok */
	}
	if (go) {		/* Timeout or cancel */
		setdtr(0);	/* Negate DTR: modem hangs up */
		pause(60);	/* Wait one second */
		setdtr(1);	/* Assert DTR: now back in command mode */
		beep();		/* 2 beeps if error */
	}
	beep();
}
