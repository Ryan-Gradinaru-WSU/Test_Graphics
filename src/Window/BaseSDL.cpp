#include "./../../headers/Window/BaseSDL.h"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
//for dogs
#include <SDL3_image/SDL_image.h>

#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   800


BaseSDL::BaseSDL( Uint32 flags )
{
    if(!SDL_Init(flags))
    {
        throw InitError();
    }
    DEBUG_PRINT("SDL_INIT SUCCESS!");

    m_window = SDL_CreateWindow("fortnite", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!m_window)
    {
        throw InitError();
    }
    DEBUG_PRINT("WINDOW CREATION SUCCESS!");


    m_gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, true, NULL);


    if (!m_gpuDevice)
    {
        throw InitError();
    }
    DEBUG_PRINT("GPU DEVICE CREATION SUCCESS!");

    if(!runInitTests()){
        DEBUG_PRINT("TESTS FAILED!");
        throw InitError();
    }
    

    if(!SDL_ClaimWindowForGPUDevice(m_gpuDevice, m_window)){
        throw InitError();
    }
    DEBUG_PRINT("GPU BIND SUCCESS!");
    //dog start
    SDL_Surface* surface = IMG_Load("C:/Users/USER/Desktop/WSU/2nd year/CPT_S-322/dogs.png");
    if (!surface){
        SDL_Log("Failed to load image: %s", SDL_GetError());
        throw InitError();
    }
    
    m_dogsW = surface->w;
    m_dogsH = surface->h;
    
    SDL_GPUTextureCreateInfo texinfo{};
    texinfo.width = m_dogsW;
    texinfo.height = m_dogsH;
    texinfo.format = SDL_GPU_TEXTUREFORMAT_RGBA8;
    texinfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLED | SDL_GPU_TEXTUREUSAGE_TRANSFER_DST;
    
    m_dogsTexture = SDL_CreateGPUTexture(m_gpuDevice, &texinfo);
    
    size_t uploadSize = surface->pitch * surface->h;
    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = uploadSize;
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    
    SDL_GPUTransferBuffer* staging = SDL_CreateGPUTransferBuffer(
        m_gpuDevice,
        &transferInfo
    );
    
    void* ptr = SDL_MapGPUTransferBuffer(m_gpuDevice, staging, false);
    memcpy(ptr, surface->pixels, uploadSize);
    SDL_UnmapGPUTransferBuffer(m_gpuDevice, staging);
    
    SDL_GPUCopyTextureRegion copy{};
    copy.texture = m_dogsTexture;
    copy.transfer_buffer = staging;
    copy.width = m_dogsW;
    copy.height = m_dogsH;
    
    SDL_UploadToGPUTexture(m_gpuDevice, &copy);
    
    SDL_DestroySurface(surface);
    DEBUG_PRINT("DOGS TEXTURE LOAD SUCCESS!");
    //dog end
}


BaseSDL::~BaseSDL()
{
    SDL_DestroyWindow( m_window );
    SDL_Quit();
}

bool BaseSDL::runInitTests(){
    int num_drivers = SDL_GetNumGPUDrivers();
    DEBUG_PRINT(num_drivers << " driver(s) supported on this machine!");

    std::string supported_driver = SDL_GetGPUDriver(0);
    DEBUG_PRINT(supported_driver << " will be used on this machine!");
    
    if(SDL_GPUSupportsShaderFormats(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, NULL))
        DEBUG_PRINT("D3X12 shaders and Vulkan shaders supported!");
    else{
        DEBUG_PRINT("ERROR - SHADERS NOT SUPPORTED.");
        return false;
    }


    if(SDL_GPUTextureSupportsFormat(m_gpuDevice, SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT, SDL_GPU_TEXTURETYPE_2D, 0) ? "yes" : "no")
        DEBUG_PRINT("R32G32B32A32_FLOAT supported for 2D textures!");
    else{
        DEBUG_PRINT("ERROR - COMPLEX TEXTURE FORMAT NOT SUPPORTED.");
        return false;
    }

    if(SDL_GPUTextureSupportsFormat(m_gpuDevice, SDL_GPU_TEXTUREFORMAT_D16_UNORM, SDL_GPU_TEXTURETYPE_2D, 0) ? "yes" : "no")
        DEBUG_PRINT("D16_UNORM supported for 2D textures!");
    else{
        DEBUG_PRINT("ERROR - DEPTH TEXTURE FORMAT NOT SUPPORTED.");
        return false;
    }

    return true;
}

// TEMP FUNCTION - STAGE WILL DRAW
void BaseSDL::draw()
{
    SDL_GPUCommandBuffer* test_cmdbuff = SDL_AcquireGPUCommandBuffer(m_gpuDevice);

    if(!test_cmdbuff){
        throw InitError();
    }

    SDL_GPUTexture* backbuffer;
    Uint32 w, h;

    if(!SDL_WaitAndAcquireGPUSwapchainTexture(test_cmdbuff, m_window, &backbuffer, &w, &h)){
        throw InitError();
    }

    if(!test_cmdbuff){
        throw InitError();
    }

    if (w == 0 || h == 0) {
        SDL_CancelGPUCommandBuffer(test_cmdbuff);
        return; // skip this frame
    }
    

    SDL_GPUColorTargetInfo color_target{};
    color_target.texture = backbuffer;
    color_target.clear_color = {1.0f, 0, 1.0f, 1.0f};
    color_target.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target.store_op = SDL_GPU_STOREOP_DONT_CARE;
    
    SDL_GPURenderPass* test_rp = SDL_BeginGPURenderPass(test_cmdbuff, &color_target, 1, NULL);

    if(!test_rp){
        throw InitError();
    }

    //dogsStart
    SDL_GPUBlitInfo blit{};
    blit.source.texture = m_dogsTexture;
    blit.source.w = m_dogsW;
    blit.source.h = m_dogsH;
    blit.destination.texture = backbuffer;
    blit.destination.x = 100;
    blit.destination.y = 100;
    blit.destination.w = m_dogsW;
    blit.destination.h = m_dogsH;
    blit.filter = SDL_GPU_FILTER_LINEAR;
    SDL_BlitGPUTexture(test_cmdbuff, &blit);
    //dogsEnd

    SDL_EndGPURenderPass(test_rp);
    SDL_SubmitGPUCommandBuffer(test_cmdbuff);
}
