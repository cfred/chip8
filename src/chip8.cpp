#include "chip8.h"
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>

void invalid_instruction(int opcode);

const int Chip8::INSTRUCTIONS_PER_CYCLE = 20;
const int Chip8::SLEEP_TIME_BETWEEN_CYCLES_MS = 20;

Chip8::Chip8(): pc(0), memory(memory_size), delay_timer(0), sound_timer(0), update_screen(false) {

    std::fill(std::begin(memory), std::end(memory), 0x00);
    std::fill(std::begin(screen_buffer), std::end(screen_buffer), 0x00);
    std::fill(std::begin(V), std::end(V), 0x00);
    std::fill(std::begin(stack), std::end(stack), 0x00);
}

Chip8::~Chip8() {

}

void Chip8::step() {

    DoubleByte opcode;

    keyboard.read_key(keys);
    opcode = fetch_instruction();
    decode_instruction(opcode);

    if (update_screen) {
        display.draw(screen_buffer);
    }
}

void Chip8::run_application(const std::string& program_name) {

    load_program_in_memory(program_name);
    load_font_in_memory();

    pc = PROGRAM_START_ADDRESS;

    //dump_program();
    //dump_screenbuffer();

    while(true) {

        for(int i = 0; i < Chip8::INSTRUCTIONS_PER_CYCLE; i++) {
            step();
        }

        update_timers();
        std::this_thread::sleep_for(std::chrono::milliseconds(Chip8::SLEEP_TIME_BETWEEN_CYCLES_MS));

    }
}

void Chip8::update_timers() {

    if (delay_timer > 0) {
        delay_timer--;
    }
}

void Chip8::load_program_in_memory(const std::string& program_name) {

    std::ifstream program_file(program_name, std::ios::binary | std::ios::in);
    if (!program_file.is_open()) {
        std::cerr << "File not found: " << program_name << std::endl;
        std::abort();
    }

    unsigned int size = program_file.tellg();

    if (size >= memory_size - PROGRAM_START_ADDRESS) {
        std::cerr << "Can't load. Program size too big." << std::endl;
        std::abort();
    }

    Byte b;
    DoubleByte addr = PROGRAM_START_ADDRESS;

    /*std::copy(std::istreambuf_iterator<char>(program_file),
              std::istreambuf_iterator<char>(),
              std::next(std::begin(memory), program_start_address));*/

    while(program_file >> std::noskipws >> b) {
        memory[addr] = b;
        addr++;
    }

    program_file.close();
}

void Chip8::load_font_in_memory() {
    std::copy(std::begin(chip8_fontset), std::end(chip8_fontset), std::next(std::begin(memory), font_address));
}

DoubleByte Chip8::fetch_instruction() {
    // opcodes are big endian

    DoubleByte opcode = memory[pc];
    opcode <<= 8;
    opcode |= memory[pc+1];

    inc_program_counter();
    return opcode;
}

void Chip8::inc_program_counter() {
    pc += 2;
}

void Chip8::dec_program_counter() {
    pc -= 2;
}

