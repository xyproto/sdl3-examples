#pragma once
#include <SDL3/SDL.h>
#include <memory>
namespace sdl3 {

// Very useful function from Eric Scott Barr:
// https://eb2.co/blog/2014/04/c-plus-plus-14-and-sdl3-managing-resources/
template <typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args)
{
    auto r = c(std::forward<Arguments>(args)...);
    return std::unique_ptr<std::decay_t<decltype(*r)>, decltype(d)>(r, d);
}

// The "internal type" of the SDL System
using SDL_System = int;

// SDL_CreateSDL initiates the use of SDL.
// The given flags are passed to SDL_Init.
// The returned value contains the exit code.
inline SDL_System* SDL_CreateSDL(Uint32 flags)
{
    auto init_status = new SDL_System;
    *init_status = SDL_Init(flags);
    return init_status;
}

// SDL_DestroySDL ends the use of SDL
inline void SDL_DestroySDL(SDL_System*)
{
    SDL_Quit();
}

using sdlsystem_ptr_t = std::unique_ptr<SDL_System, decltype(&SDL_DestroySDL)>;
using window_ptr_t = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
using renderer_ptr_t = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
using surf_ptr_t = std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)>;
using texture_ptr_t = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

// Initialize SDL (the returned int* contains the return value from SDL_Init)
inline sdlsystem_ptr_t make_sdlsystem(Uint32 flags)
{
    return make_resource(SDL_CreateSDL, SDL_DestroySDL, flags);
}

// Create a window and renderer together
inline std::pair<window_ptr_t, renderer_ptr_t> make_window_and_renderer(
    const char* title,
    int width,
    int height,
    SDL_WindowFlags flags)
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer(title, width, height, flags, &window, &renderer)) {
        return { window_ptr_t(nullptr, SDL_DestroyWindow),
            renderer_ptr_t(nullptr, SDL_DestroyRenderer) };
    }

    return { window_ptr_t(window, SDL_DestroyWindow),
        renderer_ptr_t(renderer, SDL_DestroyRenderer) };
}

// Create a surface from a bmp file, containing both the surface and the destructor
inline surf_ptr_t make_bmp(SDL_IOStream* sdlfile)
{
    // May throw an exception if sdlfile is nullptr
    return make_resource(SDL_LoadBMP_IO, SDL_DestroySurface, sdlfile, 1);
}

// Create a texture from a renderer and a surface
inline texture_ptr_t make_texture(SDL_Renderer* ren, SDL_Surface* surf)
{
    return make_resource(SDL_CreateTextureFromSurface, SDL_DestroyTexture, ren, surf);
}

} // namespace sdl3
