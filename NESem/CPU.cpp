#include "CPU.h"
#include "Bus.h"

CPU::CPU()
{
    this->addInstructions();
}

void CPU::setBus(Bus* bus) {
	this->bus = bus;
}

void CPU::write(uint16_t address, uint8_t data)
{
	bus->write(address, data);
}

uint8_t CPU::read(uint16_t address) {
	return bus->read(address);
}

void CPU::reset() {
	x = y = a = sp = status = 0;
	setFlag(U, 1);
	sp = 0x00FF;
	pc = read(0xFFFC) | (read(0xFFFD) << 8);
}

void CPU::clock() {
	if (!cycles) {
        setFlag(U, 1);
        logger.pc = pc;
		uint8_t op = read(pc++);
		cycles = instructionTable[op].cycles;
		address = (this->*instructionTable[op].mode)();
		(this->*instructionTable[op].function)();
        logger.logInstruction(op, instructionTable[op].name, address, x, y, a, sp, status);
	}
	cycles--;
}

void CPU::jump(uint16_t address)
{
    pc = address;
}

void CPU::setFlag(flag flag, bool value) {
	if (value) status |= flag;
	else status &= ~flag;
}

void CPU::updateFlags(uint8_t val) {
	setFlag(Z, !(val & 0x00FF));
	setFlag(N, val & 0x80);
}

bool CPU::getFlag(flag flag) {
	return status & flag;
}

uint16_t CPU::IMP()
{
    return a;
}

uint16_t CPU::IMM()
{
    return pc++;
}

uint16_t CPU::ZP0()
{
    return 0x00FF & read(pc++);
}

uint16_t CPU::ZPX()
{
    return (read(pc++) + x) & 0x00FF;
}

uint16_t CPU::ZPY()
{
    return (read(pc++) + y) & 0x00FF;
}

uint16_t CPU::ABS()
{
    return  read(pc++) | (read(pc++) << 8);
}

uint16_t CPU::ABX()
{
    uint8_t lo = read(pc++);
    uint8_t hi = read(pc++);
    uint16_t address = ((hi << 8) | lo) + x;
    if ((address & 0xFF00) == (hi << 8)) cycles++;
    return address;
}

uint16_t CPU::ABY()
{
    uint8_t lo = read(pc++);
    uint8_t hi = read(pc++);
    uint16_t address = ((hi << 8) | lo) + y;
    if ((address & 0xFF00) == (hi << 8)) cycles++;
    return address;
}

uint16_t CPU::IND()
{
    uint16_t ptr = (read(pc++) << 8) | read(pc++);
    //page boundary hardware bug
    if ((ptr & 0x00FF) == 0x00FF) return (read(ptr & 0xFF00) << 8) | read(ptr);
    else return (read(ptr + 1) << 8) | read(ptr);
}

uint16_t CPU::IZX()
{
    uint8_t ptr = read(pc++) + x;
    return read(ptr) & 0x00FF + (read(ptr + 1) << 8) & 0x00FF;
}

uint16_t CPU::IZY()
{
    uint8_t tmp = read(pc++);
    uint8_t lo = read(tmp & 0x00FF);
    uint8_t hi = read((tmp + 1) & 0x00FF);
    return (lo | (hi << 8)) +y;
}

uint16_t CPU::REL()
{
    //return pc + (int8_t)read(pc++);
    uint16_t addr = read(pc);
    pc++;
    if (addr & 0x80) addr |= 0xFF00;
    return addr;
}

void CPU::LDA()
{
    a = read(address);
    updateFlags(a);
}

void CPU::LDX()
{
    x = read(address);
    updateFlags(x);
}

void CPU::LDY()
{
    y = read(address);
    updateFlags(y);
}

void CPU::XXX()
{
    //TODO: logger, remove this instrunction
}

void CPU::JMP()
{
    pc = address;
}

void CPU::NOP()
{

}

void CPU::INC()
{
    uint8_t tmp = read(address);
    write(address, ++tmp);
    updateFlags(tmp);
}

void CPU::INX()
{
    x++;
    updateFlags(x);
}

void CPU::INY()
{
    y++;
    updateFlags(y);
}

void CPU::JSR()
{
    pc--;
    write(sp + 0x100, (pc >> 8) & 0x00FF);
    sp--;
    write(sp + 0x100, pc & 0x00FF);
    sp--;
    pc = address;
}

