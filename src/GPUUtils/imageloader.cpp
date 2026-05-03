#include "../../headers/default_includes.h"
#include "../../headers/Error/InitError.h"


SDL_GPUTexture* UploadImage(std::string image_path, SDL_GPUDevice* gpu_device){
    DEBUG_PRINT("PNG LOADING START!");
    //load image as raw surface
    SDL_Surface* raw_surface = SDL_LoadPNG(image_path.c_str()); // pull surface from dog.png in resoruces

    if (!raw_surface){
        throw InitError();
    }
    DEBUG_PRINT("   LOADED RAW PNG!");
    
    //convert surface to usable format
    SDL_Surface* surface = SDL_ConvertSurface(raw_surface, SDL_PIXELFORMAT_RGBA32);

    

    if (!surface){
        throw InitError();
    }

    //destroy unusable surface
    SDL_DestroySurface(raw_surface);

    DEBUG_PRINT(((int*)surface->pixels)[0]);

    DEBUG_PRINT("   CONVERSION SUCCESS!");

    int surface_width = surface->w;
    int surface_height = surface->h;

    DEBUG_PRINT("m_dogsW: " << surface_width);
    DEBUG_PRINT("m_dogsH: " << surface_height);
    
    //create gpu specific texture 

    SDL_GPUTextureCreateInfo texinfo{};
        texinfo.type = SDL_GPU_TEXTURETYPE_2D;
        texinfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        texinfo.width = surface_width;
        texinfo.height = surface_height;
        texinfo.num_levels = 1;
        texinfo.layer_count_or_depth = 1;
        texinfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    
    SDL_GPUTexture* surface_texture = SDL_CreateGPUTexture(gpu_device, &texinfo);


    if (!surface_texture){
        throw InitError();
    }
    DEBUG_PRINT("   TEXTURE CREATED SUCCESSFULLY!");

    DEBUG_PRINT("PNG LOADED SUCCESSFULLY!");

    DEBUG_PRINT("GPU UPLOAD START!");
    
    //set up variables needed for upload
    Uint32 uploadSize = surface->pitch * surface->h;

    SDL_GPUTransferBufferCreateInfo transferInfo{};
        transferInfo.size = uploadSize;
        transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    
    SDL_GPUTransferBuffer* staging = SDL_CreateGPUTransferBuffer(
        gpu_device,
        &transferInfo
    );
    
    if (!staging){
        throw InitError();
    }
    DEBUG_PRINT("   INTERMEDIATE STAGING SUCCESS!");


    
    void* map = SDL_MapGPUTransferBuffer(gpu_device, staging, false);

    if (!map){
        throw InitError();
    }
    DEBUG_PRINT("   MAP TRANSFER SUCCESS!");

    memcpy(map, surface->pixels, uploadSize);
    SDL_UnmapGPUTransferBuffer(gpu_device, staging);

    
    SDL_GPUTextureTransferInfo src{};
        src.transfer_buffer = staging;
        src.offset = 0;
        //src.pixels_per_row = surface->pitch / 4;
        //src.rows_per_layer = surface->h;

    
    SDL_GPUTextureRegion dst{};
        dst.texture = surface_texture;
        dst.x = 0;
        dst.y = 0;
        dst.z = 0;
        dst.w = surface_width;
        dst.h = surface_height;
        dst.d = 1;
        dst.layer = 0;
        dst.mip_level = 0;

    //UPLOAD TEXTURE
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(gpu_device);
    SDL_GPUCopyPass* copy = SDL_BeginGPUCopyPass(cmd);
    
    SDL_UploadToGPUTexture(copy, &src, &dst, false);

    SDL_WaitForGPUIdle(gpu_device);

    SDL_EndGPUCopyPass(copy);
    SDL_SubmitGPUCommandBuffer(cmd);

    int readW = surface_width;
    int readH = surface_height;
    
    int startX = 0;
    int startY = 0;

    Uint32 size = readW * readH * sizeof(uint32_t);

    SDL_GPUTransferBufferCreateInfo tbinfo{};
    tbinfo.size  = size;
    tbinfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;

    SDL_GPUTransferBuffer* downloadBuffer =
        SDL_CreateGPUTransferBuffer(gpu_device, &tbinfo);

    SDL_GPUTextureTransferInfo dlnd_dst{};
    dlnd_dst.transfer_buffer = downloadBuffer;          // NULL means “write directly to CPU memory”
    dlnd_dst.offset = 0;
    //dlnd_dst.pixels_per_row = readW;
    //dlnd_dst.rows_per_layer = readH;

    // Describe the region of the texture to read
    SDL_GPUTextureRegion region{};
    region.texture   = surface_texture;
    region.x         = startX;
    region.y         = startY;
    region.z         = 0;
    region.w         = readW;
    region.h         = readH;
    region.d         = 1;
    region.layer     = 0;
    region.mip_level = 0;

    SDL_GPUCommandBuffer* dlnd_cmd = SDL_AcquireGPUCommandBuffer(gpu_device);
    SDL_GPUCopyPass* dlnd_copy = SDL_BeginGPUCopyPass(dlnd_cmd);
    
    SDL_DownloadFromGPUTexture(dlnd_copy, &region, &dlnd_dst);


    SDL_EndGPUCopyPass(dlnd_copy);
    SDL_SubmitGPUCommandBuffer(dlnd_cmd);
    
    SDL_WaitForGPUIdle(gpu_device);

    void* mapped = SDL_MapGPUTransferBuffer(gpu_device, dlnd_dst.transfer_buffer, false);
    uint32_t* pixels = reinterpret_cast<uint32_t*>(mapped);

    bool anyNonZero = false;

    for (int y = 0; y < readH; y++) {
        for (int x = 0; x < readW; x++) {
            uint32_t px = pixels[y * readW + x];
            if (px != 0) {
                anyNonZero = true;
                // You can break early if you want:
                // goto done_scanning;
                
            }
        }
    }

    //done_scanning:

    if (anyNonZero) {
        DEBUG_PRINT("At least one pixel is not ZERO.");
    } else {
        DEBUG_PRINT("No non-zero pixels.");
    }

    SDL_UnmapGPUTransferBuffer(gpu_device, dlnd_dst.transfer_buffer);


    DEBUG_PRINT("surface->w = " << surface->w);
    DEBUG_PRINT("surface->h = " << surface->h);
    DEBUG_PRINT("surface->pitch " << surface->pitch);
    DEBUG_PRINT("pixels_per_row " << surface->pitch / 4);

    return surface_texture;
}