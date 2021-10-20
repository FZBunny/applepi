#### A warning when building from source  
DO NOT use "qmake -project" to recreate the .pro file. (You will, however, need to do "qmake -makefile" to create a Makefile.)  The source code contains more than one definition of "main", four belonging to some small utilities.  Qmake can handle only a single main.  I'm still trying to find a clean way of integrationg these utilities into the project.   

#### Shortcomings and known bugs

- The 'help' files are incomplete, and out-of-date.  Updates coming soon.

- The 'Apple //e enhanced' emulation has several bugs and anomalies. No ideas when they might be resolved. 
- No joystick support yet.  Coming soon.
- The same character-set ROMS are used for all AppleII models. This results in older models anachronistic access to the enhanced character ROM.  I am also not certain that the character ROMS are otherwise accurate.  To be fixed eventually.
- Applepi does not always play well with the later versions of ProDOS utility disks.   
Later ProDOS utilities frequently crash to the monitor when attempting  to exit the program.
- Only later versions of the ProDOS uitlities can be used to format disk II floppies. They will warn that the drive is too fast, but will succeed. The earliest ProDOS utility will immediately claim the drive is too fast, and abandon the operation.
- There are four mildly annoying, though not always obvious, one-pixel-high horizontal lines in the display. Under investigation.
- ApplePi emulates a 65C02 (not the original 6502) processor, whether or not the IIe enhanced model is selected. This saves code, and few will care.  
- Cassette tape I/O is temporarily disabled. A workaround for the format differences between monitor and BASIC formats will eventually be devised.
- There is a printer in slot 1, BUT: no dialog to select output yet, just prints to console. Coming soon. Also does not print Applesoft code properly; keywords show only 1st character.  Integer BASIC prints properly, however.  Under investigation.
- There is no serial I/O emulation. Coming soon. (This can be partially mitigated by echoing screen text to the console or a file on the host O/S.)
