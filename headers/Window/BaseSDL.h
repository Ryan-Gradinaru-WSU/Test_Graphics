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

    //Tests-dogs.png
    SDL_GPUTexture* m_dogsTexture = nullptr;
    int m_dogsW = 0;
    int m_dogsH = 0;
    SDL_GPUGraphicsPipeline* m_pipeline = nullptr;

    SDL_GPUSampler* m_dogsSampler = nullptr;
    SDL_GPUBuffer* m_quadBuffer;
    //Tests-dogs.png



    bool runInitTests();
    BaseSDL( Uint32 flags = 0 );
    virtual ~BaseSDL();
    void draw();
};

