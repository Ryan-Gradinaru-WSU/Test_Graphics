#pragma once
#include "./../default_includes.h"
#include "./../Error/InitError.h"
#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>

class BaseSDL
{
    
public:
    //MAKE PRIVATE
    SDL_Window* m_window;
    SDL_GPUDevice* m_gpuDevice;

    //Tests-dogs.png
    SDL_GPUTexture* m_dogsTexture = nullptr;
    SDL_GPUTexture* m_grassTexture = nullptr;
    int m_dogsW = 0;
    int m_dogsH = 0;
    SDL_GPUGraphicsPipeline* m_pipeline = nullptr;


    float m_tx = 640;
    float m_ty = 400;
    float m_rotationDeg = 90.0f;
    float m_scaleX = 80;
    float m_scaleY = 60;

    SDL_GPUSampler* m_dogsSampler = nullptr;
    SDL_GPUBuffer* m_quadBuffer;

    glm::mat4 m_mvp;
    //Tests-dogs.png



    bool runInitTests();
    BaseSDL( Uint32 flags = 0 );
    virtual ~BaseSDL();
    void draw();
};

