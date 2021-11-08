
#### V 0.1.5   
- **disassemble\_memory_dialog.cpp**  
Added "fflush" to force all output to console when done.   
- **floppy_controller.cpp**  
Renamed logicalToPhysicalSector() to logicalSector()   
- **hd_controller.cpp**   
Fixed offsetKludge() to set zero offset if target bytes are not found in 1st block.   
Changed fn. headStepDelay to insert delay for hard drives as well as 800KB disks.
- **hd_dialog.cpp**  
Added ".hdv" to list of known suffixes.   
- **machine.cpp**   
Replaced fn. 'getProcessorState' with 'processorState'.  
- **mainwindow.cpp**    
Changed 'm_scaleButtonKludge' value to choose font size for "big screen/small screen" button to fit inside a fixed-size button.  (Font & size vary between distros; previous method changed button size.)
- **screen.cpp**   
(Re)added character-flash code to primary character set; this had somehow "got lost".
- **traptrace.cpp**   
Added missing hex validator calls to start and end text boxes.   
Changed start/stop trace button to class 'ApplepiButton'.

#### V 0.1.6
- **mainwindow.cpp**  
 Added a bit of (gimmicky) code to update date & time once/second when running ProDOS.
- **hd_controller.cpp**   
Added code to make the HD/3.5" floppy controller obey soft switch settings. (Previously only read and wrote main RAM. (\*blush\*)  Also fixed typo which prevented "Total Replay" from running.
- **mainwindow_menus.cpp**   
Added Ctrl-V shortcut paste-to-keyboard operation. (Ctrl-C must be reserved for 'abort'.)
- **rom_dialog.cpp**   
Fixed bug which would cause a segfault.

#### V 0.1.7
- **printer.cpp**  
Added ability to print, using PR#1, to either a disk file or console.
- **screen.cpp**  
Fixed (i.e. removed) anomalous horizontal lines in screen display.
- **gamepad.cpp**  
Added basic gamepad support, plus ability to use mouse input in place of a gamepad if none is available.
- **mainwindow.cpp**  
1) Fixed a bug preventing the left & right 'Alt' keys to mimic the open- & closed-apple keys on the Apple II keyboard.  
2) Added a bit of (gimmicky) code to automatically set and maintain the time & day when ProDOS is running