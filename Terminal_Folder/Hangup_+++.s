/*
	Terminal 2.0
	Script to hangup using software command

	"Hangup +++.s"
*/

main()
{
	type("+++");	/* Go into command mode */
	pause(120);		/* Wait 2 seconds */
	type("ATH0\r");	/* Hang up */
	beep();
}
