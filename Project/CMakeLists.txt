cmake_minimum_required(VERSION 3.13)
project(Terminal C)

# Terminal application program compiled by Retro68 toolchain.
#
add_application(Terminal ../Sources/Terminal.r)

target_sources(Terminal PRIVATE
			../Sources/CancelDialog.h
			../Sources/CisB.h
			../Sources/CRC.h
			../Sources/Document.h
			../Sources/File.h
			../Sources/FormatStr.h
			../Sources/Interp.h
			../Sources/MacBinary.h
			../Sources/Macros.h
			../Sources/Main.h
			../Sources/Monitor.h
			../Sources/MySF.h
			../Sources/Options.h
			../Sources/Popup.h
			../Sources/Port.h
			../Sources/Procedure.h
			../Sources/Scroll.h
			../Sources/Strings.h
			../Sources/Text.h
			../Sources/Utilities.h
			../Sources/XModem.h
			../Sources/ZModem.h)
target_sources(Terminal PRIVATE
			../Sources/About.c
			../Sources/CancelDialog.c
			../Sources/CisB.c
			../Sources/CRC.c
			../Sources/Document.c
			../Sources/File.c
			../Sources/FormatStr.c
			../Sources/Init.c
			../Sources/Interp.c
			../Sources/MacBinary.c
			../Sources/Macros.c
			../Sources/Main.c
			../Sources/Monitor.c
			../Sources/MySF.c
			../Sources/OtherOptions.c
			../Sources/Popup.c
			../Sources/Port.c
			../Sources/Procedure.c
			../Sources/ProtocolOptions.c
			../Sources/Script.c
			../Sources/Scroll.c
			../Sources/SendOptions.c
			../Sources/TerminalOptions.c
			../Sources/Text.c
			../Sources/Utilities.c
			../Sources/XModem.c
			../Sources/XOptions.c
			../Sources/ZModem.c
			../Sources/ZOptions.c)
target_sources(Terminal PRIVATE	../Sources/Glue.a)

# Enable -ffunction-sections and -gc-sections to make the app as small as possible
set_target_properties(Terminal PROPERTIES COMPILE_OPTIONS -ffunction-sections)
set_target_properties(Terminal PROPERTIES LINK_FLAGS "-Wl,-gc-sections")
