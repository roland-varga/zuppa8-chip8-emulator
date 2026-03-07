#pragma once
#include "emulator.h"
#include <string.h>

/**
 * @brief Predefined foreground color modes for rendering.
 *
 * Each value represents a 32-bit RGBA color encoded as
 * 0xRRGGBBAA, compatible with raylib's GetColor().
 *
 * These values are used to determine the color of active
 * pixels when drawing the CHIP-8 display buffer.
 */
typedef enum {
    RED_MODE   = 0xE7363CFF, /**< Red foreground color */
    GREEN_MODE = 0x59AC99FF, /**< Green foreground color */
    BLUE_MODE  = 0x3E446EFF  /**< Blue foreground color */
} ColorMode;

/**
 * @brief Timing state used by the rendering loop.
 *
 * Stores accumulated delta time values in order to:
 * - Step the CPU at a fixed frequency.
 * - Update delay and sound timers at 60 Hz.
 *
 * These accumulators allow decoupling emulation timing
 * from the display frame rate.
 */
typedef struct {
    double cpu_accum;   /**< Accumulated time for CPU execution steps */
    double timer_accum; /**< Accumulated time for 60 Hz timer updates */
} ScreenSettings;

/**
 * @brief Initializes the graphical display system.
 *
 * Creates the application window, configures rendering
 * parameters, and starts the main render loop.
 *
 * The window title is derived from the provided ROM name.
 *
 * @param c           Pointer to the initialized CPU instance.
 * @param game_name   Path or name of the loaded ROM.
 * @param color_mode  Foreground color mode for rendering.
 *
 * @pre The CPU must be initialized and a ROM must be loaded.
 */
void init_screen(CPU *c, const char* game_name, ColorMode color_mode);

/**
 * @brief Executes the main rendering and emulation loop.
 *
 * Continuously:
 * - Processes input
 * - Steps the CPU at the configured frequency
 * - Updates timers at 60 Hz
 * - Draws the display buffer to the window
 *
 * The loop runs until the window is closed.
 *
 * @param c          Pointer to the CPU instance.
 * @param color_mode Foreground color mode used for drawing.
 */
void render(CPU *c, ColorMode color_mode);