DoubleByte Chip8::decode_instruction(DoubleByte opcode) {

    Byte X, Y, N, NN;
    DoubleByte NNN;

    X = (opcode & 0x0F00) >> 8;
    Y = (opcode & 0x00F0) >> 4;
    N = opcode & 0x000F;
    NN = opcode & 0x00FF;
    NNN = opcode & 0x0FFF;
    update_screen = false;

    //std::cout << std::hex << opcode << std::endl;
    switch(opcode & 0xF000) {

        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0:
                    instruction_00E0();
                break;

                case 0x00EE:
                    instruction_00EE();
                break;

                default:
                    invalid_instruction(opcode);
                break;
            }
        break;

        case 0x1000:
            instruction_1NNN(NNN);
        break;

        case 0x2000:
            instruction_2NNN(NNN);
        break;

        case 0x3000:
            instruction_3XNN(X, NN);
        break;

        case 0x4000:
            instruction_4XNN(X, NN);
        break;

        case 0x5000:
            instruction_5XY0(X, Y);
        break;

        case 0x6000:
            instruction_6XNN(X, NN);
        break;

        case 0x7000:
            instruction_7XNN(X, NN);
        break;

        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0000:
                    instruction_8XY0(X, Y);
                break;
                case 0x0001:
                    instruction_8XY1(X, Y);
                break;
                case 0x0002:
                    instruction_8XY2(X, Y);
                break;
                case 0x0003:
                    instruction_8XY3(X, Y);
                break;
                case 0x0004:
                    instruction_8XY4(X, Y);
                break;
                case 0x0005:
                    instruction_8XY5(X, Y);
                break;
                case 0x0006:
                    instruction_8XY6(X);
                break;
                case 0x0007:
                    instruction_8XY7(X, Y);
                break;
                case 0x000E:
                    instruction_8XYE(X);
                break;
                default:
                    invalid_instruction(opcode);
                break;
            }

        break;

        case 0x9000:
            instruction_9XY0(X, Y);
        break;

        case 0xA000:
            instruction_ANNN(NNN);
        break;

        case 0xB000:
            instruction_BNNN(NNN);
        break;

        case 0xC000:
            instruction_CXNN(X, NN);
        break;

        case 0xD000:
            instruction_DXYN(X, Y, N);
        break;

        case 0xE000:
            switch(opcode & 0x00FF) {
                case 0x009E:
                    instruction_EX9E(X);
                break;

                case 0x00A1:
                    instruction_EXA1(X);
                break;

                default:
                    invalid_instruction(opcode);
                break;
            }
        break;

        case 0xF000:
            switch(opcode & 0x00FF) {
                case 0x0007:
                    instruction_FX07(X);
                break;

                case 0x000A:
                    instruction_FX0A(X);
                break;

                case 0x0015:
                    instruction_FX15(X);
                break;

                case 0x0018:
                    instruction_FX18(X);
                break;

                case 0x001E:
                    instruction_FX1E(X);
                break;

                case 0x0029:
                    instruction_FX29(X);
                break;

                case 0x0033:
                    instruction_FX33(X);
                break;

                case 0x0055:
                    instruction_FX55(X);
                break;

                case 0x0065:
                    instruction_FX65(X);
                break;

                default:
                    invalid_instruction(opcode);
                break;
            }

        break;

        default:
            invalid_instruction(opcode);
        break;

    }

    return 1;

}

void Chip8::instruction_00E0() {
    std::fill(std::begin(screen_buffer), std::end(screen_buffer), 0x00);
    update_screen = true;
}

void Chip8::instruction_00EE() {
    pc = stack[--sp];
}

void Chip8::instruction_1NNN(DoubleByte NNN) {
    pc = NNN;
}

void Chip8::instruction_2NNN(DoubleByte NNN) {
    stack[sp++] = pc;
    pc = NNN;
}

void Chip8::instruction_3XNN(Byte X, Byte NN) {
    if (V[X] == NN) {
        inc_program_counter();
    }
}

void Chip8::instruction_4XNN(Byte X, Byte NN) {
    if (V[X] != NN) {
        inc_program_counter();
    }
}

void Chip8::instruction_5XY0(Byte X, Byte Y) {
    if (V[X] == V[Y]) {
        inc_program_counter();
    }
}

void Chip8::instruction_6XNN(Byte X, Byte NN) {
    V[X] = NN;
}

void Chip8::instruction_7XNN(Byte X, Byte NN) {
    V[X] += NN;
}

void Chip8::instruction_8XY0(Byte X, Byte Y) {
    V[X] = V[Y];
}

void Chip8::instruction_8XY1(Byte X, Byte Y) {
    V[X] |= V[Y];
}

void Chip8::instruction_8XY2(Byte X, Byte Y) {
    V[X] &= V[Y];
}

void Chip8::instruction_8XY3(Byte X, Byte Y) {
    V[X] ^= V[Y];
}

void Chip8::instruction_8XY4(Byte X, Byte Y) {
    V[0xF] = V[X] > (0xFF - V[Y]) ? 0x01: 0x00;
    V[X] += V[Y];

}

void Chip8::instruction_8XY5(Byte X, Byte Y) {
    V[0xF] = V[X] > V[Y] ? 0x01: 0x00;
    V[X] -= V[Y];
}

void Chip8::instruction_8XY6(Byte X) {
    V[0xF] = V[X] & 0x01;
    V[X] >>= 1;
}

