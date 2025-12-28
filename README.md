# Terminal

`Terminal` is a serial communication program for the (Classic, Motorola
68k-based) Macintosh computer.

Features are:

* Fast display. No characters are lost for up to 4800 Baud on a Mac Plus,
  or up to 9600 Baud on a Mac IIcx in 1-bit color mode, with text capture to
  disk enabled. (For most commercial programs this only holds for short term
  until the serial input buffer is full.)
* Capture buffer, so that the last 32768 (default, can be configured, upper
  limit is 32768 lines) characters are always available in the scrolling
  terminal window. The buffer can be saved as TEXT file to disk or appended
  to an existing file. Very fast scrolling to move around in the buffer.
* Text capture to disk, so that everything received or transmitted is saved
  automatically to a TEXT file on disk. Can also be appended to an existing
  file.
* Send TEXT files from disk, from up to 10 different macro buffers or from
  the clipboard. Wait for prompt string before sending line, delay after each
  line, delay after each character.
* XON/XOFF, CTS, DTR and CTS/DTR handshake. Hardware handshake is useful
  for high speed modems or terminal node controllers.
* Binary file transfer using
  - X-Modem protocol (checksum, CRC or 1K options),
  - Y-Modem (i.e. XModem batch),
  - Z-Modem or
  - CompuServe QuickB protocol (up- and download).
* Automatic recognition of MacBinary (I and II) file format.
* Built-in C interpreter with a rich set of intrinsic functions to execute
  scripts from TEXT files. Scripts can be as simple as modem setup, dial or
  auto-logon, but can also be used to program a complete BBS.
* Very compact program. Only about 90K on disk and can run in a 160K
  partition under MultiFinder.
* Complete non-modal user driven program (even during file transfers). Runs
  in the background under MultiFinder (even if "Set Aside" under
  MultiFinder).
* Several copies of the program may be run at the same time,
  under MultiFinder, each one on a different serial port.
* Recognizes and uses all serial devices that are correctly registered with
  the Communications Toolbox.
* The program is free.
* Source code is free.

## Original Source

This source distribution is derived from the `Terminal_2.2.cpt.bin` archive
distributed by the orignal author, Erny Tontlinger, as found on

http://www.knubbelmac.de/software/internet-programme/telnet-ssh-terminal/
http://dl.knubbelmac.de/terminal-22.hqx

The goal of the Github version is to support compilation using the
[Retro68](https://github.com/autc04/Retro68) toolchain, where the original
was compiled using the Think C environment on the classic Mac OS.

## License

The original release was accompanied by the following permissive statement,
quoted from `Terminal.doc`.

```
This program is absolutely free, including the C sources. So do with it
what you like, but please include the documentation if you give it away.
```

This is presumably governed by the laws of Luxembourg, circa 1992. None of the
electronic contact information provided by the author appears usable as of
December 2025, so I have taken the liberty to "do with it what you like"
by applying the MIT license terms to this repository.
