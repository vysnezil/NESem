#include <cstdint>

class Save {
    struct saveState {
        uint8_t x, y, a, sp, status;
        uint16_t pc;
        uint16_t address;
        uint8_t cycles;
        uint8_t system_clocks;
    };
    char* name;
    uint8_t* preview;
    char* romHash;

};