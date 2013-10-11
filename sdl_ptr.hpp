#ifndef UUID_94DC1BCC_1D92_428C_A617_2A51DCF1981A
#define UUID_94DC1BCC_1D92_428C_A617_2A51DCF1981A
#pragma once

#include <memory>
#include <SDL.h>

template <typename T, void (*Fun)(T*)>
struct FunDelete
{
    void operator()(T* ptr) const
    {
        Fun(ptr);
    }
};

typedef FunDelete<SDL_Window, SDL_DestroyWindow> WindowDeleter;
typedef std::unique_ptr<SDL_Window, WindowDeleter> WindowPtr;

typedef FunDelete<SDL_Renderer, SDL_DestroyRenderer> RendererDeleter;
typedef std::unique_ptr<SDL_Renderer, RendererDeleter> RendererPtr;

typedef FunDelete<SDL_Surface, SDL_FreeSurface> SurfaceDeleter;
typedef std::unique_ptr<SDL_Surface, SurfaceDeleter> SurfacePtr;

typedef FunDelete<SDL_Texture, SDL_DestroyTexture> TextureDeleter;
typedef std::unique_ptr<SDL_Texture, TextureDeleter> TexturePtr;

#endif
