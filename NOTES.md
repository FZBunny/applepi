#### A warning when building from source  
DO NOT use "qmake -project" to recreate the .pro file. The source code contains more than one definition of "main", four belonging to some small utilities.  Qmake can handle only a single main.  I'm still trying to find a clean way of integrationg these utilities into the project.  Using "qmake -makefile" is fine, but know that the current Makefile may have a few manually added tweaks. (I really should go through the .pro syntax docs again.)  

#### Shortcomings and known bugs

- The 'help' files are incomplete, and out-of-date.  Updates coming soon.
- During installation, ApplePi does not yet create an entry in the desktop applications menu. To be fixed soon. It does, however, volunteer to add a desktop file as the last step in installation. 
- When the 'Apple //e enhanced' ROM is selected, applepi will not boot from a hard drive or 800KiB floppy in slot 7 drive 1.   Instead, it will skip slot 7 and boot from slot 6.  Under investigation.  Workarounds: A) Boot from slot 6 and use: ```PR#7``` -or- B) Select the 'Apple //e' ROM instead. Use "File->Select AppleII Model..." to do that.
- Applepi does not always play well with the later versions of ProDOS utility disks.  The utility programs will often ignore both "HD" drives. Under investigation. Nearest thing to a workaround is to use ProDOS 1.0 utilities. This is true only of the utility programs themselves; BASIC works fine on all versions of ProDOS.
- There are four mildly annoying, though not always obvious, one-pixel-high horizontal lines in the display.  Under investigation.
- ApplePi emulates a 65C02 (not the original 6502) processor, whether or not the IIe enhanced model is selected. This saves code, and few will care.  
- Cassette tape I/O is temporarily disabled. A workaround for the format differences between monitor and BASIC formats is being devised.
