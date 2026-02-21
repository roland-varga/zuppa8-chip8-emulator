#pragma once

#include <stdio.h>
#include "raylib.h"
#include "cpu.h"

#define LEN(x) (sizeof(x) / sizeof((x)[0]))


/**
 * @brief Maps physical keyboard input to the CHIP-8 hexadecimal keypad.
 *
 * Updates the CPU key state array based on the current keyboard state
 * using raylib's IsKeyDown().
 *
 * CHIP-8 uses a 16-key hexadecimal keypad (0x0–0xF) arranged as:
 *
 *      1 2 3 C
 *      4 5 6 D
 *      7 8 9 E
 *      A 0 B F
 *
 * This implementation maps the keypad to the PC keyboard layout:
 *
 *      1 2 3 4
 *      Q W E R
 *      A S D F
 *      Z X C V
 *
 * The function should be called once per frame before executing
 * CPU cycles that depend on input (e.g., Ex9E, ExA1, Fx0A).
 *
 * @param c Pointer to the CPU instance whose key state will be updated.
 */
void map_keys(CPU *c);


/**
 * @brief Loads a CHIP-8 ROM into memory.
 *
 * Opens the specified ROM file and loads its contents into
 * memory starting at address 0x200 (the standard program start
 * location for CHIP-8).
 *
 * The function does not modify the CPU state beyond writing
 * the ROM bytes into memory.
 *
 * @param c Pointer to the CPU instance.
 * @param filename Path to the ROM file.
 *
 * @return true if the ROM was successfully loaded.
 * @return false if the file could not be opened or read.
 */
bool load_rom(CPU *c, const char *filename);