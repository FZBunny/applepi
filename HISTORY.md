
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

