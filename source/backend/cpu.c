#include "cpu.h"
#include "emulator.h"
#include <stdlib.h>

unsigned char chip8_fontset[80] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void init(CPU *c) {
    srand((unsigned)time(NULL));

    c->program_counter = 0x200;
    c->opcode = 0;
    c->index = 0;
    c->stack_pointer = 0;
    c->sound_timer = 0;
    c->delay_timer = 0;

    memset(c->gfx, 0, sizeof(c->gfx));
    memset(c->memory, 0, sizeof(c->memory));
    memset(c->registers, 0, sizeof(c->registers));
    memset(c->stack, 0, sizeof(c->stack));
    memset(c->keys, 0, sizeof(c->keys));

    for (size_t i = 0; i < LEN(chip8_fontset); i++) {
        c->memory[FONTSET_START_ADDRESS + i] = chip8_fontset[i];
    }
}


// TODO: This needs a refactor, for smaller functions or something 
void cycle(CPU *c) {
    //fatch
    c->opcode = (uint16_t)((c->memory[c->program_counter] << 8) | c->memory[c->program_counter + 1]);

    // Default advance: every instruction is 2 bytes
    c->program_counter += 2;

    uint16_t first = c->opcode >> 12;

    switch (first) {
        case 0x0:
            if (c->opcode == 0x00E0) {
                for (size_t i = 0; i < LEN(c->gfx); i++) c->gfx[i] = 0;
            } else if (c->opcode == 0x00EE) {
                c->stack_pointer--;
                c->program_counter = c->stack[c->stack_pointer];
            }
            break;

        case 0x1:
            c->program_counter = c->opcode & 0x0FFF;
            break;

        case 0x2:
            c->stack[c->stack_pointer] = c->program_counter; // return address = next instruction
            c->stack_pointer++;
            c->program_counter = c->opcode & 0x0FFF;
            break;

        case 0x3: {
            int x = (c->opcode & 0x0F00) >> 8;
            if (c->registers[x] == (c->opcode & 0x00FF)) {
                c->program_counter += 2;
            }
        } break;

        case 0x4: {
            int x = (c->opcode & 0x0F00) >> 8;
            if (c->registers[x] != (c->opcode & 0x00FF)) {
                c->program_counter += 2;
            }
        } break;

        case 0x5: {
            if ((c->opcode & 0x000F) != 0) break;
            int x = (c->opcode & 0x0F00) >> 8;
            int y = (c->opcode & 0x00F0) >> 4;
            if (c->registers[x] == c->registers[y]) {
                c->program_counter += 2;
            }
        } break;

        case 0x6: {
            int x = (c->opcode & 0x0F00) >> 8;
            c->registers[x] = (uint8_t)(c->opcode & 0x00FF);
        } break;

        case 0x7: {
            int x = (c->opcode & 0x0F00) >> 8;
            c->registers[x] = (uint8_t)(c->registers[x] + (uint8_t)(c->opcode & 0x00FF));
        } break;

        case 0x8: {
            int x = (c->opcode & 0x0F00) >> 8;
            int y = (c->opcode & 0x00F0) >> 4;
            int m = c->opcode & 0x000F;

            switch (m) {
                case 0x0: c->registers[x]  = c->registers[y]; break;
                case 0x1: c->registers[x] |= c->registers[y]; break;
                case 0x2: c->registers[x] &= c->registers[y]; break;
                case 0x3: c->registers[x] ^= c->registers[y]; break;

                case 0x4: {
                    uint16_t sum = (uint16_t)c->registers[x] + (uint16_t)c->registers[y];
                    c->registers[0xF] = (sum > 255) ? 1 : 0;
                    c->registers[x] = (uint8_t)sum;
                } break;

                case 0x5:
                    c->registers[0xF] = (c->registers[x] >= c->registers[y]) ? 1 : 0;
                    c->registers[x] = (uint8_t)(c->registers[x] - c->registers[y]);
                    break;

                case 0x6:
                    c->registers[0xF] = c->registers[x] & 0x01;
                    c->registers[x] >>= 1;
                    break;

                case 0x7:
                    c->registers[0xF] = (c->registers[y] >= c->registers[x]) ? 1 : 0;
                    c->registers[x] = (uint8_t)(c->registers[y] - c->registers[x]);
                    break;

                case 0xE:
                    c->registers[0xF] = (c->registers[x] & 0x80) ? 1 : 0;
                    c->registers[x] <<= 1;
                    break;

                default:
                    break;
            }
        } break;

        case 0x9: {
            if ((c->opcode & 0x000F) != 0) break;
            int x = (c->opcode & 0x0F00) >> 8;
            int y = (c->opcode & 0x00F0) >> 4;
            if (c->registers[x] != c->registers[y]) {
                c->program_counter += 2;
            }
        } break;

        case 0xA:
            c->index = c->opcode & 0x0FFF;
            break;

        case 0xB:
            c->program_counter = (uint16_t)((c->opcode & 0x0FFF) + c->registers[0]);
            break;

        case 0xC: {
            int x = (c->opcode & 0x0F00) >> 8;
            int kk = c->opcode & 0x00FF;
            c->registers[x] = (uint8_t)(rand() & kk);
        } break;

        case 0xD: {
            int x = (c->opcode >> 8) & 0xF;
            int y = (c->opcode >> 4) & 0xF;
            int n = c->opcode & 0xF;

            int vx = c->registers[x];
            int vy = c->registers[y];

            c->registers[0xF] = 0;

            for (int row = 0; row < n; row++) {
                uint8_t sprite = c->memory[c->index + row];

                for (int col = 0; col < 8; col++) {
                    if ((sprite & (0x80 >> col)) != 0) {
                        int px = (vx + col) % 64;
                        int py = (vy + row) % 32;
                        int idx = px + py * 64;

                        if (c->gfx[idx] == 1) c->registers[0xF] = 1;
                        c->gfx[idx] ^= 1;
                    }
                }
            }
        } break;

        case 0xE: {
            int x = (c->opcode & 0x0F00) >> 8;
            int kk = c->opcode & 0x00FF;

            if (kk == 0x9E) {
                if (c->keys[c->registers[x]] == 1) c->program_counter += 2;
            } else if (kk == 0xA1) {
                if (c->keys[c->registers[x]] != 1) c->program_counter += 2;
            }
        } break;

        case 0xF: {
            int x = (c->opcode & 0x0F00) >> 8;
            int kk = c->opcode & 0x00FF;

            if (kk == 0x07) {
                c->registers[x] = c->delay_timer;
            } else if (kk == 0x0A) {
                bool key_pressed = false;
                for (size_t i = 0; i < LEN(c->keys); i++) {
                    if (c->keys[i] != 0) {
                        c->registers[x] = (uint8_t)i;
                        key_pressed = true;
                        break;
                    }
                }
                if (!key_pressed) {
                    c->program_counter -= 2;
                }
            } else if (kk == 0x15) {
                c->delay_timer = c->registers[x];
            } else if (kk == 0x18) {
                c->sound_timer = c->registers[x];
            } else if (kk == 0x1E) {
                c->index += c->registers[x];
            } else if (kk == 0x29) {
                c->index = (uint16_t)(0x50 + (c->registers[x] * 5));
            } else if (kk == 0x33) {
                c->memory[c->index]     = c->registers[x] / 100;
                c->memory[c->index + 1] = (c->registers[x] / 10) % 10;
                c->memory[c->index + 2] = c->registers[x] % 10;
            } else if (kk == 0x55) {
                for (int i = 0; i <= x; i++) {
                    c->memory[c->index + i] = c->registers[i];
                }
            } else if (kk == 0x65) {
                for (int i = 0; i <= x; i++) {
                    c->registers[i] = c->memory[c->index + i];
                }
            }
        } break;

        default:
            break;
    }
}