#pragma once
#include "emulator.h"

/**
 * @brief Available display color modes.
 *
 * Defines preset color themes used when rendering the
 * CHIP-8 display output.
 */
typedef enum {
    RED_MODE,   /**< Render pixels using a red color theme */
    GREEN_MODE, /**< Render pixels using a green color theme */
    BLUE_MODE   /**< Render pixels using a blue color theme */
} ColorMode;

/**
 * @brief Runtime timing configuration for the screen system.
 *
 * This structure stores time accumulators used to control
 * CPU cycle timing and delay/sound timer updates independently.
 */
typedef struct {
    double cpu_accum;   /**< Accumulated time for CPU cycle stepping */
    double timer_accum; /**< Accumulated time for 60Hz timer updates */
} ScreenSettings;

/**
 * @brief Initializes the rendering system.
 *
 * Sets up the graphics window, rendering context, and any
 * visual configuration required to display the CHIP-8 output.
 *
 * This function should be called once before entering the
 * main emulation loop.
 *
 * @param c Pointer to the CPU instance.
 */
void init_screen(CPU *c);

/**
 * @brief Renders the current frame.
 *
 * Draws the CHIP-8 display buffer to the screen using the
 * currently selected color mode. Also handles timing logic
 * for CPU stepping and timer updates if integrated with
 * the render loop.
 *
 * @param c Pointer to the CPU instance.
 */
void render(CPU *c);