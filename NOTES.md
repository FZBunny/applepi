#### To build from source:    
For Debian-derived distributions, you will need to install these libraries:   

- qt5-qmake       (Not a lib, but a utility needed to create a Makefile)
- libasound2-dev
- libpulse-dev   
- qtdeclarative5-dev
- libqt5gamepad5-dev
- qtmultimedia5-dev


#### A warning when building from source  
ApplePi uses the Qt5 library.
You will have to do a 'qmake -makefile applepi-lnx.pro' to create a makefile.

-DO NOT- use "qmake -project" to recreate the .pro file.  
However, if you must, add the following lines to the new applepi.pro :  

`QMAKE_CXXFLAGS += -Wno-implicit-fallthrough -Wno-deprecated-declarations`   
`LIBS += -L/usr/lib/x86_64-linux-gnu/alsa-lib -lasound -no-pie`   
`QT += widgets   printsupport   gamepad  multimedia`  
`OBJECTS_DIR=obj`  
`DESTDIR=bin`   

You will also have to remove tape.cpp, tape\_dialog.cpp, tape.h, and tape_dialog.h from the source and header lists of the new applepi.pro .  (All 'tape' code is old, using wxWidgets.  It may be converted to Qt some time in future)

  
#### Shortcomings and known bugs

- Using `PR#3` to display 80-column text does not work for the original AppleII or the AppleII+ models.  To be fixed soon.
- There is some confusion about which ROM is the IIe and which is the enhanced IIe ROM. Selecting the IIe model may really select the enhanced IIe. Or not... 
- ApplePi cannot read 13-sector floppy disks; that is, DOS versions before 3.3; this will probably not change.
- There is a bug when double-buffering double-hires graphics. To be fixed soon(ish)
- The Edit->"Copy Screen Text to Clipboard" menu selection fails to copy 80-column text.  Fixed in source code, but not in release code yet. (As of 2023-06-11)
- Only later versions of the ProDOS uitlities can be used to format disk II floppies. They will warn that the drive is too fast, but will succeed. The earliest ProDOS utility will immediately claim the drive is too fast, and abandon the operation.
- ApplePi emulates a 65C02 (not the original 6502) processor, whether or not the IIe enhanced model is selected. This saves code, and few will care.
- Cassette tape I/O is temporarily disabled. A workaround for the format differences between monitor and BASIC formats will eventually be devised.
- There is no serial I/O emulation. Coming soon. (This can be partially mitigated by echoing screen text to the console or a file on the host O/S.)

