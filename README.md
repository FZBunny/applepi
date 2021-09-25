# ApplePi
"applepi" is an apple IIe emulator for the Raspberry Pi, but can be built (I think) on most Linux distributions.  
It runs easily on a Pi4B, and acceptably on a Pi3B. (The original Pi can't quite handle the load.)  
It also runs well on Ubuntu 20.04, the O/S used for development.  To build from source, you will need the development packages for Qt5 for your distro.

This is only version 0.1.0, so expect bugs.   You can report them to j.bruce.ward@gmail.com; be sure to describe what must be done to bring the bug out.

### .deb files:
As of 2021-09-24, there are two .deb package files in directory [binaries].  
# 2021-09-25 11:24 UTC / OOPS. Bad .deb files... one moment please 
- https://github.com/FZBunny/applepi/blob/main/binaries/applepi-0.1.0_armhf.deb   (32-bit Raspberry Pi OS)
- https://github.com/FZBunny/applepi/blob/main/binaries/applepi-0.1.0_amd64.deb   (64-bit Ubuntu 20.04)

Just right-click & "Save Link As..." to get a copy, then use: 
```sh
sudo apt install ./applepi-0.1.0_(whichever)
```
 to install.    (Note that the "./" is necessary.)

Not a lot else here yet, but here's a perty picture:
### Splash screen
![Screenshot of ApplePi](https://github.com/FZBunny/applepi/blob/main/images/Screenshot_2021-09-21_08-52-27.png)

[binaries]: <https://github.com/FZBunny/applepi/blob/main/binaries>
