#pragma once
#include "emulator.h"

typedef enum {
    RED_MODE,
    GREEN_MODE,
    BLUE_MODE,
} ColorMode;

typedef struct {
    double cpu_accum;
    double timer_accum;
} ScreenSettings;


void init_screen(CPU *c);
void render(CPU *c);