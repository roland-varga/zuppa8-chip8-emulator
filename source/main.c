#include "backend/emulator.h"
#include "frontend/screen.h"

int main(int argc, char **argv) {

    CPU cpu = {0};
    init(&cpu);

    // TODO: better input handling, errors, parsing args, etc
    if (!load_rom(&cpu, argv[1])) {
        printf("\nROM NOT FOUND!\n");
        return 1;
    }

    init_screen(&cpu);
    
    return 0;
}