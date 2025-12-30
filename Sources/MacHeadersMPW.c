/*
	Terminal 2.0
	"MacHeadersMPW.c"

	Compile this file, it will create "MacHeadersMPW"
	containing the precompiled headers needed
*/


#include <Controls.h>
#include <CType.h>
#include <Desk.h>
#include <Devices.h>
#include <Dialogs.h>
#include <DiskInit.h>
#include <Errors.h>
#include <Files.h>
#include <Fonts.h>
#include <Memory.h>
#include <Menus.h>
#include <Packages.h>
#include <OSUtils.h>
#include <Resources.h>
#include <Scrap.h>
#include <Script.h>
#include <Serial.h>
#include <SetJmp.h>
#include <StandardFile.h>
#include <StdArg.h>
#include <String.h>
#include <Strings.h>
#include <SysEqu.h>
#include <ToolUtils.h>
#include <Types.h>
#include <Windows.h>

#include "Util.h"

/* Macros not found in MPW, but used by THINK C */

/* typedef unsigned char Byte; */	/* Is now defined in "Types.h" of MPW 3.2 */

#define CtoPstr		c2pstr
#define PtoCstr		p2cstr

/*
	Attention:
	
	¥ int		4 bytes in MPW and 2 bytes in THINK C
	¥ \r		Line feed in MPW, carriage return in THINK C
	¥ \n		Carriage return in MPW, line feed in THINK C
	¥ char		4 bytes on stack if passed to a function
	¥ short		4 bytes on stack if passed to a function
*/

/* Precompile all definitions */

#pragma dump ":(Objects):MacHeadersMPW"
