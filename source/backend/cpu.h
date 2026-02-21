#pragma once

#include <stdint.h>
#include <string.h>
#include <time.h>

#define FONTSET_START_ADDRESS 0x50


/**
 * @brief Represents the state of the CHIP-8 virtual machine.
 *
 * Contains memory, registers, stack, timers, input state,
 * and video buffer required to emulate a CHIP-8 system.
 */
typedef struct {
    uint16_t opcode;            ///< Currently fetched opcode (2 bytes)

    uint8_t memory[4096];       ///< 4KB system memory
    uint8_t gfx[64 * 32];       ///< Monochrome display buffer (64x32 pixels)

    uint8_t registers[16];      ///< V0–VF general-purpose registers (VF = flag)
    uint16_t index;             ///< Index register (I)
    uint16_t program_counter;   ///< Program counter (PC)

    uint8_t delay_timer;        ///< Delay timer (decrements at 60Hz)
    uint8_t sound_timer;        ///< Sound timer (beeps while > 0)

    uint16_t stack[16];         ///< Call stack (16 levels)
    uint16_t stack_pointer;     ///< Stack pointer (SP)

    uint8_t keys[16];           ///< Hex-based keypad state (0x0–0xF)
} CPU;


/**
 * @brief Initializes the CPU to its default state.
 *
 * Resets registers, memory, stack, timers, and program counter.
 * Seeds the random number generator and loads the CHIP-8 fontset
 * into memory.
 *
 * @param c Pointer to the CPU instance to initialize.
 */
void init(CPU* c);


/**
 * @brief Executes one emulation cycle of the CHIP-8 CPU.
 *
 * Performs the fetch–decode–execute sequence:
 *  - Fetches the current 2-byte opcode from memory at the program counter.
 *  - Advances the program counter by 2 (default instruction size).
 *  - Decodes and executes the instruction.
 *
 * Handles control flow instructions (jumps, calls, returns, skips),
 * arithmetic/logical operations, drawing, input, and memory operations.
 *
 * @param c Pointer to the CPU instance.
 */
void cycle(CPU *c);