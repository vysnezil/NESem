#pragma once
#include <cstdint>
#include "json.hpp"

using json = nlohmann::json;

struct Save {
    struct CPUstate {
        uint8_t x, y, a, sp, status;
        uint16_t pc;
        uint16_t address;
        uint8_t cycles;
    } cpu_state;
    
    struct Sprite {
        uint8_t y;
        uint8_t id;
        uint8_t attribute;
        uint8_t x;
    };

    struct PPUstate {
        uint8_t status;
        uint8_t mask;
        uint8_t control;
        uint16_t vram_addr;
        uint16_t tram_addr;
        uint8_t fine_x;
        uint8_t* screen;
        Sprite OAM[64];
        uint8_t tblName[2][1024];
        uint8_t tblPattern[2][4096];
        uint8_t tblPalette[32];
    } ppu_state;

    uint8_t* ram;
    uint64_t system_clocks;
    std::string name;
    uint8_t* preview;
    std::string romHash;
};