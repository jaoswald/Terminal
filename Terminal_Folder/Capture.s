/*
	Terminal 2.0 script
	"Capture.s"

	Create a new unique capture file and open it.
*/

main()
{
	char name[32];
	int year, month, day, hour, minute, second, weekday;

	/* Get Macintosh date and time */

	date(time(), &year, &month, &day, &hour, &minute, &second, &weekday);

	/* Use date and time to build a unique file name */

	format(name, "Capture %04i%02i%02i%02i%02i%02i",
		year, month, day, hour, minute, second);

	/* Start capturing incoming data into that file */

	if (capture(1, name)) {
		beep();	/* If error */
		return;
	}

	/* The following text is displayed and also captured into the file */

	display("*** Start of capture file %02i-%02i-%04i %02i:%02i:%02i ***\r\r",
		day, month, year, hour, minute, second);
}
