# SHMBoy - A Game Boy Color Emulator
A Game Boy Color emulator written by Stephen Molyneaux. Currently in development.

Uses Dear Imgui by Omar Cornut for the debug output.

## Why make another Game Boy emulator?
The idea of creating an emulator for a device I used and enjoyed as a kid enthralled me. When I realized it was reasonably within my ability to create I couldn't NOT work on it. The ultimate goal is to completely play through Pokémon Red using the emulator I created. The majority of the journey still lies ahead

## TODO
Lots of things still need to be done to make this emulator remotely function:
- Verify instruction implementation
- Implement PPU (pixel processing unit)
- Cartridge Emulation
- Debugger development
    - ~~Register Viewer (A,B,C,D,E,F,H,L,PC,SP)~~
    - ~~Instruction Disassembly~~
    - Single stepping
    - Breakpoints
    - Sound waveform visualizer
    - Memory Viewer
        - Memory editing?
    - Graphics
        - Palette Viewer
            - Shows colour
            - Shows colour code on hover
        - Object/BG Tile Viewer
            - Shows OBJ number on hover
    - Stack display
    - Button status

## Novel Emulator Implementation Details (TODO)


### Instruction Decoding
Instructions decoded using a nested switch statement which was [generated using an decoder generator I wrote](https://github.com/WimbledonLabs/opcode-decoder-generator). For 8-bit instructions this implementation is somewhat inefficient. Common ways to perform instruction decoding are using a single 256-way switch statement, or an array of 256 function pointers. These alternatives have the disadvantage of scaling exponentially with increasing instruction size, and require quite a bit of code duplication.

One of the main benefits of generating instruction decoders is that they can be used to create a disassembler quite easily. This helps tremendously for debugging homebrew games, and the emulator itself.

### Interacting with Hardware Through Memory
TODO implement this and explain it

## Game Boy Emulator Development Resources
Here's a list of Game Boy documentation that I've compiled for my own reference. Hopefully other emulator developers find this collection helpful as well:
- [Official Nintendo Game Boy Programming Manual](http://www.chrisantonellis.com/files/gameboy/gb-programming-manual.pdf)
- [Excellent Opcode Summary Page](http://goldencrystal.free.fr/GBZ80Opcodes.pdf)
- [Game Boy CPU Manual](http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf)
- [Game Boy CPU Instruction Set Matrix](http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html)
- [Pan Docs - Comprehensive Technical Guide](http://bgb.bircd.org/pandocs.htm)
- [Cycle Accurate Game Boy Docs](https://github.com/AntonioND/giibiiadvance/blob/master/docs/TCAGBD.pdf)
- [Simple CPU Instruction Descriptions](http://www.chrisantonellis.com/files/gameboy/gb-instructions.txt)
- [Game Boy Development Resource List](https://github.com/avivace/awesome-gbdev). Comprehensive list of gameboy development resources, but less of a focus on emulator development topics. Most of the items above are repeated in this list.
