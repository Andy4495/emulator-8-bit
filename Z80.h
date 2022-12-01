#ifndef Z80_H
#define Z80_H

/* Z80 Core Definitions */
#define MAX_MEMORY 65536
#define MAX_IO 256

unsigned char memory[MAX_MEMORY] = {0};

unsigned char io[MAX_IO] = {0};

class Z80 {
    public:
        // Main register set
        // Accumulator 
        unsigned char A;
        // Flags
        unsigned char F;
        // General purpose registers
        unsigned char B, C, D, E, H, L;

        // Alernate register set
        // Accumulator 
        unsigned char Aprime;
        // Flags
        unsigned char Fprime;
        // General purpose registers
        unsigned char Bprime, Cprime, Dprime, Eprime, Hprime, Lprime;

        // Special purpose registers
        // Interrupt vector
        unsigned int I;
        // Memory refresh register
        unsigned int R;
        // Index registers
        unsigned short IX, IY;
        // Stack pointer
        unsigned short SP;
        // Program counter
        unsigned short PC;

    private:

};
#endif