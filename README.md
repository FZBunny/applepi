# applepi
"applepi" is an apple IIe emulator for the Raspberry Pi, but can be built (I think) on any debian-derived Linux distro.
At any rate, it is known to build and run well on PiOS and Ubuntu 20.04.

This is only version 0.1.0, so expect bugs.   You can report them to j.bruce.ward@gmail.com; be sure to describe what must be done to bring the bug out.

### .deb files:
As of 2021-09-24, there are two .deb package files in directory [binaries], one each for PiOS & .

- https://github.com/FZBunny/applepi/blob/main/binaries/applepi-0.1.0_armhf.deb   (32-bit Raspberry Pi OS)
- https://github.com/FZBunny/applepi/blob/main/binaries/applepi-0.1.0_amd64.deb   (64-bit Ubuntu 20.04)

Just right-click & "Save Link As..." to get a copy, and then (as root) use: 
```sh
apt install ./applepi-0.1.0_(whichever)
```
 to install.    (Note that the "./" is necessary.)

Not a lot else here yet, but here's a perty picture:
### Splash screen
![Screenshot of ApplePi](https://github.com/FZBunny/applepi/blob/main/images/Screenshot_2021-09-21_08-52-27.png)

[binaries]: <https://github.com/FZBunny/applepi/blob/main/binaries>
