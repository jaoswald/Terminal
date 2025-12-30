#include "Menus.r"
#include "Types.r"

resource 'SIZE' (-1) {
	dontSaveScreen,
	acceptSuspendResumeEvents,
	enableOptionSwitch,
	canBackground,
	multiFinderAware,
	backgroundAndForeground,
	dontGetFrontClicks,
	ignoreChildDiedEvents,
	is32BitCompatible,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	reserved,
	163840,
	163840
};
resource 'MENU' (128) {
	128,
	textMenuProc,
	0x7FFFFFFD,
	enabled,
	apple,
	{
	"About Terminal...", noIcon, noKey, noMark, plain,
	"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (129) {
	129,
	textMenuProc,
	0x7FFFFBB7,
	enabled,
	"File",
	{
	  "Save capture buffer...", noIcon, noKey, noMark, plain,
	  "Text capture...", noIcon, noKey, noMark, plain,
	  "Text send...", noIcon, "S", noMark, plain,
	  "-", noIcon, noKey, noMark, plain,
	  "File receive...", noIcon, "R", noMark, plain,
	  "File transmit...", noIcon, "T", noMark, plain,
	  "-", noIcon, noKey, noMark, plain,
	  "Make MacBinary file...", noIcon, noKey, noMark, plain,
	  "Extract from MacBinary file...", noIcon, noKey, noMark, plain,
	  "Kiss script file...", noIcon, noKey, noMark, plain,
	  "-", noIcon, noKey, noMark, plain,
	  "Quit", noIcon, "Q", noMark, plain
	  }
};


resource 'MENU' (130) {
	130,
	textMenuProc,
	0x3DBD,
	enabled,
	"Edit",
	{
	  "Undo", noIcon, "Z", noMark, plain,
	  "-", noIcon, noKey, noMark, plain,
	  "Cut", noIcon, "X", noMark, plain,
	  "Copy", noIcon, "C", noMark, plain,
	  "Paste", noIcon, "V", noMark, plain,
	  "Clear", noIcon, noKey, noMark, plain,
	  "-", noIcon, noKey, noMark, plain,
	  "Clear capture buffer", noIcon, noKey, noMark, plain,
	  "Show progress window", noIcon, noKey, noMark, plain,
	  "-", noIcon, noKey, noMark, plain,
	  "Deblock send", noIcon, noKey, noMark, plain,
	  "Negate DTR", noIcon, noKey, noMark, plain,
	  "Assert DTR", noIcon, noKey, noMark, plain,
	  "Check CTS", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (131) {
	131,
	textMenuProc,
	0x7F,
	enabled,
	"Options",
	{
		"Communication...", noIcon, "M", noMark, plain,
		"TEXT file send...", noIcon, noKey, noMark, plain,
		"Terminal...", noIcon, noKey, noMark, plain,
		"Other...", noIcon, noKey, noMark, plain,
		"Binary File transfer...", noIcon, noKey, noMark, plain,
		"XY-Modem...", noIcon, noKey, noMark, plain,
		"Z-Modem...", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (132) {
	132,
	textMenuProc,
	0x7FFFFFFD,
	enabled,
	"Scripts",
	{
	"Execute script...", noIcon, "E", noMark, plain,
	"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (133) {
	133,
	textMenuProc,
	0x7FFFF001,
	enabled,
	"Macros",
	{
	"Load macros...", noIcon, "L", noMark, plain,
	"-", noIcon, noKey, noMark, plain,
	"", noIcon, "0", noMark, plain,
	"", noIcon, "1", noMark, plain,
	"", noIcon, "2", noMark, plain,
	"", noIcon, "3", noMark, plain,
	"", noIcon, "4", noMark, plain,
	"", noIcon, "5", noMark, plain,
	"", noIcon, "6", noMark, plain,
	"", noIcon, "7", noMark, plain,
	"", noIcon, "8", noMark, plain,
	"", noIcon, "9", noMark, plain
	}
};

resource 'MENU' (201) {
	201,
	textMenuProc,
	allEnabled,
	enabled,
	"Baud:",
	{
	"300", noIcon, noKey, noMark, plain,
	"600", noIcon, noKey, noMark, plain,
	"1200", noIcon, noKey, noMark, plain,
	"2400", noIcon, noKey, noMark, plain,
	"4800", noIcon, noKey, noMark, plain,
	"9600", noIcon, noKey, noMark, plain,
	"19200", noIcon, noKey, noMark, plain,
	"38400", noIcon, noKey, noMark, plain,
	"57600", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (202) {
	202,
	textMenuProc,
	allEnabled,
	enabled,
	"Data bits:",
	{
	"7", noIcon, noKey, noMark, plain,
	"8", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (203) {
	203,
	textMenuProc,
	allEnabled,
	enabled,
	"Parity:",
	{
	"none", noIcon, noKey, noMark, plain,
	"even", noIcon, noKey, noMark, plain,
	"odd", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (204) {
	204,
	textMenuProc,
	allEnabled,
	enabled,
	"Stop bits:",
	{
	"1", noIcon, noKey, noMark, plain,
	"2", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (200) {
	200,
	textMenuProc,
	allEnabled,
	enabled,
	"Port:",
	{
	"none", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (205) {
	205,
	textMenuProc,
	allEnabled,
	enabled,
	"Handshake:",
	{
	"none", noIcon, noKey, noMark, plain,
	"XON-XOFF", noIcon, noKey, noMark, plain,
	"CTS", noIcon, noKey, noMark, plain,
	"DTR", noIcon, noKey, noMark, plain,
	"CTS & DTR", noIcon, noKey, noMark, plain
	}
};

resource 'MBAR' (128) {
	{
	128,
	129,
	130,
	131,
	132,
	133
	}
};
