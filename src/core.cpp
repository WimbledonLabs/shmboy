void Core::reset() {
    mem[P1] = 0;
    mem[SC] = 0;
    mem[TIMA] = 0;
    mem[TAC] = 0;
    mem[IE] = 0;
    mem[LCDC] = 0 //TODO actual value: $83 BG/OBJ ON, LCDC OPERATION;
    mem[SCY] = 0;
    mem[SCX] = 0;
    mem[LYC] = 0;
    mem[WY] = 0;
    mem[W] = 0;
    mem[IE] = 0;// DI; //TODO DI = disable interrupt instruction

    mem[IF] = 0; // needed? - SHM

    if (mode == CGB_MODE) {
        mem[VBK] = 0;
    }
}

Core::Core() {

}
