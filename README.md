# Classic Chip8 Instruction Set Interpreter --M. I.
An interpreter for the original Chip 8 Rom games  
To run a .ch8 rom, launch the executable and pass the path to the .ch8 rom as the only argument  
- path/to/exec path/to/rom.ch8  

The interpreter can be compiled with different backends for handling Rendering, Audio and Input, such as **SDL**, **SFML**, **Raylib**, *etc*.  
To change the backend, keep every definition the same, and only change the functionality of the backend's functions.  
Read backend.h and backend.c for instructions. Don't change any other files unless you know what you're doing.  
  
Credits:  
[Cowgod's Technical Chip8 Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)  
[NetPro2 Chip8 Roms](https://github.com/netpro2k/Chip8/tree/master/games)