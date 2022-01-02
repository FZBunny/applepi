
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
2) Added a bit of (gimmicky) code to automatically set and maintain the time & day when ProDOS is running.

#### V 0.1.8
- **floppy_controller.cpp** & **mainwindow.cpp**  
Added ability to recognize Apple Pascal disks and display Pascal volume names under the floppy disk buttons (Only tested on Pascal 1.3)

#### V 0.2
- **gamepad.cpp**  
Fixed gamepad code & removed mouse emulation of a gamepad.
- **printer.cpp**  
Added ability to print to PDF files.  'Print Shop' now works, but only one sheet at a time.  Banners are not yet possible.
- **help.cpp**  
Reworked method of displaying help (again).  Help has been brought up-to-date.
- **utilities directory**  
Moved source code for several small utility programs from src directory, and added a separate makefile for them.

#### V 0.2.1
- This release was simply a bug fix to the gamepad code.

#### V 0.2.2
- Added DiskII head sounds.
- Affected source files: applepi-qt.pro, resources.qrc, floppy\_controller.cpp, lin-speaker.cpp, mainwindow.cpp, mainwindow.h, version.h, volume\_dial.cpp