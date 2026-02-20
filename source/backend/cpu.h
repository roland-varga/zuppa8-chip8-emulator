#pragma once

#include <stdint.h>

typedef struct {
    uint16_t opcode;
    uint8_t memory[4096];
    uint8_t gfx[64 * 32];
    uint8_t registers[16];
    uint16_t index;
    uint16_t program_counter;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[16];
    uint16_t sp;
    uint8_t keys[16];
} CPU;

// TODO: docs
void init(CPU* c);

// TODO: docs
void poll_keys(CPU *c);

// TODO: docs
void cycle(CPU* c);