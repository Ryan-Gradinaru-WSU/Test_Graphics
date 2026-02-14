#pragma once
#include "./../default_includes.h"
#include "./../Error/InitError.h"
#include <SDL3/SDL_gpu.h>

class BaseSDL
{
    
public:
    //MAKE PRIVATE
    SDL_Window* m_window;
    SDL_GPUDevice* m_gpuDevice;




    bool runInitTests();
    BaseSDL( Uint32 flags = 0 );
    virtual ~BaseSDL();
    void draw();
};

