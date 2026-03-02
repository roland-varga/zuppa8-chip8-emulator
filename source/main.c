#include "backend/emulator.h"
#include "frontend/screen.h"

// TODO: README.md
int main(int argc, char **argv) {

    CPU cpu = {0};
    init(&cpu);

    if (argc < 2) {
        printf("Usage: %s <rom> [-r|-g|-b]\n", argv[0]);
        return 1;
    }

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
    
    init_screen(&cpu, argv[1], color_mode);
    
    return 0;
}