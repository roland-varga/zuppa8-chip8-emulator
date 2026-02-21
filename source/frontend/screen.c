#include "screen.h"

const double CPU_HZ = 700.0;

void init_screen(CPU *c) {
    // TODO: Game name in title:
    InitWindow(64 * 20, 32 * 20, "Zuppa-8 | ");
    SetTargetFPS(60);
    render(c);
}

void render(CPU *c) {
    
    ScreenSettings settings = {0};
    settings.cpu_accum = 0;
    settings.timer_accum = 0;

    Color bg_color = GetColor(0xB3B4B6FF);
    Color fg_color = GetColor(0x264BCCFF);

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