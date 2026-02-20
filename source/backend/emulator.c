#include "emulator.h"

void map_keys(CPU *c) {
    c->keys[0x1] = IsKeyDown(KEY_ONE);
    c->keys[0x2] = IsKeyDown(KEY_TWO);
    c->keys[0x3] = IsKeyDown(KEY_THREE);
    c->keys[0xC] = IsKeyDown(KEY_FOUR);

    c->keys[0x4] = IsKeyDown(KEY_Q);
    c->keys[0x5] = IsKeyDown(KEY_W);
    c->keys[0x6] = IsKeyDown(KEY_E);
    c->keys[0xD] = IsKeyDown(KEY_R);

    c->keys[0x7] = IsKeyDown(KEY_A);
    c->keys[0x8] = IsKeyDown(KEY_S);
    c->keys[0x9] = IsKeyDown(KEY_D);
    c->keys[0xE] = IsKeyDown(KEY_F);

    c->keys[0xA] = IsKeyDown(KEY_Z);
    c->keys[0x0] = IsKeyDown(KEY_X);
    c->keys[0xB] = IsKeyDown(KEY_C);
    c->keys[0xF] = IsKeyDown(KEY_V);
}