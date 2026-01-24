#pragma once
#include "./../default_includes.h"
#include "./../Error/InitError.h"

class BaseSDL
{
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
public:
    BaseSDL( Uint32 flags = 0 );
    virtual ~BaseSDL();
    void draw();
};

