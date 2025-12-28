/*
	Terminal 2.0
	Script to hangup using DTR line

	"Hangup DTR.s"
*/

main()
{
	setdtr(0);	/* Negate DTR: modem hangs up */
	pause(60);	/* Wait one second */
	setdtr(1);	/* Assert DTR: now back in command mode */
	beep();
}
