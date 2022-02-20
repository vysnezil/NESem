#pragma once
#include <cstdint>
#include "Logger.h"

class Bus;
class CPU
{
    struct INSTRUCTION {
        const char* name = "";
        void (CPU::* function)(void) = nullptr;
        uint16_t (CPU::* mode)(void) = nullptr;
        uint8_t cycles = 0;
    };

    enum flag {
        C = 1 << 0,
        Z = 1 << 1,
        I = 1 << 2,
        D = 1 << 3,
        B = 1 << 4,
        U = 1 << 5,
        V = 1 << 6,
        N = 1 << 7
    };

public:
	void clock();
	void reset();

    void setBus(Bus* bus);
    CPU();
    void NMI();
    //DEBUG
    void jump(uint16_t address);

private:
    //Internal registers
    uint8_t x, y, a, sp, status;
    uint16_t pc;

    Bus* bus;
    uint16_t address;
    uint8_t op;

    uint8_t cycles;
    INSTRUCTION instructionTable[0xFF];

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);
    
    //Helper functions
    void branch(CPU::flag flag, bool status);
    void setFlag(flag flag, bool value);
    void updateFlags(uint8_t val);
    bool getFlag(flag flag);
    void addInstructions();
    uint16_t getAddress();
    

    //Adddressing modes
    uint16_t IMP(), IMM(), ZP0(), ZPX(), ZPY(), ABS(), ABX(), ABY(), IND(), IZX(), IZY(), REL();

    //Load and Store instructions
    void LDA(), LDX(), LDY(), STA(), STX(), STY();

    //Aritmetic instructions
    void SBC(), ADC();

    //Increment and Decrement instructions
    void DEC(), DEX(), DEY(), INC(), INX(), INY();

    //Shift and Rotate instructions
    void ASL(), LSR(), ROL(), ROR();

    //Logic instructions
    void AND(), ORA(), EOR();

    //Compare and Test Bit instructions
    void CMP(), CPX(), CPY(), BIT();

    //Branch instructions
    void BCC(), BCS(), BNE(), BEQ(), BPL(), BMI(), BVC(), BVS();

    //Transfer instructions
    void TAX(), TXA(), TAY(), TYA(), TSX(), TXS();

    //Stack instructions
    void PHA(), PLA(), PHP(), PLP();

    //Subroutines and Jump instructions
    void JMP(), JSR(), RTS(), RTI();

    //Set and Clear instructions
    void CLC(), CLD(), CLI(), CLV(), SEC(), SED(), SEI();

    //Miscellaneous
    void NOP(), BRK(), XXX();
};
