#include "chip8.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: chip8 filename" << std::endl;
        std::exit(0);
    }

    Chip8 chip8;
    chip8.run_application(argv[1]);

    return 0;
}



