
#####  - Go to "Releases" on the right for installable files. Pi users want the "..armhf.deb" file. -
![Screenshot of ApplePi](https://github.com/FZBunny/applepi/blob/main/images/Screenshot_2021-10-20_10-40-48.png)

"ApplePi" is an apple IIe emulator for the Raspberry Pi, but can be built on most Linux distributions. (See "NOTES.md" for a build warning.)
It runs easily on a Pi4B, and acceptably on a Pi3B; the original Pi can't quite handle the load.  
It also runs well on Ubuntu 20.04, the O/S used for development.  To build from source, you will need the development packages for Qt5 for your distro.

This is only up to version 0.1.8, so expect bugs.   You can report them to jbward@bellaliant.net; be sure to describe what must be done to bring the bug out.

To get a copy, go to "Releases" on the right of this page, download a .deb file, then use: 

``` sudo apt install ./applepi-(whichever)```
##### (Note that the "./" is necessary.)   
 
- Of course, an Apple II of any kind isn't much use without a disk or two. To find disk-image files on the 'web, google "apple ii disk images".  For Apple disk masters, try https://www.apple.asimov.net/images/masters/;  for games, go to https://archive.org/details/TotalReplay for an amazing 32MB hard drive full of of HUNDREDS of games.  Also a good source: http://virtualapple.org/. 

- When you do boot a floppy disk image, you will hear a series of 'tick' sounds.  These are a kind-of lame attempt to mimic the sound of head seeks as the drive moves from track to track. They will be replaced by proper recorded sounds someday.
- A note about games:  if a game floppy fails to boot, put it in the 'Floppy 2' drive, and boot dos3. Then enter   ``` RUN HELLO,D2 ``` . If that fails (it won't), catalog disk 2 to find the game's file name: ``` CATALOG,D2 ``` ; then ``` BRUN ``` whatever the name is.   
- See "NOTES.md" for a (short!) list of known bugs and shortcomings.

 
