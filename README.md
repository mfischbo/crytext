crytext
=======

Crytext is a small and simple text editor that automagically encrypts your text files when you save them to your hard disk. It uses stnadard cryptographic algorithms (AES / RSA) to achieve this. It is possible to save files in such a way that you can exchange them with others.

To keep away the hassle to deal with the security details crytext introduces a concept of "stickers" or maby better pictured as address labels. Each user using crytext has it's own private sticker that he can exchange with others. If Alice wants to send Bob a crytext file, she would only need to attach bob's sticker to the file, so he could open in.

## Technology & Dependencies
The sofware is written in using tht QT Framework and currently in a very early stage.

- QT Framework >= 4.8
- crypto++ = >= 5.6.1
- libpoco >= 1.3.6


More infos comming soon, as I finish the alpha release
