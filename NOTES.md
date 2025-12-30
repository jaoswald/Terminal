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
