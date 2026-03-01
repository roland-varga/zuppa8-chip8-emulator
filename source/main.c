#include "backend/emulator.h"
#include "frontend/screen.h"
#include <string.h>

// TODO: WARNINGS

int main(int argc, char **argv) {

    CPU cpu = {0};
    init(&cpu);

    // TODO: better input handling, errors, parsing args, etc
    if (!load_rom(&cpu, argv[1])) {
        printf("\nROM NOT FOUND!\n");
        return 1;
    }

    ColorMode color_mode = RED_MODE; // Default color mode
    
    if (argc >= 3) {
        if (strcmp(argv[2], "-r") == 0) color_mode = RED_MODE;
        else if (strcmp(argv[2], "-g") == 0) color_mode = GREEN_MODE;
        else if (strcmp(argv[2], "-b") == 0) color_mode = BLUE_MODE;
    }
    
    init_screen(&cpu, color_mode);
    
    return 0;
}