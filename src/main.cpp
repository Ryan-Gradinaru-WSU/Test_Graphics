#include "./../headers/Window/BaseSDL.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>

int main(int argc, char* argv[])
{
    bool quit = false;
    /* block: temp code */
    //SDL_SetHint(SDL_HINT_GPU_DRIVER, "vulkan");
    std::cout << "SDL3 lib path: " << SDL_GetBasePath() << std::endl;

    std::cout 
        << "VIDEO INIT SUCCEESS: "
        << (SDL_Init(SDL_INIT_VIDEO) ? "true" : "false")
        << std::endl;

    std::cout 
        << SDL_GetError()
        << std::endl;


    SDL_Log("GPU DRIVER COUNT: %d\n", SDL_GetNumGPUDrivers());
    SDL_Log("SDL GET GPU DRIVER: %s\n", SDL_GetGPUDriver(0));

    //SDL_Vulkan_LoadLibrary(nullptr);
    SDL_GPUDevice* test_dev;
    test_dev = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, true, NULL);


    SDL_GPUTextureCreateInfo test_texInfo;
    test_texInfo.format = SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT;

    std::cout 
        << SDL_GetError()
        << std::endl;

    std::cout
        << "DEVICE EXISTS: "
        << (test_dev ? "yes" : "no")
        << std::endl;

    std::cout 
        << "IS DEVICE FORMAT SUPPORTED: "
        << (SDL_GPUSupportsShaderFormats(4, NULL) ? "yes" : "no")
        << std::endl;

    std::cout   
        << "IS TEXTURE FORMAT SUPPORTED: " 
        << (SDL_GPUTextureSupportsFormat(test_dev, test_texInfo.format, SDL_GPU_TEXTURETYPE_2D, 0) ? "yes" : "no")
        << std::endl;
    
    //    test_texInfo.height;
    SDL_GPUTexture* test_tex;
    //test_tex = SDL_CreateGPUTexture(test_dev, )

    /* end block */

    try
    {
        BaseSDL b_sdl( SDL_INIT_VIDEO);


        /* block: temp code */

        if(!SDL_ClaimWindowForGPUDevice(test_dev, b_sdl.m_window)){
            std::cout 
                << SDL_GetError()
                << std::endl;
            return 0;
        }

        SDL_GPUCommandBuffer* test_buff = SDL_AcquireGPUCommandBuffer(test_dev);

        SDL_GPUTexture* backbuffer;
        Uint32 w, h;

        if(!SDL_AcquireGPUSwapchainTexture(test_buff, b_sdl.m_window, &backbuffer, &w, &h)){
            std::cout 
                << SDL_GetError()
                << std::endl;
            return 0;
        }

        SDL_GPUColorTargetInfo color_target = {};
            color_target.texture = backbuffer;
            color_target.clear_color = {1.0f, 0, 1.0f, 1.0f};
            color_target.load_op = SDL_GPU_LOADOP_CLEAR;
            color_target.store_op = SDL_GPU_STOREOP_STORE;
        
        SDL_GPUDepthStencilTargetInfo* test_dstarget = nullptr;

        SDL_GPURenderPass* test_rp = SDL_BeginGPURenderPass(test_buff, &color_target, 1, test_dstarget);

        SDL_EndGPURenderPass(test_rp);
        SDL_SubmitGPUCommandBuffer(test_buff);
        /* end block */


        while(!quit)
        {
            SDL_Event e;

            // Get available event
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_EVENT_QUIT)
                {
                    quit = true;
                    break;
                }
                else if(e.type == SDL_EVENT_KEY_DOWN)
                {
                    switch (e.key.key)
                    {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    }
                }
                //TEMP REMOVE
                //b_sdl.draw();
            }
        }


        return 0;
    }
    
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing BaseSDL:  "
                  << err.what()
                  << std::endl;

        std::cerr << SDL_GetError() << std::endl;
    }
    
    //catch(){}
    return 1;
}
