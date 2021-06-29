#include "keyboard.h"
#include <iostream>
#include <bitset>

Keyboard::Keyboard() {

    // initialize keymaps
    keymap.insert({SDLK_1, 0x1});
    keymap.insert({SDLK_2, 0x2});
    keymap.insert({SDLK_3, 0x3});
    keymap.insert({SDLK_4, 0xC});
    keymap.insert({SDLK_q, 0x4});
    keymap.insert({SDLK_w, 0x5});
    keymap.insert({SDLK_e, 0x6});
    keymap.insert({SDLK_r, 0xD});
    keymap.insert({SDLK_a, 0x7});
    keymap.insert({SDLK_s, 0x8});
    keymap.insert({SDLK_d, 0x9});
    keymap.insert({SDLK_f, 0xE});
    keymap.insert({SDLK_z, 0xA});
    keymap.insert({SDLK_x, 0x0});
    keymap.insert({SDLK_c, 0xB});
    keymap.insert({SDLK_v, 0xF});

}

void Keyboard::read_key(std::bitset<16>& keys) {

    SDL_Event event;
    while(SDL_PollEvent(&event)) {

        SDL_Keycode key = event.key.keysym.sym;

        if (event.type == SDL_QUIT) {
            SDL_Quit();
            std::exit(0);
        }

        if (event.type == SDL_KEYDOWN) {
            if (keymap.find(key) != keymap.end()) {
                keys.reset();
                keys.set(keymap[key], true);
            }
        }

        if (event.type == SDL_KEYUP) {
            if (keymap.find(key) != keymap.end()) {
                keys.set(keymap[key], false);
            }
        }
    }
}
