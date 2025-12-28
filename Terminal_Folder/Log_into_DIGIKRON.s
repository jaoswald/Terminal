/*
	Terminal 2.0
	Script to dial and log into DIGIKRON using MNP-5

	"Log into DIGIKRON.s"
*/

int TIMEOUT = 3600;	/* 60 seconds */

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
		5,		/* 9600 baud */
		1,		/* 8 data */
		0,		/* no parity */
		0,		/* 1 stop */
		-1,		/* port: no change */
		-1,		/* DTR: no change */
		4);		/* Handshake: CTS/DTR (hardware) */
	type("AT\r");
	pause(30);

	while (go) {
		/* The "ATZ1" sets my modem into MNP-5 mode */
		if (check("ATZ1\r", "OK", 60))
			break;
		pause(30);
		/* disable data compression */
/*		if (check("AT%%C0\r", "OK", 60))
			break; */
		pause(30);
		if (check("ATDP001,4082531309\r", "CONNECT", TIMEOUT))
			break;
		pause(60);
		type("\r\r");
		if (answer("Your first name?", "Erny\r"))
			break;
		if (answer("Your last name?", "Tontlinger\r"))
			break;
		if (answer("Your password?", "...\r"))
			break;
		go = 0;
	}
	if (go) {
		type("+++");
		pause(120);
		type("ATH\r");
		beep();
	}
	beep();
}
