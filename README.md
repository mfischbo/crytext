crytext
=======

Crytext is a small and simple text editor that automagically encrypts your text files before saving them to your hard disk. It uses standard cryptographic algorithms (AES / RSA) to achieve this. It is possible to save files in such a way that you can exchange them with others.

To keep away the hassle to deal with the security details crytext introduces a concept of "stickers" or maby better pictured as address labels. Each user using crytext has it's own private sticker that he can exchange with others. If Alice wants to send Bob a crytext file, she would only need to attach bob's sticker to the file, so he could open in.

## Features
* simplicity
* full encryption of your text files
* stickers allow you to share files with your friends
* sending textfiles and stickers with the built in e-mail client

Here's a screenshot of the (nearly) finished user interface
![ScreenShot](http://fischboeck.net/crytext/crytext-screener.png)

## Technology & Dependencies
The sofware is written in using tht QT Framework and currently in a very early stage.

- QT Framework >= 4.8
- crypto++ = >= 5.6.1
- libpoco >= 1.3.6


## Build from sources
I'm planning to build packages for Linux (Ubuntu), Windows and Mac and provide them here. As logn as those packages do not exist you can build from sources using qt-creator. 

### Ubuntu 14.04 (Trusty Thar)

```
  sudo apt-get install qtcreator libpoco-dev libcrypto++-dev
```
Then checkout the sources from here and add the project in qt creator.


More infos comming soon, as I finish the alpha release


