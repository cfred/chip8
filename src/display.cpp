#include <stdexcept>
#include "display.h"

std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> make_window(const char *title, int x, int y, int w, int h, Uint32 flags);
std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> make_renderer(SDL_Window* pw);

Display::Display() {
    init();
    clear();

}

Display::~Display() {
    close();
}

void Display::clear() {

    SDL_Renderer *pRenderer = renderer.get();
    SDL_SetRenderDrawColor(pRenderer, background_color.r, background_color.g, background_color.b, background_color.a);
    SDL_RenderClear(pRenderer);
    SDL_RenderPresent(pRenderer);

}

void Display::draw(Byte buffer[]) {

    clear();
    SDL_Renderer *pRenderer = renderer.get();
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {

        if (buffer[i] != 0x00) {

            int x = i % SCREEN_WIDTH;
            int y = i / SCREEN_WIDTH;

            SDL_Rect fillRect = { x*pixel_scale, y*pixel_scale, pixel_scale, pixel_scale };
            SDL_SetRenderDrawColor(pRenderer, foreground_color.r, foreground_color.g, foreground_color.b, foreground_color.a);
            SDL_RenderFillRect(pRenderer, &fillRect);
        }
    }
    //Update screen
    SDL_RenderPresent(pRenderer);

}


bool Display::init() {

    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        window = make_window("SDL Tutorial", 0, 0, SCREEN_WIDTH * pixel_scale, SCREEN_HEIGHT * pixel_scale, SDL_WINDOW_SHOWN);
        renderer = make_renderer(window.get());
    }
    return success;
}

void Display::close() {
    SDL_Quit();
}


std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> make_window(const char *title, int x, int y, int w, int h, Uint32 flags) {

    SDL_Window* pw = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!pw) {
        char msg[100];
        sprintf(msg, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        throw std::runtime_error(msg);
    }

    return std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(pw, SDL_DestroyWindow);
}

std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> make_renderer(SDL_Window* pw) {

    SDL_Renderer *pr = SDL_CreateRenderer(pw, -1, SDL_RENDERER_ACCELERATED);
    if (!pw) {
        char msg[100];
        sprintf(msg, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        throw std::runtime_error(msg);
    }

    return std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>(pr, SDL_DestroyRenderer);
}
