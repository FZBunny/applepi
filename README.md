    
# ApplePi
"applepi" is an apple IIe emulator for the Raspberry Pi, but can be built (I think) on most Linux distributions.  
It runs easily on a Pi4B, and acceptably on a Pi3B. (The original Pi can't quite handle the load.)  
It also runs well on Ubuntu 20.04, the O/S used for development.  To build from source, you will need the development packages for Qt5 for your distro.

This is only up to version 0.1.2, so expect bugs.   You can report them to jbward@bellaliant.net; be sure to describe what must be done to bring the bug out.

To get a copy, go to "Releases" on the right of this page, download a .deb file, then use: 
```sh
sudo apt install ./applepi-(whichever)
```
 to install.    (Note that the "./" is necessary.)   
 
- Of course, an Apple II of any kind isn't much use without a disk or two. To find disk-image files on the 'web, google "apple ii disk images".  For Apple disk masters, try https://www.apple.asimov.net/images/masters/;  for games, try http://virtualapple.org/. 

- When you do boot a floppy disk image, you will hear a series of 'tick' sounds.  These are a kind-of lame attempt to mimic the sound of head seeks as the drive moves from track to track. They will be replaced by proper recorded sounds someday.
- A note about games:  if a game fails to boot, put it in the 'Floppy 2' drive, and boot dos3. Then enter   ``` RUN HELLO,D2 ``` . If that fails (it won't), catalog disk 2 to find the game's file name: ``` CATALOG,D2 ``` ; then ``` BRUN ``` whatever the name is.   

- (The 'help' files are incomplete, and out-of-date.  Upates coming soon.)
  
  
Not a lot more here yet, but here's a perty picture:
### Splash screen
![Screenshot of ApplePi](https://github.com/FZBunny/applepi/blob/main/images/Screenshot_2021-09-21_08-52-27.png)


