#ifndef CHIP8_H
#define CHIP8_H

#include <iostream>
#include <map>
#include <bitset>
#include <vector>
#include "defs.h"
#include "display.h"
#include "keyboard.h"


static Byte chip8_fontset[] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};


class Chip8 {

public:
    Chip8();
    ~Chip8();

    static const int memory_size = 4096;
    static const int num_registers = 16;
    static const int stack_size = 16;
    static const int num_keys = 16;
    static const DoubleByte font_address = 0x0000;
    static const int INSTRUCTIONS_PER_CYCLE;
    static const int SLEEP_TIME_BETWEEN_CYCLES_MS;
    const DoubleByte PROGRAM_START_ADDRESS = 0x0200;

    void run_application(const std::string&);
private:

    DoubleByte pc;
    DoubleByte I;
    Byte sp;
    Byte V[num_registers];
    DoubleByte stack[stack_size];
    std::vector<Byte> memory;
    Display display;
    Byte screen_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
    Byte delay_timer;
    Byte sound_timer;
    Keyboard keyboard;
    std::bitset<num_keys> keys;
    bool update_screen;

    void reset();
    void load_program_in_memory(const std::string&);
    void load_font_in_memory();
    DoubleByte fetch_instruction();
    void inc_program_counter();
    void dec_program_counter();
    DoubleByte decode_instruction(DoubleByte);
    void step();
    void update_timers();

    // debug
    void dump_screenbuffer();
    void dump_program();


    // instructions
    inline void instruction_00E0();
    inline void instruction_00EE();
    inline void instruction_1NNN(DoubleByte NNN);
    inline void instruction_2NNN(DoubleByte NNN);
    inline void instruction_3XNN(Byte X, Byte NN);
    inline void instruction_4XNN(Byte X, Byte NN);
    inline void instruction_5XY0(Byte X, Byte Y);
    inline void instruction_6XNN(Byte X, Byte NN);
    inline void instruction_7XNN(Byte X, Byte NN);
    inline void instruction_8XY0(Byte X, Byte Y);
    inline void instruction_8XY1(Byte X, Byte Y);
    inline void instruction_8XY2(Byte X, Byte Y);
    inline void instruction_8XY3(Byte X, Byte Y);
    inline void instruction_8XY4(Byte X, Byte Y);
    inline void instruction_8XY5(Byte X, Byte Y);
    inline void instruction_8XY6(Byte X);
    inline void instruction_8XY7(Byte X, Byte Y);
    inline void instruction_8XYE(Byte X);
    inline void instruction_9XY0(Byte X, Byte Y);
    inline void instruction_ANNN(DoubleByte NNN);
    inline void instruction_BNNN(DoubleByte NNN);
    inline void instruction_CXNN(Byte X, Byte NN);
    inline void instruction_DXYN(Byte X, Byte Y, Byte N);
    inline void instruction_EX9E(Byte X);
    inline void instruction_EXA1(Byte X);
    inline void instruction_FX07(Byte X);
    inline void instruction_FX0A(Byte X);
    inline void instruction_FX15(Byte X);
    inline void instruction_FX18(Byte X);
    inline void instruction_FX1E(Byte X);
    inline void instruction_FX29(Byte X);
    inline void instruction_FX33(Byte X);
    inline void instruction_FX55(Byte X);
    inline void instruction_FX65(Byte X);
};




#endif // CHIP8_H
