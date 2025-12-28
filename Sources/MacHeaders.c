/*
	Terminal 2.2
	"MacHeaders.c"

	To be precompiled into "MacHeaders"
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
#include <pascal.h>
#include <OSUtils.h>
#include <Resources.h>
#include <Scrap.h>
#include <Script.h>
#include <Serial.h>
#include <SetJmp.h>
#include <StandardFile.h>
#include <StdArg.h>
#include <String.h>
#include <SysEqu.h>
#include <ToolUtils.h>
#include <Types.h>
#include <Windows.h>

#define topLeft(r)		(((Point *) &(r))[0])
#define botRight(r)		(((Point *) &(r))[1])
#define QD(x)			(qd.x)

#define Time			(*(long *)0x20C)
#define Ticks			(*(long *)0x16A)
#define SCCRd			(*(Ptr *)0x1D8)
#define SFSaveDisk		(*(short *)0x214)
#define CurDirStore		(*(long *)0x398)
#define CurStackBase	(*(Ptr *)0x908)
#define GrayRgn			(*(RgnHandle *)0x9EE)
#define MemErr			(*(short *)0x220)
#define MBarHeight		(*(short *)0xBAA)

#define FALSE 0
#define TRUE 1
