#include <SDL3/SDL.h>
#include <iostream>

#include "sdl3.h"

auto main() -> int
{
    using std::cerr;
    using std::endl;

    auto sys = sdl3::make_sdlsystem(SDL_INIT_VIDEO);
    if (!sys) {
        cerr << "Error creating SDL3 system: " << SDL_GetError() << endl;
        return 1;
    }

    auto win = sdl3::make_window("Hello World!", 620, 387, SDL_WINDOW_RESIZABLE);
    if (!win) {
        cerr << "Error creating window: " << SDL_GetError() << endl;
        return 1;
    }

    auto ren
        = sdl3::make_renderer(win.get(), nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        cerr << "Error creating renderer: " << SDL_GetError() << endl;
        return 1;
    }

    auto file = SDL_RWFromFile(IMGDIR "grumpy-cat.bmp", "rb");
    if (file == nullptr) {
        cerr << "Error reading file: " << SDL_GetError() << endl;
        return 1;
    }

    auto bmp = sdl3::make_bmp(file);
    if (!bmp) {
        cerr << "Error creating surface: " << SDL_GetError() << endl;
        return 1;
    }

    auto tex = sdl3::make_texture(ren.get(), bmp.get());
    if (!tex) {
        cerr << "Error creating texture: " << SDL_GetError() << endl;
        return 1;
    }

    for (int i = 0; i < 20; i++) {
        SDL_RenderClear(ren.get());
        SDL_RenderTexture(ren.get(), tex.get(), nullptr, nullptr);
        SDL_RenderPresent(ren.get());
        SDL_Delay(100);
    }

    return 0;
}