void CPU::RTS()
{
    sp++;
    pc = (uint16_t)read(sp + 0x100);
    sp++;
    pc |= (uint16_t)read(sp + 0x100) << 8;
    pc++;
}

void CPU::RTI()
{
    sp++;
    status = (uint16_t)read(sp + 0x100);
    status &= ~B;
    status &= ~U;
    sp++;
    pc = (uint16_t)read(sp + 0x100);
    sp++;
    pc |= (uint16_t)read(sp + 0x100) << 8;
}

void CPU::ADC()
{
    uint16_t temp = (uint16_t)a + (uint16_t)read(address) + (uint16_t)getFlag(C);
    setFlag(C, temp > 255);
    setFlag(Z, (temp & 0x00FF) == 0);

    //
    setFlag(V, (~((uint16_t)a ^ (uint16_t)read(address)) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

    setFlag(N, temp & 0x80);
    a = temp & 0x00FF;
}

void CPU::SBC()
{
    uint16_t value = ((uint16_t)read(address)) ^ 0x00FF;
    uint16_t temp = (uint16_t)a + value + (uint16_t)getFlag(C);
    setFlag(C, temp & 0xFF00);
    setFlag(Z, (temp & 0x00FF) == 0);
    setFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
    setFlag(N, temp & 0x0080);
    a = temp & 0x00FF;
}

void CPU::SEC()
{
    setFlag(C, 1);
}

void CPU::SED()
{
    setFlag(D, 1);
}

void CPU::SEI()
{
    setFlag(I, 1);
}

void CPU::STA()
{
    write(address, a);
}

void CPU::STX()
{
    write(address, x);
}

void CPU::STY()
{
    write(address, y);
}

void CPU::branch(CPU::flag flag, bool status) {
    if (getFlag(flag) == status)
    {
        cycles++;
        address = pc + address;

        if ((address & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = address;
    }
}

void CPU::BCC()
{
    branch(C, 0);
}

void CPU::BCS()
{
    branch(C, 1);
}

void CPU::BNE()
{
    branch(Z, 0);
}

void CPU::BEQ()
{
    branch(Z, 1);
}

void CPU::BPL()
{
    branch(N, 0);
}

void CPU::BMI()
{
    branch(N, 1);
}

void CPU::BVC()
{
    branch(V, 0);
}

void CPU::BVS()
{
    branch(V, 1);
}

void CPU::CPX()
{
    uint16_t temp = (uint16_t)x - (uint16_t)read(address);
    setFlag(C, x >= read(address));
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
}

void CPU::CPY()
{
    uint16_t temp = (uint16_t)y - (uint16_t)read(address);
    setFlag(C, y >= read(address));
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
}

void CPU::DEC()
{
    uint8_t tmp = read(address);
    write(address, --tmp);
    updateFlags(tmp);
}

void CPU::DEX()
{
    x--;
    updateFlags(x);
}

void CPU::DEY()
{
    y--;
    updateFlags(y);
}

void CPU::PHA()
{
    write(sp + 0x100, a);
    sp--;
}

void CPU::PLA()
{
    sp++;
    a = read(sp + 0x100);
    updateFlags(a);
}

void CPU::PHP()
{
    write(sp + 0x100, status);
    sp--;
}

void CPU::PLP()
{
    sp++;
    status = read(sp + 0x100);
}

void CPU::TAX()
{
    x = a;
    updateFlags(x);
}

void CPU::TXA()
{
    a = x;
    updateFlags(a);
}

void CPU::TAY()
{
    y = a;
    updateFlags(y);
}

void CPU::TYA()
{
    a = y;
    updateFlags(a);
}

void CPU::TSX()
{
    x = sp;
    updateFlags(x);
}

void CPU::TXS()
{
    sp = x;
}

void CPU::ASL()
{
    uint16_t temp = (uint16_t)read(address) << 1;
    setFlag(C, (temp & 0xFF00) > 0);
    updateFlags(temp);
    if (instructionTable[op].mode == &CPU::IMP)
        a = temp & 0x00FF;
    else
        write(address, temp & 0x00FF);
}

void CPU::LSR()
{
    setFlag(C, (uint16_t)(read(address) & 0xFF00) > 0);
    uint16_t temp = (uint16_t)read(address) >> 1;
    updateFlags(temp);
    if (instructionTable[op].mode == &CPU::IMP)
        a = temp & 0x00FF;
    else
        write(address, temp & 0x00FF);
}

void CPU::ROL()
{
    uint8_t temp = (uint16_t)(read(address) << 1) | (uint16_t)getFlag(C);
    setFlag(C, temp & 0xFF00);
    updateFlags(temp);
    if (instructionTable[op].mode == &CPU::IMP)
        a = temp & 0x00FF;
    else
        write(address, temp & 0x00FF);
}

void CPU::ROR()
{
    uint8_t temp = (uint16_t)(getFlag(C) << 7) | (read(address) >> 1);
    setFlag(C, read(address) & 0x01);
    updateFlags(temp);
    if (instructionTable[op].mode == &CPU::IMP)
        a = temp & 0x00FF;
    else
        write(address, temp & 0x00FF);
}

void CPU::AND()
{
    a &= read(address);
    updateFlags(a);
    cycles++;
}

void CPU::ORA()
{
    a |= read(address);
    updateFlags(a);
    cycles++;
}

void CPU::EOR()
{
    a ^= read(address);
    updateFlags(a);
    cycles++;
}

void CPU::BIT()
{
    uint8_t tmp = a & read(address);
    setFlag(Z, (tmp & 0x00FF) == 0x00);
    setFlag(N, read(address) & (1 << 7));
    setFlag(V, read(address) & (1 << 6));
}

void CPU::BRK()
{
    pc++;
    setFlag(I, 1);
    write(sp + 0x100, (pc >> 8) & 0x00FF);
    sp--;
    write(sp + 0x100, pc & 0x00FF);
    sp--;

    setFlag(B, 1);
    write(sp + 0x100, status);
    sp--;
    setFlag(B, 0);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
}

void CPU::CLC()
{
    setFlag(C, 0);
}

void CPU::CLD()
{
    setFlag(D, 0);
}

void CPU::CLI()
{
    setFlag(I, 0);
}

void CPU::CLV()
{
    setFlag(V, 0);
}

void CPU::CMP()
{
    uint16_t tmp = (uint16_t)a - (uint16_t)read(address);
    setFlag(C, a >= read(address));
    updateFlags(tmp);
    cycles++;
}

void CPU::addInstructions()
{
#define addInstruction(opcode, name, functionP, addressMode, numCycles) instructionTable[opcode] = { name, &CPU::functionP, &CPU::addressMode, numCycles }

    addInstruction(0xA9, "LDA", LDA, IMM, 2);
    addInstruction(0xA5, "LDA", LDA, ZP0, 3);
    addInstruction(0xB5, "LDA", LDA, ZPX, 4);
    addInstruction(0xAD, "LDA", LDA, ABS, 4);
    addInstruction(0xBD, "LDA", LDA, ABX, 4);
    addInstruction(0xB9, "LDA", LDA, ABY, 4);
    addInstruction(0xA1, "LDA", LDA, IZX, 6);
    addInstruction(0xB1, "LDA", LDA, IZY, 5);

    addInstruction(0xA2, "LDX", LDX, IMM, 2);
    addInstruction(0xA6, "LDX", LDX, ZP0, 3);
    addInstruction(0xB6, "LDX", LDX, ZPY, 4);
    addInstruction(0xAE, "LDX", LDX, ABS, 4);
    addInstruction(0xBE, "LDX", LDX, ABY, 4);

    addInstruction(0xA0, "LDY", LDY, IMM, 2);
    addInstruction(0xA4, "LDY", LDY, ZP0, 3);
    addInstruction(0xB4, "LDY", LDY, ZPX, 4);
    addInstruction(0xAC, "LDY", LDY, ABS, 4);
    addInstruction(0xBC, "LDY", LDY, ABX, 4);

    addInstruction(0x4C, "JMP", JMP, ABS, 3);
    addInstruction(0x6C, "JMP", JMP, IND, 5);

    addInstruction(0xEA, "NOP", NOP, IMP, 2);
    addInstruction(0x01, "BRK", BRK, IMP, 7);

    addInstruction(0x24, "BIT", BIT, ZP0, 3);
    addInstruction(0x2C, "BIT", BIT, ABS, 4);

    addInstruction(0xD0, "BNE", BNE, REL, 2);
    addInstruction(0xF0, "BEQ", BEQ, REL, 2);
    addInstruction(0x90, "BCC", BCC, REL, 2);
    addInstruction(0xB0, "BCS", BCS, REL, 2);
    addInstruction(0x10, "BPL", BPL, REL, 2);
    addInstruction(0x30, "BMI", BMI, REL, 2);
    addInstruction(0x50, "BVC", BVC, REL, 2);
    addInstruction(0x70, "BVS", BVS, REL, 2);

    addInstruction(0x48, "PHA", PHA, IMP, 3);
    addInstruction(0x68, "PLA", PLA, IMP, 4);
    addInstruction(0x08, "PHP", PHP, IMP, 3);
    addInstruction(0x28, "PLP", PLP, IMP, 4);

    addInstruction(0xAA, "TAX", TAX, IMP, 2);
    addInstruction(0x8A, "TXA", TXA, IMP, 2);
    addInstruction(0xA8, "TAY", TAY, IMP, 2);
    addInstruction(0x98, "TYA", TYA, IMP, 2);
    addInstruction(0xBA, "TSX", TSX, IMP, 2);
    addInstruction(0x9A, "TXS", TXS, IMP, 2);

    addInstruction(0x0E, "ASL", ASL, ABS, 6);
    addInstruction(0x1E, "ASL", ASL, ABX, 7);
    addInstruction(0x0A, "ASL", ASL, IMP, 2);
    addInstruction(0x06, "ASL", ASL, ZP0, 5);
    addInstruction(0x16, "ASL", ASL, ZPX, 6);

    addInstruction(0x4E, "LSR", LSR, ABS, 6);
    addInstruction(0x5E, "LSR", LSR, ABX, 7);
    addInstruction(0x4A, "LSR", LSR, IMP, 2);
    addInstruction(0x46, "LSR", LSR, ZP0, 5);
    addInstruction(0x56, "LSR", LSR, ZPX, 6);

    addInstruction(0x2E, "ROL", ROL, ABS, 6);
    addInstruction(0x3E, "ROL", ROL, ABX, 7);
    addInstruction(0x2A, "ROL", ROL, IMP, 2);
    addInstruction(0x26, "ROL", ROL, ZP0, 5);
    addInstruction(0x36, "ROL", ROL, ZPX, 6);

    addInstruction(0x6E, "ROR", ROR, ABS, 6);
    addInstruction(0x7E, "ROR", ROR, ABX, 7);
    addInstruction(0x6A, "ROR", ROR, IMP, 2);
    addInstruction(0x66, "ROR", ROR, ZP0, 5);
    addInstruction(0x76, "ROR", ROR, ZPX, 6);

    addInstruction(0x2D, "AND", AND, ABS, 4);
    addInstruction(0x3D, "AND", AND, ABX, 4);
    addInstruction(0x39, "AND", AND, ABY, 4);
    addInstruction(0x29, "AND", AND, IMM, 2);
    addInstruction(0x25, "AND", AND, ZP0, 3);
    addInstruction(0x21, "AND", AND, IZX, 6);
    addInstruction(0x35, "AND", AND, ZPX, 4);
    addInstruction(0x31, "AND", AND, IZY, 5);

    addInstruction(0x69, "ADC", ADC, IMM, 2);
    addInstruction(0x65, "ADC", ADC, ZP0, 3);
    addInstruction(0x75, "ADC", ADC, ZPX, 4);
    addInstruction(0x6D, "ADC", ADC, ABS, 4);
    addInstruction(0x7D, "ADC", ADC, ABX, 4);
    addInstruction(0x79, "ADC", ADC, ABY, 4);
    addInstruction(0x61, "ADC", ADC, IZX, 6);
    addInstruction(0x71, "ADC", ADC, IZY, 5);

    addInstruction(0x0D, "ORA", ORA, ABS, 4);
    addInstruction(0x1D, "ORA", ORA, ABX, 4);
    addInstruction(0x19, "ORA", ORA, ABY, 4);
    addInstruction(0x09, "ORA", ORA, IMM, 2);
    addInstruction(0x05, "ORA", ORA, ZP0, 3);
    addInstruction(0x01, "ORA", ORA, IZX, 6);
    addInstruction(0x15, "ORA", ORA, ZPX, 4);
    addInstruction(0x11, "ORA", ORA, IZY, 5);

    addInstruction(0x4D, "EOR", EOR, ABS, 4);
    addInstruction(0x5D, "EOR", EOR, ABX, 4);
    addInstruction(0x59, "EOR", EOR, ABY, 4);
    addInstruction(0x49, "EOR", EOR, IMM, 2);
    addInstruction(0x45, "EOR", EOR, ZP0, 3);
    addInstruction(0x41, "EOR", EOR, IZX, 6);
    addInstruction(0x55, "EOR", EOR, ZPX, 4);
    addInstruction(0x51, "EOR", EOR, IZY, 5);

    addInstruction(0x18, "CLC", CLC, IMP, 2);
    addInstruction(0xD8, "CLD", CLD, IMP, 2);
    addInstruction(0x58, "CLI", CLI, IMP, 2);
    addInstruction(0xB8, "CLV", CLV, IMP, 2);

    addInstruction(0xC9, "CMP", CMP, IMM, 2);
    addInstruction(0xC5, "CMP", CMP, ZP0, 3);
    addInstruction(0xD5, "CMP", CMP, ZPX, 4);
    addInstruction(0xCD, "CMP", CMP, ABS, 4);
    addInstruction(0xDD, "CMP", CMP, ABX, 4);
    addInstruction(0xD9, "CMP", CMP, ABY, 4);
    addInstruction(0xC1, "CMP", CMP, IZX, 6);
    addInstruction(0xD1, "CMP", CMP, IZY, 5);

    addInstruction(0xE0, "CPX", CPX, IMM, 2);
    addInstruction(0xE4, "CPX", CPX, ZP0, 3);
    addInstruction(0xEC, "CPX", CPX, ABS, 4);

    addInstruction(0xC0, "CPY", CPY, IMM, 2);
    addInstruction(0xC4, "CPY", CPY, ZP0, 3);
    addInstruction(0xCC, "CPY", CPY, ABS, 4);

    addInstruction(0xC6, "DEC", DEC, ZP0, 5);
    addInstruction(0xD6, "DEC", DEC, ZPX, 6);
    addInstruction(0xCE, "DEC", DEC, ABS, 6);
    addInstruction(0xDe, "DEC", DEC, ABX, 7);
    addInstruction(0xCA, "DEX", DEX, IMP, 2);
    addInstruction(0x88, "DEY", DEY, IMP, 2);

    addInstruction(0xE6, "INC", INC, ZP0, 5);
    addInstruction(0xF6, "INC", INC, ZPX, 6);
    addInstruction(0xEE, "INC", INC, ABS, 6);
    addInstruction(0xFE, "INC", INC, ABX, 7);
    addInstruction(0xE8, "INX", INX, IMP, 2);
    addInstruction(0xC8, "INY", INY, IMP, 2);

    addInstruction(0x20, "JSR", JSR, ABS, 6);

    addInstruction(0x60, "RTS", RTS, IMP, 6);
    addInstruction(0x40, "RTI", RTI, IMP, 6);

    addInstruction(0xE9, "SBC", SBC, IMM, 2);
    addInstruction(0xE5, "SBC", SBC, ZP0, 3);
    addInstruction(0xF5, "SBC", SBC, ZPX, 4);
    addInstruction(0xED, "SBC", SBC, ABS, 4);
    addInstruction(0xFD, "SBC", SBC, ABX, 4);
    addInstruction(0xF9, "SBC", SBC, ABY, 4);
    addInstruction(0xE1, "SBC", SBC, IZX, 6);
    addInstruction(0xF1, "SBC", SBC, IZY, 5);

    addInstruction(0x38, "SEC", SEC, IMP, 2);
    addInstruction(0xF8, "SED", SED, IMP, 2);
    addInstruction(0x78, "SEI", SEI, IMP, 2);

    addInstruction(0x85, "STA", STA, ZP0, 3);
    addInstruction(0x95, "STA", STA, ZPX, 4);
    addInstruction(0x8D, "STA", STA, ABS, 4);
    addInstruction(0x9D, "STA", STA, ABX, 5);
    addInstruction(0x99, "STA", STA, ABY, 5);
    addInstruction(0x81, "STA", STA, IZX, 6);
    addInstruction(0x91, "STA", STA, IZY, 6);

    addInstruction(0x86, "STX", STX, ZP0, 3);
    addInstruction(0x96, "STX", STX, ZPY, 4);
    addInstruction(0x8E, "STX", STX, ABS, 4);

    addInstruction(0x84, "STY", STY, ZP0, 3);
    addInstruction(0x94, "STY", STY, ZPY, 4);
    addInstruction(0x8C, "STY", STY, ABS, 4);
}