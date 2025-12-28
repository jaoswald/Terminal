/*
	Terminal 2.0 script
	"Append.s"

	Append to capture file.
*/

main()
{
	int year, month, day, hour, minute, second, weekday, err;

	/* Get Macintosh date and time */

	date(time(), &year, &month, &day, &hour, &minute, &second, &weekday);

	/* Start capturing incoming data into a file */

	err = capture(2, "Capture");	/* Append */
	if (err && err != 2) {			/* Error */
		beep();
		return;
	}
	/* err == 2: capture file was already open */

	/* The following text is displayed and also captured into the file */

	display("\r*** Start of capture %02i-%02i-%04i %02i:%02i:%02i ***\r",
		day, month, year, hour, minute, second);
}
