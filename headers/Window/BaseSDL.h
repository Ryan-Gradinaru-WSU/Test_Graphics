#pragma once
#include "./../default_includes.h"
#include "./../Error/InitError.h"

class BaseSDL
{
    
public:
    //MAKE PRIVATE
    SDL_Window * m_window;

    BaseSDL( Uint32 flags = 0 );
    virtual ~BaseSDL();
    void draw();
};

