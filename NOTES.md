# NOTES 2025-12-29

## The `CNFG` resource type

The `CNFG` resource type is specific to Terminal. The reason it is recognized
by ResEdit is because the Terminal resource fork also has a `TMPL` resource
which ResEdit reads and uses to parse the resource. See Chapter 5 of the
ResEdit Reference manual.

DWRD Reserved
DWRD Font size
DWRD Lines
DWRD Columns
DLNG Terminal buffer size
DLNG Serial input buffer size
DLNG Reserved
DLNG Script memory size
PSTR Font name

## The Default CDEF

Apparently, Terminal used a CDEF (control definition procedure) developed by
Lloyd Lim of Lim Unlimited, which was a 68k code resource which automatically
handled setting and correctly styling 'default' items in dialog and alert
windows.

See [About_Default](./Documenation/About_Default) for developer documentation.

Unfortunately, this is only available as the compiled `CDEF` resource, not
as source. (It also comes with a license requirement to provide a "free, fully
functional and fully supported copy of your software" to the apparently-defunct
firm of Lim Unlimited, which is possibly incompatible with the MIT license
used by this repostiory).

My plan is to update the dialog and alert handling code to do the manual work
of `default` control styling and handling, and drop the extra CDEF.

# 2025-12-30

## Headers

The ThinkC and MPW approach to headers was to use precompilation: the whole bag
of headers was included in MacHeaders{MPW}.c, and then `#include "MacHeaders"`
or `#pragma load ":(Objects):MacHeadersMPW"` was used.

For now, I am going to follow the principle of Include-What-You-Use and add
explicit includes for the headers needed by each file. That will likely slow
down Think C and MPW performance, but I'm not actually concerned about that.

Maybe I should use the opposite conditionals to skip my 'Retro68' includes?

## FormatStr/variable argument list

ThinkC 2-byte shorts causes serious-sounding warnings with FormatStr()/stdargs.h
usage in FormatStr.c

  In file included from .../Sources/FormatStr.c:35:
  .../Sources/FormatStr.c: In function ‘FormatStr’:
  .../Sources/FormatStr.c:140:64: warning: ‘short int’ is promoted to ‘int’
     when passed through ‘...’
          NumToString(va_arg(ap, short), num);
  note: (so you should pass ‘int’ not ‘short int’ to ‘va_arg’)
  note: if this code is reached, the program will abort

## Other int-size issues

I'm not sure how worried I should be about integer size issues and Retro68.
The code accomodates some Think C/MPW differences, maybe I should extend that
to Retro68?
