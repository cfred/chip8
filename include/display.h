#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "defs.h"
#include <memory>

class Display {

public:
    Display();
    ~Display();

    // White
    const SDL_Color foreground_color = {0xFF, 0xFF, 0xFF, 0xFF};
    // Black
    const SDL_Color background_color = {0x00, 0x00, 0x00, 0xFF};

    void draw(Byte buffer[]);

private:
    bool init();
    void close();
    void clear();

    std::shared_ptr<SDL_Window> window;
    std::shared_ptr<SDL_Renderer> renderer;
    const int pixel_scale = 10;

};

#endif // DISPLAY_H
