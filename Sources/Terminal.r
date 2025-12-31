#include "Dialogs.r"
#include "MacTypes.r"
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
	"About Terminal\$c9", noIcon, noKey, noMark, plain,  // $c9 = `...`
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
	  "Save capture buffer\$c9", noIcon, noKey, noMark, plain,  // $c9 = `...`
	  "Text capture\$c9", noIcon, noKey, noMark, plain,
	  "Text send\$c9", noIcon, "S", noMark, plain,
	  "-", noIcon, noKey, noMark, plain,
	  "File receive\$c9", noIcon, "R", noMark, plain,
	  "File transmit\$c9", noIcon, "T", noMark, plain,
	  "-", noIcon, noKey, noMark, plain,
	  "Make MacBinary file\$c9", noIcon, noKey, noMark, plain,
	  "Extract from MacBinary file\$c9", noIcon, noKey, noMark, plain,
	  "Kiss script file\$c9", noIcon, noKey, noMark, plain,
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
		"Communication\$c9", noIcon, "M", noMark, plain,    // $c9 = `...`
		"TEXT file send\$c9", noIcon, noKey, noMark, plain,
		"Terminal\$c9", noIcon, noKey, noMark, plain,
		"Other\$c9", noIcon, noKey, noMark, plain,
		"Binary File transfer\$c9", noIcon, noKey, noMark, plain,
		"XY-Modem\$c9", noIcon, noKey, noMark, plain,
		"Z-Modem\$c9", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (132) {
	132,
	textMenuProc,
	0x7FFFFFFD,
	enabled,
	"Scripts",
	{
	"Execute script\$c9", noIcon, "E", noMark, plain,  // $c9 = `...`
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
	"Load macros\$c9", noIcon, "L", noMark, plain,    // $c9 = `...`
	"-", noIcon, noKey, noMark, plain,
	"\$00", noIcon, "0", noMark, plain,
	"\$00", noIcon, "1", noMark, plain,
	"\$00", noIcon, "2", noMark, plain,
	"\$00", noIcon, "3", noMark, plain,
	"\$00", noIcon, "4", noMark, plain,
	"\$00", noIcon, "5", noMark, plain,
	"\$00", noIcon, "6", noMark, plain,
	"\$00", noIcon, "7", noMark, plain,
	"\$00", noIcon, "8", noMark, plain,
	"\$00", noIcon, "9", noMark, plain
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

resource 'DLOG' (128) {
  {40, 40, 240, 280},
  documentProc,
  invisible,
  noGoAway,
  0,  /* refCon */
  136, /* DITL Id */
  "",
  noAutoCenter
};

resource 'DLOG' (129, purgeable) {
  {42, 14, 210, 258},
  dBoxProc,
  visible,
  noGoAway,
  0, /* refCon */
  129, /* DITL Id */
  "",
  noAutoCenter
};

resource 'DLOG' (130, purgeable) {
  {36, 15, 256, 280},
  dBoxProc,
  invisible,
  noGoAway,
  0, /* refCon */
  130, /* DITL Id */
  "",
  noAutoCenter
};

resource 'DLOG' (131, purgeable) {
  {51, 7, 171, 448},
  noGrowDocProc,
  visible,
  noGoAway,
  0, /* refCon */
  131, /* DITL Id */
  "File transfer",
  noAutoCenter
};

resource 'DLOG' (132, purgeable) {
  {80, 80, 304, 380},
  dBoxProc,
  invisible,
  noGoAway,
  0, /* refCon */
  132, /* DITL Id */
  "",
  noAutoCenter,
};

resource 'DLOG' (133, purgeable) {
  {40, 40, 216, 292},
  dBoxProc,
  invisible,
  noGoAway,
  0, /* refCon */
  133, /* DITL Id */
  "",
  noAutoCenter
};

resource 'DLOG' (134, purgeable) {
  {36, 15, 187, 374},
  dBoxProc,
  invisible,
  noGoAway,
  0, /* refCon */
  134, /* DITL Id */
  "",
  noAutoCenter
};

resource 'DLOG' (135, purgeable) {
  {40, 40, 308, 299},
  dBoxProc,
  invisible,
  noGoAway,
  0, /* refCon */
  135, /* DITL Id */
  "",
  noAutoCenter
};

resource 'DLOG' (136, purgeable) {
  {40, 40, 309, 310},
  dBoxProc,
  invisible,
  noGoAway,
  0, /* refCon */
  136, /* DITL Id */
  "New Dialog",
  noAutoCenter
};

resource 'DLOG' (137, purgeable) {
  {40, 40, 216, 439},
  dBoxProc,
  invisible,
  noGoAway,
  0, /* refCon */
  137, /* DITL Id */
  "",
  noAutoCenter
};

resource 'DITL' (131, purgeable) {
	{
	{6, 375, 22, 435},
	Button {
		enabled,
		"Cancel"
	},
	{6, 6, 22, 166},
	StaticText {
		disabled,
		""
	},
	{6, 164, 22, 370},
	StaticText {
		disabled,
		""
	},
	{28, 6, 44, 336},
	StaticText {
		disabled,
		""
	},
	{65, 374, 81, 434},
	StaticText {
		disabled,
		""
	},
	{28, 374, 44, 434},
	StaticText {
		disabled,
		""
	},
	{43, 374, 59, 434},
	StaticText {
		disabled,
		""
	},
	{58, 16, 74, 257},
	UserItem {
		disabled
	},
	{97, 48, 113, 108},
	StaticText {
		disabled,
		""
	},
	{97, 106, 113, 328},
	StaticText {
		disabled,
		""
	},
	{97, 374, 113, 434},
	StaticText {
		disabled,
		""
	},
	{74, 14, 90, 267},
	Picture {
		disabled,
		128
	},
	{43, 334, 59, 375},
	StaticText {
		disabled,
		"Total:"
	},
	{65, 309, 81, 375},
	StaticText {
		disabled,
		"Time left:"
	},
	{97, 6, 113, 50},
	StaticText {
		disabled,
		"Block:"
	},
	{97, 327, 113, 375},
	StaticText {
		disabled,
		"Errors:"
	},
	{28, 334, 44, 375},
	StaticText {
		disabled,
		"Done:"
	}
}
};

resource 'DITL' (133, purgeable) {
	{
	{147, 148, 167, 208},
	Button {
		enabled,
		"OK@"    // TODO: fix reliance on 'Default' CDEF
	},
	{147, 52, 167, 112},
	Button {
		enabled,
		"Cancel"
	},
	{35, 21, 51, 81},
	CheckBox {
		enabled,
		"CRC"
	},
	{75, 21, 91, 81},
	RadioButton {
		enabled,
		"off"
	},
	{90, 21, 106, 81},
	RadioButton {
		enabled,
		"auto"
	},
	{105, 21, 121, 81},
	RadioButton {
		enabled,
		"CK"
	},
	{75, 98, 91, 158},
	RadioButton {
		enabled,
		"off"
	},
	{90, 98, 106, 158},
	RadioButton {
		enabled,
		"Y"
	},
	{105, 98, 121, 158},
	RadioButton {
		enabled,
		"RR"
	},
	{60, 174, 76, 234},
	RadioButton {
		enabled,
		"\$ca5"  // $ca = non-breaking space
	},
	{75, 174, 91, 234},
	RadioButton {
		enabled,
		"10"
	},
	{90, 174, 106, 234},
	RadioButton {
		enabled,
		"15"
	},
	{105, 174, 121, 234},
	RadioButton {
		enabled,
		"20"
	},
	{20, 6, 22, 247},
	UserItem {
		disabled
	},
	{61, 41, 76, 62},
	StaticText {
		disabled,
		"1K"
	},
	{69, 16, 126, 86},
	UserItem {
		disabled
	},
	{60, 107, 76, 148},
	StaticText {
		disabled,
		"Batch"
	},
	{69, 93, 126, 163},
	UserItem {
		disabled
	},
	{46, 177, 61, 231},
	StaticText {
		disabled,
		"Timeout"
	},
	{54, 169, 126, 239},
	UserItem {
		disabled
	},
	{2, 68, 19, 205},
	StaticText {
		disabled,
		"X/Y-Modem options"
	}
	}
};

resource 'DITL' (134, purgeable) {
	{
	{121, 288, 141, 348},
	Button {
		enabled,
		"OK@"     // TODO: fix reliance on 'Default' CDEF
	},
	{121, 214, 141, 274},
	Button {
		enabled,
		"Cancel"
	},
	{39, 223, 55, 346},
	EditText {
		enabled,
		""
	},
	{64, 296, 80, 346},
	EditText {
		enabled,
		""
	},
	{89, 296, 105, 346},
	EditText {
		enabled,
		""
	},
	{26, 7, 28, 353},
	UserItem {
		disabled
	},
	{39, 6, 55, 218},
	StaticText {
		disabled,
		"Wait after each line sent for:"
	},
	{64, 6, 80, 291},
	StaticText {
		disabled,
		"Delay after each line sent (ticks):"
	},
	{89, 6, 105, 291},
	StaticText {
		disabled,
		"Delay after each character sent (ticks):"
	},
	{7, 105, 24, 254},
	StaticText {
		disabled,
		"TEXT file send options"
	}
	}
};

resource 'DITL' (135, purgeable) {
	{
	{239, 152, 259, 212},
	Button {
		enabled,
		"OK@"    // TODO: fix reliance on 'Default' CDEF
	},
	{239, 52, 259, 112},
	Button {
		enabled,
		"Cancel"
	},
	{33, 194, 49, 248},
	EditText {
		enabled,
		""
	},
	{58, 194, 74, 248},
	EditText {
		enabled,
		""
	},
	{83, 194, 99, 248},
	EditText {
		enabled,
		""
	},
	{108, 194, 124, 226},
	EditText {
		enabled,
		""
	},
	{133, 194, 149, 226},
	EditText {
		enabled,
		""
	},
	{160, 7, 176, 149},
	CheckBox {
		enabled,
		"\$d2control-G\$d3 beeps"  // $d2,$d3 are open/close double-quote marks
	},
	{205, 17, 221, 89},
	RadioButton {
		enabled,
		"control"
	},
	{205, 88, 221, 153},
	RadioButton {
		enabled,
		"option"
	},
	{205, 152, 221, 237},
	RadioButton {
		enabled,
		"command"
	},
	{188, 83, 205, 162},
	StaticText {
		disabled,
		"Control key"
	},
	{196, 11, 228, 243},
	UserItem {
		disabled
	},
	{20, 7, 22, 254},
	UserItem {
		disabled
	},
	{3, 81, 19, 175},
	StaticText {
		disabled,
		"Other options"
	},
	{33, 8, 49, 190},
	StaticText {
		disabled,
		"Capture TEXT file creator:"
	},
	{58, 8, 74, 190},
	StaticText {
		disabled,
		"Non-Macbinary file - type:"
	},
	{84, 120, 100, 190},
	StaticText {
		disabled,
		"- creator:"
	},
	{108, 8, 124, 190},
	StaticText {
		disabled,
		"Code for \$d2Backspace\$d3 key:"  // $d2,$d3 are open/close double-quote marks
	},
	{133, 8, 149, 190},
	StaticText {
		disabled,
		"Code for \$d2 ` \$d3 key:"
	}
	}
};

resource 'DITL' (129, purgeable) {
	{
	{136, 212, 168, 244},
	Icon {
		enabled,
		129
	},
	{45, 0, 124, 245},
	StaticText {
		disabled,
		""
	},
	{0, 0, 32, 32},
	UserItem {
		disabled
	},
	{149, 124, 165, 190},
	StaticText {
		disabled,
		""
	},
	{0, 43, 32, 245},
	StaticText {
		disabled,
		""
	},
	{38, 0, 40, 245},
	UserItem {
		disabled
	},
	{129, 0, 131, 245},
	UserItem {
		disabled
	},
	{149, 2, 165, 125},
	StaticText {
		disabled,
		"Heap free (bytes):"
	}
	}
};

resource 'DITL' (130, purgeable) {
	{
	{189, 139, 209, 199},
	Button {
		enabled,
		"OK@"  // TODO: fix reliance on 'Default' CDEF
	},
	{189, 63, 209, 123},
	Button {
		enabled,
		"Cancel"
	},
	{48, 99, 64, 252},
	CheckBox {
		enabled,
		"Display and capture"
	},
	{63, 99, 79, 173},
	CheckBox {
		enabled,
		"Auto LF"
	},
	{48, 9, 64, 79},
	CheckBox {
		enabled,
		"Local"
	},
	{63, 9, 79, 79},
	CheckBox {
		enabled,
		"Remote"
	},
	{33, 27, 49, 62},
	StaticText {
		disabled,
		"Echo"
	},
	{41, 4, 87, 85},
	UserItem {
		disabled
	},
	{150, 9, 166, 73},
	CheckBox {
		enabled,
		"Enable"
	},
	{151, 197, 167, 253},
	Button {
		enabled,
		"Select"
	},
	{129, 10, 145, 253},
	StaticText {
		disabled,
		""
	},
	{108, 81, 125, 176},
	StaticText {
		disabled,
		"Startup script"
	},
	{117, 5, 173, 258},
	UserItem {
		disabled
	},
	{6, 66, 24, 180},
	StaticText {
		disabled,
		"Terminal options"
	},
	{23, 5, 25, 258},
	UserItem {
		disabled
	}
	}
};

resource 'DITL' (128, purgeable) {
	{
	{44, 223, 64, 283},
	Button {
		enabled,
		"Ok@"  // TODO: fix reliance on 'Default' CDEF
	},
	{5, 45, 37, 283},
	StaticText {
		disabled,
		"^0"
	},
	{5, 6, 37, 38},
	Icon {
		disabled,
		129
	}
}
};

resource 'DITL' (132, purgeable) {
	{
	{196, 230, 216, 290},
	Button {
		enabled,
		"OK@"  // TODO: fix reliance on 'Default' CDEF
	},
	{165, 230, 185, 290},
	Button {
		enabled,
		"Cancel"
	},
	{40, 90, 56, 290},
	UserItem {
		enabled
	},
	{65, 90, 81, 290},
	UserItem {
		enabled
	},
	{90, 90, 106, 290},
	UserItem {
		enabled
	},
	{115, 90, 131, 290},
	UserItem {
		enabled
	},
	{140, 90, 156, 290},
	UserItem {
		enabled
	},
	{165, 90, 181, 220},
	UserItem {
		enabled
	},
	{196, 5, 216, 215},
	CheckBox {
		enabled,
		"Don't drop DTR when quitting"
	},
	{18, 3, 20, 298},
	UserItem {
		disabled
	},
	{0, 66, 17, 224},
	StaticText {
		disabled,
		"Communication options"
	}
	}
};

resource 'DITL' (136, purgeable) {
	{
	{243, 157, 263, 217},
	Button {
		enabled,
		"OK@"  // TODO: fix reliance on 'Default' CDEF
	},
	{243, 64, 263, 124},
	Button {
		enabled,
		"Cancel"
	},
	{38, 2, 54, 216},
	CheckBox {
		enabled,
		"Escape all control characters"
	},
	{60, 199, 76, 254},
	EditText {
		enabled,
		""
	},
	{81, 199, 97, 254},
	EditText {
		enabled,
		""
	},
	{122, 199, 138, 254},
	EditText {
		enabled,
		""
	},
	{167, 199, 183, 254},
	EditText {
		enabled,
		""
	},
	{188, 199, 204, 254},
	EditText {
		enabled,
		""
	},
	{209, 199, 225, 254},
	EditText {
		enabled,
		""
	},
	{28, 2, 30, 270},
	UserItem {
		disabled
	},
	{104, 110, 119, 165},
	StaticText {
		disabled,
		"Receive"
	},
	{113, 7, 147, 264},
	UserItem {
		disabled
	},
	{148, 107, 164, 169},
	StaticText {
		disabled,
		"Transmit"
	},
	{155, 7, 235, 264},
	UserItem {
		disabled
	},
	{7, 85, 23, 210},
	StaticText {
		disabled,
		"Z-Modem options"
	},
	{60, 13, 76, 193},
	StaticText {
		disabled,
		"Timeout (seconds):"
	},
	{81, 13, 97, 193},
	StaticText {
		disabled,
		"Maximal retries:"
	},
	{122, 14, 138, 194},
	StaticText {
		disabled,
		"Buffer size (bytes):"
	},
	{167, 14, 183, 194},
	StaticText {
		disabled,
		"Sub-packet length (Bytes):"
	},
	{188, 14, 204, 194},
	StaticText {
		disabled,
		"Window limit (Bytes):"
	},
	{209, 14, 225, 194},
	StaticText {
		disabled,
		"ZCRCQ spacing (Bytes):"
	}
}
};

resource 'DITL' (137, purgeable) {
	{
	{106, 321, 126, 381},
	Button {
		enabled,
		"OK@"  // TODO: fix reliance on 'Default' CDEF
	},
	{73, 320, 93, 380},
	Button {
		enabled,
		"Cancel"
	},
	{31, 4, 47, 389},
	CheckBox {
		enabled,
		"Recognize and use MacBinary format for non-TEXT files"
	},
	{72, 16, 87, 175},
	CheckBox {
		enabled,
		"CompuServe Quick-B"
	},
	{86, 16, 102, 116},
	RadioButton {
		enabled,
		"X/Y-Modem"
	},
	{101, 16, 117, 116},
	RadioButton {
		enabled,
		"Z-Modem"
	},
	{101, 115, 117, 220},
	CheckBox {
		enabled,
		"Auto receive"
	},
	{133, 98, 150, 297},
	Button {
		enabled,
		"Path for up- and downloads:"
	},
	{153, 7, 169, 392},
	StaticText {
		disabled,
		""
	},
	{20, 6, 22, 390},
	UserItem {
		disabled
	},
	{55, 75, 71, 135},
	StaticText {
		disabled,
		"Protocol"
	},
	{63, 9, 123, 227},
	UserItem {
		disabled
	},
	{2, 111, 18, 293},
	StaticText {
		disabled,
		"Binary file transfer options"
	}
}
};

data 'ICN#' (128, purgeable) {
	$"0000 0000 0000 0008 0000 0010 0000 0008"
	$"0000 0004 0000 0008 0000 0010 0700 0220"
	$"0F80 0540 1FC0 0880 3FE0 0400 3FE0 0200"
	$"7FE0 0400 7FE0 0800 7FC1 1000 7F82 A000"
	$"3E04 4000 3E02 0000 1E11 0000 1F2A 0000"
	$"0F84 0000 0FC0 3C00 07E0 7E00 03F0 FF00"
	$"01F8 FF80 00FF FF80 007F FF80 003F FF00"
	$"000F FE00 0003 FC00 0000 F000 0000 0000"
	$"0000 0000 0000 000C 0000 0018 0000 000C"
	$"0000 0006 0000 000C 0700 0018 0F80 0330"
	$"1FC0 07E0 3FE0 0CC0 7FF0 0600 7FF0 0300"
	$"FFF0 0600 FFF0 0C00 FFE1 9800 FFC3 F000"
	$"7F06 6000 7F03 0000 3F19 8000 3FBF 0000"
	$"1FC6 1C00 1FE0 7E00 0FF0 FF00 07F9 FF80"
	$"03FF FFC0 01FF FFC0 00FF FFC0 007F FF80"
	$"003F FF00 000F FE00 0003 FC00 0000 F000"
};

data 'ICN#' (129, purgeable) {
	$"0000 0004 7775 D2E8 5525 5A90 5725 56E8"
	$"5425 5224 7425 D2E8 0000 0010 0700 0220"
	$"0F80 0540 1FC0 0880 3FE0 0400 3FE0 0200"
	$"7FE0 0400 7FE0 0800 7FC1 1000 7F82 A000"
	$"3E04 4000 3E02 0000 1E11 0000 1F2A 0000"
	$"0F84 0000 0FC0 3C00 07E0 7E00 03F0 FF00"
	$"01F8 FF80 00FF FF80 007F FF80 003F FF00"
	$"000F FE00 0003 FC00 0000 F000 0000 0000"
	$"FFFF FFF0 FFFF FFFC FFFF FFF8 FFFF FFFC"
	$"FFFF FFF6 FFFF FFFC FFFF FFF8 0F80 0330"
	$"1FC0 07E0 3FE0 0CC0 7FF0 0600 7FF0 0300"
	$"FFF0 0600 FFF0 0C00 FFE1 9800 FFC3 F000"
	$"7F06 6000 7F03 0000 3F19 8000 3FBF 0000"
	$"1FC6 1C00 1FE0 7E00 0FF0 FF00 07F9 FF80"
	$"03FF FFC0 01FF FFC0 00FF FFC0 007F FF80"
	$"003F FF00 000F FE00 0003 FC00 0000 F000"
};

data 'ICN#' (130, purgeable) {
	$"0000 0000 7524 F448 55AA 9290 756E F108"
	$"552A A104 752A 9108 0000 0010 0700 0220"
	$"0F80 0540 1FC0 0880 3FE0 0400 3FE0 0200"
	$"7FE0 0400 7FE0 0800 7FC1 1000 7F82 A000"
	$"3E04 4000 3E02 0000 1E11 0000 1F2A 0000"
	$"0F84 0000 0FC0 3C00 07E0 7E00 03F0 FF00"
	$"01F8 FF80 00FF FF80 007F FF80 003F FF00"
	$"000F FE00 0003 FC00 0000 F000 0000 0000"
	$"FFFF FFF0 FFFF FFFC FFFF FFF8 FFFF FFFC"
	$"FFFF FFF6 FFFF FFFC FFFF FFF8 0F80 0330"
	$"1FC0 07E0 3FE0 0CC0 7FF0 0600 7FF0 0300"
	$"FFF0 0600 FFF0 0C00 FFE1 9800 FFC3 F000"
	$"7F06 6000 7F03 0000 3F19 8000 3FBF 0000"
	$"1FC6 1C00 1FE0 7E00 0FF0 FF00 07F9 FF80"
	$"03FF FFC0 01FF FFC0 00FF FFC0 007F FF80"
	$"003F FF00 000F FE00 0003 FC00 0000 F000"
};

data 'ICN#' (131, purgeable) {
	$"0000 0000 7BD7 B808 4254 9010 7BD7 9008"
	$"0A94 1004 7A54 1008 0000 0010 0700 0220"
	$"0F80 0540 1FC0 0880 3FE0 0400 3FE0 0200"
	$"7FE0 0400 7FE0 0800 7FC1 1000 7F82 A000"
	$"3E04 4000 3E02 0000 1E11 0000 1F2A 0000"
	$"0F84 0000 0FC0 3C00 07E0 7E00 03F0 FF00"
	$"01F8 FF80 00FF FF80 007F FF80 003F FF00"
	$"000F FE00 0003 FC00 0000 F000 0000 0000"
	$"FFFF FC00 FFFF FC0C FFFF FC18 FFFF FC0C"
	$"FFFF FC06 FFFF FC0C FFFF FC18 0F80 0330"
	$"1FC0 07E0 3FE0 0CC0 7FF0 0600 7FF0 0300"
	$"FFF0 0600 FFF0 0C00 FFE1 9800 FFC3 F000"
	$"7F06 6000 7F03 0000 3F19 8000 3FBF 0000"
	$"1FC6 1C00 1FE0 7E00 0FF0 FF00 07F9 FF80"
	$"03FF FFC0 01FF FFC0 00FF FFC0 007F FF80"
	$"003F FF00 000F FE00 0003 FC00 0000 F000"
};

data 'ICN#' (132) {
	$"0000 0000 7077 7708 1055 5210 2777 7208"
	$"4045 6204 7045 5208 0000 0010 0700 0220"
	$"0F80 0540 1FC0 0880 3FE0 0400 3FE0 0200"
	$"7FE0 0400 7FE0 0800 7FC1 1000 7F82 A000"
	$"3E04 4000 3E02 0000 1E11 0000 1F2A 0000"
	$"0F84 0000 0FC0 3C00 07E0 7E00 03F0 FF00"
	$"01F8 FF80 00FF FF80 007F FF80 003F FF00"
	$"000F FE00 0003 FC00 0000 F000 0000 0000"
	$"FFFF FF80 FFFF FF8C FFFF FF98 FFFF FF8C"
	$"FFFF FF86 FFFF FF8C FFFF FF98 0F80 0330"
	$"1FC0 07E0 3FE0 0CC0 7FF0 0600 7FF0 0300"
	$"FFF0 0600 FFF0 0C00 FFE1 9800 FFC3 F000"
	$"7F06 6000 7F03 0000 3F19 8000 3FBF 0000"
	$"1FC6 1C00 1FE0 7E00 0FF0 FF00 07F9 FF80"
	$"03FF FFC0 01FF FFC0 00FF FFC0 007F FF80"
	$"003F FF00 000F FE00 0003 FC00 0000 F000"
};

resource 'ICON' (129, purgeable) {
	$"0100 0000 011B FC00 077B FF00 1BFB FF80"
	$"07F3 BFC0 1FE3 CFE0 27C1 C070 0780 E070"
	$"0600 6030 0600 0030 047E 3F10 0F81 C0F8"
	$"1701 4074 1501 C054 150D 5854 150D 5854"
	$"0C82 2098 047C 1F10 0403 E010 0400 0010"
	$"0408 0810 020F F820 0103 E040 0080 0080"
	$"0040 0100 0038 0E00 0027 F200 00E0 0380"
	$"03FF FFE0 3FFF FFFE 7FFF FFFF 7FFF FFFF"
};

resource 'ALRT' (128, purgeable) {
  {40, 93, 112, 384},
  128, /* DITL Id */
  {
    OK, visible, sound1,
    OK, visible, sound1,
    OK, visible, sound1,
    OK, visible, sound1,
  },
  noAutoCenter
};

resource 'STR#' (128, locked, preload) {
{ "Capture text in file:",
  "Capture",
  "Save capture buffer as:",
  "Buffer",
  "Send@",    // TODO: fix reliance on 'Default' CDEF
  "Receive@", // TODO: fix reliance on 'Default' CDEF
  "Save file to receive as:",
  "Download",
  "Transmit@", // TODO: fix reliance on 'Default' CDEF
  ".BIN",
  "Make@", // TODO: fix reliance on 'Default' CDEF
  "Output file name",
  "Extract@", // TODO: fix reliance on 'Default' CDEF
  "Save file extracted as:",
  "Terminal Settings",
  "Terminal",
  "Select@", // TODO: fix reliance on 'Default' CDEF
  "Script@", // TODO: fix reliance on 'Default' CDEF
  ".s",
  "Terminal Folder",
  "Macros.m",
  ".m",
  "Macros@", // TODO: fix reliance on 'Default' CDEF
  "Open@", // TODO: fix reliance on 'Default' CDEF
  "Save@", // TODO: fix reliance on 'Default' CDEF
  "Append@", // TODO: fix reliance on 'Default' CDEF
  "Kiss@"
  }
};

resource 'STR#' (129, locked, preload) {
{
  "Verified",
  "Timeout",
  "Invalid character",
  "Repeated",
  "Wrong Checksum/CRC",
  "Block number error",
  "Wrong block number",
  "X-Modem ",
  "CRC ",
  "checksum ",
  "(TEXT)",
  "(MacBinary I)",
  "(MacBinary II)",
  "Sending text file: %s",
  "Receiving file:",
  "Transmitting file:",
  "1K ",
  "Compuserve B file:",
  "Upload ",
  "Download ",
  "End of transmission",
  "Re-verified",
  "Y-Modem ",
  "Negociating", // [sic]
  "QuickB ",
  "SendAhead ",
  "Sending clipboard"
}
};

resource 'STR#' (130, locked, preload) {
{ "Error #",
  "File transfer completed: %l bytes in %s [%l bytes/sec]  (err=%i)",
  "File transfer in progress",
  "Unsupported configuration"
}
};

resource 'STR#' (131, locked, preload) {
{ "\$d2%s\$d3 %s on line %i",
  "%s on line %i",
  "Unexpected end of file",
  "unrecognized",
  "duplicate identifier",
  "Symbol table full",
  "Out of heap memory",
  "undeclared identifier",
  "Syntax error",
  "unmatched",
  "missing",
  "Not a function",
  "out of place",
  "Token buffer overflow",
  "Divide by zero",
  "Pointer error",
  "Parameter error"
}
};

resource 'FREF' (128, purgeable) {
  'APPL',
  0,
  ""
};

resource 'FREF' (129, purgeable) {
  'ET_O',
  1,
  ""
};

resource 'FREF' (130, purgeable) {
  'ET_B',
  2,
  ""
};

resource 'FREF' (131, purgeable) {
  'TEXT',
  3,
  ""
};

resource 'FREF' (132) {
  'ET_Z',
  4,
  ""
};

resource 'BNDL' (128, purgeable) {
  'ET_T',  // signature
  0,
  { 'FREF',
    { 0, 128,
      1, 129, /* ET_O */
      2, 130, /* ET_B */
      3, 131, /* TEXT */
      4, 132   /* ET_Z */
    },
    'ICN#',
    { 0, 128,
      1, 129,
      2, 130,
      3, 131,
      4, 132 }
  }
};

data 'ET_T' (0, purgeable) {
  "\$a0\$d2Terminal\$d3 is a compact and powerful serial communication program. "
  "It's exactly what I wanted for my personal needs. "
  "If you find it usefull, keep it. It's free!"
};

// A configuration resource for the Terminal application.
type 'CNFG' {
  fill word;
  integer; /* font size */
  integer; /* lines */
  integer; /* columns */

  longint; /* terminal buffer size */
  longint; /* serial input buffer size */
  fill long;
  longint; /* script memory size */
  pstring; /* font name */
};

// Default configuration compiled into Terminal application.
resource 'CNFG' (128) {
    9,       /* font size */
    24,      /* lines */
    81,      /* columns */
    0x8000,  /* terminal buffer size */
    0x1000,  /* serial input buffer size */
    0x4000,  /* script memory size */
    "Monaco" /* pstring font name */
};

/* a ResEdit template for editing resources; the name of the TMPL resource
   should match the Macintosh resource type ID which takes the template */

type 'TMPL' {
  array {
    pstring;  // name of the field
    literal longint;  // type of the field

    // See ResEdit Reference, Chapter 5
    //
    // 'DBYT', 'DWRD', 'DLNG': the field is decimal byte/2-byte word/4-byte long
    // 'HBYT', 'HWRD', 'HLNG':              hex byte/word/long
    // 'AWRD', 'ALNG'        : align to a word, long boundary
    // 'FBYT','FWRD','FLNG'  : field is a zero-filled byte/word/long
    // 'HEXD'                : can only appear last, field is a hex dump of
    //                         remaining bytes
    // 'PSTR' : field is a Pascal string (first byte is length)
    // 'LSTR' : field is 'long' string (first 4 bytes are length)
    // 'WSTR' : field is a 'word'-length string (first 2-byte word is length)
    // 'ESTR' : pascal string padded to Even or Odd length (used in DITL)
    // 'CSTR' : C null-terminated string.
    // 'ECST', 'OCST': C null-terminated string, padded to Even or Odd length.
    // 'BOOL' : two-byte boolean
    // 'BBIT' : binary bit (must appear in multiples of 8)
    // 'TNAM' : 4-character OSType name.
    // 'CHAR' : single character
    // 'RECT' : 8 byte Quickdraw Rect
    // 'Hnnn' : nnn is hex number, shows that many bytes in hexadecimal.
    // 'Cnnn' : nnn is hex number, C string occupying nnn byte field.
    // 'P0nn' : nn is a hex number, Pascal-style string taking nn+1 (length)
    //          bytes.
    //
    // List sequences:
    // 'LSTZ'...'LSTE' 'List Zero', list terminated by a 0 byte, as in 'MENU'
    // 'ZCNT' 'LSTC'..'LSTE': zero count/list count-list end
    //                        list preceded by zero-based (N-1) word count
    //                        as in 'DITL'
    // 'OCNT' 'LSTC'..'LSTE': one count/list count-list end
    //                        list preceded by a 1-based word count,
    //                        as in STR#
    // 'LSTB'..'LSTE': List begin/list end: terminated by the end of the
    //                  resource (as in 'acur' and 'APPL')
  };
};

// ResEdit template for the 'CNFG' resource type defined above.
resource 'TMPL' (128, "CNFG") {
 {
  "Reserved", 'DWRD',
  "Font size", 'DWRD',
  "Lines", 'DWRD',
  "Colums" /* sic */, 'DWRD',
  "Terminal buffer size", 'DLNG',
  "Serial input buffer size", 'DLNG',
  "Reserved", 'DLNG',
  "Script memory size", 'DLNG',
  "Font name", 'PSTR'
 }
};

resource 'vers' (1, purgeable) {
 $02, $20, /* version 2.2.0 */
 final,    /* release stage */
 0,        /* non-final release # */
 verUS,    /* region code USA */
 "2.2",    /* short version string */
 "2.2 (17-Jan-1992)\rErny Tontlinger" /* long version string */
};

resource 'vers' (2, purgeable) {
  $02, $20, /* version 2.2.0 */
  final,    /* release stage */
  0,        /* non-final release # */
  verUS,    /* region code USA */
  "2.2",    /* short version string */
  "Serial communication program", /* long version string */
};
