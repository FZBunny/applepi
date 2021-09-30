#### A warning when building from source  
DO NOT use "qmake -project" to recreate the .pro file. The source code contains more than one definition of "main", three belonging to some small utilities.  Qmake can handle only a single main.  I'm still trying to find a clean way of integrationg these utilities into the project.  Using "qmake -makefile" is fine, but know that the current Makefile has a few manually added tweaks. (I really should go through the .pro syntax docs again.)  

#### A short list of shortcomings and known bugs

- The 'help' files are incomplete, and out-of-date.  Updates coming soon.
- When the 'Apple //e enhanced' ROM is selected, applepi will not boot from a hard drive or 800KiB floppy in slot 7 drive 1.   Instead, it will skip slot 7 and boot from slot 6.  Under investigation.  Workarounds: A) Boot from slot 6 and use: ```PR#7``` -or- B) Select the 'Apple //e' ROM instead)
- Applepi does not always play well with the later versions of ProDOS utility disks.  The utility programs will often ignore both "HD" drives. Under investigation. Nearest thing to a workaround is to use ProDOS 1.0 utilities.
- There are four mildly annoying horizontal lines in "Small Screen" mode. Under investigation.  Workaround: Use the "Big Screen" mode if your display will permit.
