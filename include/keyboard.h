#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>
#include <map>
#include <bitset>
#include "defs.h"

class Keyboard {

public:
    Keyboard();
    ~Keyboard() {};

    void read_key(std::bitset<16>& keys);
private:
   std::map<SDL_Keycode, Byte> keymap;

};

#endif // KEYBOARD_H