void Chip8::instruction_8XY7(Byte X, Byte Y) {
    V[0xF] = V[Y] > V[X] ? 0x01: 0x00;
    V[X] = V[Y] - V[X];
}

void Chip8::instruction_8XYE(Byte X) {
    V[0xF] = V[X] >> 7;
    V[X] <<= 1;
}

void Chip8::instruction_9XY0(Byte X, Byte Y) {
    if (V[X] != V[Y]) {
        inc_program_counter();
    }
}

void Chip8::instruction_ANNN(DoubleByte NNN) {
    I = NNN;
}

void Chip8::instruction_BNNN(DoubleByte NNN) {
    pc = V[0x0] + NNN;
}

void Chip8::instruction_CXNN(Byte X, Byte NN) {
    V[X] = static_cast<Byte>(rand() % 256) & NN;
}

void Chip8::instruction_DXYN(Byte X, Byte Y, Byte N) {
    //Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N+1 pixels.
    //Each row of 8 pixels is read as bit-coded starting from memory location I;
    //I value does not change after the execution of this instruction.
    //As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
    //and to 0 if that does not happen
    V[0xF] = 0;
    for (int yline = 0; yline < N; yline++) {

        Byte pixel = memory[I + yline];
        for(int xline = 0; xline < 8; xline++) {

            if((pixel & (0x80 >> xline)) != 0) {

                if(screen_buffer[(V[X] + xline + ((V[Y] + yline) * SCREEN_WIDTH)) % (2048)] == 1) {
                    V[0xF] = 1;
                }
                screen_buffer[(V[X] + xline + ((V[Y] + yline) * SCREEN_WIDTH)) % (2048)] ^= 1;
            }
        }
    }
    update_screen = true;
}

void Chip8::instruction_EX9E(Byte X) {
    if (keys.test(V[X])) {
        inc_program_counter();
    }
}

void Chip8::instruction_EXA1(Byte X) {
    if (!keys.test(V[X])) {
        inc_program_counter();
    }
}

void Chip8::instruction_FX07(Byte X) {
    V[X] = delay_timer;
}

void Chip8::instruction_FX0A(Byte X) {

    if(!keys.any()) {
        dec_program_counter();
    } else {
        for (std::size_t i = 0; i < keys.size(); i++) {
            if (keys.test(i)) {
                V[X] = (Byte)i;
                break;
            }
        }
    }
}

void Chip8::instruction_FX15(Byte X) {
    delay_timer = V[X];
}

void Chip8::instruction_FX18(Byte X) {
    sound_timer = V[X];
}

void Chip8::instruction_FX1E(Byte X) {
    I += V[X];
}

void Chip8::instruction_FX29(Byte X) {
    I = font_address + V[X] * 5;
}

void Chip8::instruction_FX33(Byte X) {

    memory[I] =  V[X] / 100;
    memory[I + 1] = (V[X] / 10) % 10;
    memory[I + 2] = (V[X] % 100) % 10;

//    int n = V[X];
//    int c = 2;
//    while(c >= 0) {
//        int d = n % 10;
//        memory[I + c] = d;
//        n = n / 10;
//        c--;
//    }

}

void Chip8::instruction_FX55(Byte X) {
    for (Byte i = 0; i <= X; i++) {
        memory[I + i] = V[i];
    }
}

void Chip8::instruction_FX65(Byte X) {
    for (Byte i = 0; i <= X; i++) {
        V[i] = memory[I + i];
    }
}

void Chip8::dump_screenbuffer() {

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {

        if (i % SCREEN_WIDTH == 0) {
            std::cout << std::endl;
        }
        std::cout << static_cast<int>(screen_buffer[i]);
    }
}

void Chip8::dump_program() {

    pc = 0x0200;
    std::cout << "PROGRAM START" << std::endl;
    while(pc <= 0xFFF) {
        DoubleByte opcode = fetch_instruction();
        std::cout << std::setbase(16) << opcode << std::endl;
    }
    std::cout << "PROGRAM END" << std::endl;

}

void invalid_instruction(int opcode) {
    std::ostringstream oss("Invalid Instruction", std::ios::ate);
    oss << " " << std::hex << opcode;
    throw std::runtime_error(oss.str());
}
