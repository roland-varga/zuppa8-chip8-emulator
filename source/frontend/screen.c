#include "screen.h"

const double CPU_HZ = 700.0;

/**
 * @brief Builds a window title from a ROM file path.
 *
 * Extracts the filename from the provided file path, removes its
 * extension, and formats it into a window title string in the form:
 *
 *     "Zuppa-8 | <ROM Name>"
 *
 * The function supports both Windows ('\') and Unix ('/') path
 * separators.
 *
 * @param game_path   Full path to the ROM file.
 * @param output      Buffer that will receive the formatted title string.
 * @param output_size Size of the output buffer in bytes.
 *
 * @note The caller must provide a valid writable buffer for @p output.
 * @note The resulting string is truncated if it exceeds @p output_size.
 */
void parse_game_title(const char* game_path, char* output, size_t output_size) {
    const char *filename = strrchr(game_path, '\\');
    if (!filename) filename = strrchr(game_path, '/');
    filename = filename ? filename + 1 : game_path;

    char name[256];
    snprintf(name, sizeof(name), "%s", filename);

    char *dot = strrchr(name, '.');
    if (dot) *dot = '\0';

    snprintf(output, output_size, "Zuppa-8 | %s", name);
}


void init_screen(CPU *c, const char* game_name, ColorMode color_mode) {
    char title[256];
    parse_game_title(game_name, title, sizeof(title));
    
    InitWindow(64 * 20, 32 * 20, title);
    SetTargetFPS(60);
    render(c, color_mode);
}

void render(CPU *c, ColorMode color_mode) {
    
    ScreenSettings settings = {0};
    settings.cpu_accum = 0;
    settings.timer_accum = 0;

    Color bg_color = GetColor(0xB3B4B6FF);
    Color fg_color = {0};
    
    switch (color_mode) {
        case RED_MODE:
            fg_color = GetColor(RED_MODE);
            break;
        case GREEN_MODE:
            fg_color = GetColor(GREEN_MODE);
            break;
        case BLUE_MODE:
            fg_color = GetColor(BLUE_MODE);
            break;
    }

    while (!WindowShouldClose()) {
        double dt = GetFrameTime();
        settings.cpu_accum += dt;
        settings.timer_accum += dt;

        map_keys(c);

        while (settings.cpu_accum >= 1.0 / CPU_HZ) {
            cycle(c);
            settings.cpu_accum -= 1.0 / CPU_HZ;
        }

        while (settings.timer_accum >= 1.0 / 60.0) {
            if (c->delay_timer > 0) c->delay_timer--;
            if (c->sound_timer > 0) c->sound_timer--;
            settings.timer_accum -= 1.0 / 60.0;
        }

        BeginDrawing();
        ClearBackground(bg_color);

        // TODO: do this with texture
        int scale = 20;
        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 64; x++) {
                if (c->gfx[x + y * 64]) {
                    DrawRectangle(x * scale, y * scale, scale, scale, fg_color);
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();
}