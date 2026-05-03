#include "./../../headers/Window/BaseSDL.h"
#include "../../headers/GPUUtils/imageloader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//for dogs
//#include <SDL3_image/SDL_image.h>


BaseSDL::BaseSDL( Uint32 flags )
{

    // CREATE DEVICE

    DEBUG_PRINT("DEVICE INIT START!");

    if(!SDL_Init(flags))
    {
        throw InitError();
    }
    DEBUG_PRINT("   SDL_INIT SUCCESS!");

    m_window = SDL_CreateWindow("Bold Game Engine", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!m_window)
    {
        throw InitError();
    }
    DEBUG_PRINT("   WINDOW CREATION SUCCESS!");
    
    if(SDL_GetNumGPUDrivers() == 0){
        throw InitError("NO GPU DRIVERS FOUND! TERMINATING!");
    }
    


    std::string default_driver =  SDL_GetGPUDriver(0);
    DEBUG_PRINT("YOUR DEFAULT DRIVER IS: " << default_driver);
    int shader_type = -1;
    std::string vert_shader_path = "";
    std::string frag_shader_path = "";
    std::string dog_path = "";
    
    if(default_driver == "vulkan"){
        shader_type = SDL_GPU_SHADERFORMAT_SPIRV;
        vert_shader_path = "../shaders/compiled/vulkan/textured.vert.spv";
        frag_shader_path = "../shaders/compiled/vulkan/textured.frag.spv";
        dog_path = "../resources/guy.png";
    }

    if(default_driver == "direct3d12"){
        shader_type = SDL_GPU_SHADERFORMAT_DXIL;
        vert_shader_path = "../../shaders/compiled/d3d12/TexturedQuad.vert.dxil";
        frag_shader_path = "../../shaders/compiled/d3d12/TexturedQuad.frag.dxil";
        dog_path = "../../resources/guy.png";
    }
    
    
    DEBUG_PRINT("CHOSEN VERTEX SHADER PATH: " << vert_shader_path);
    DEBUG_PRINT("CHOSEN FRAGMENT SHADER PATH: " << frag_shader_path);


    m_gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, true, NULL);


    if (!m_gpuDevice)
    {
        throw InitError();
    }
    DEBUG_PRINT("   GPU DEVICE CREATION SUCCESS!");

    if(!runInitTests()){
        DEBUG_PRINT("TESTS FAILED!");
        throw InitError();
    }
    

    if(!SDL_ClaimWindowForGPUDevice(m_gpuDevice, m_window)){
        throw InitError();
    }
    DEBUG_PRINT("   GPU BIND SUCCESS!");

    DEBUG_PRINT("DEVICE INIT SUCCESS!");

    // -----------------------------------------
    //          DEBUG - REMOVE LATER
    // -----------------------------------------
    //LOAD PNG
    DEBUG_PRINT("PNG LOADING START!");

    m_dogsTexture = UploadImage(dog_path.c_str(), m_gpuDevice);

    m_grassTexture = UploadImage("../../resources/grass.png", m_gpuDevice);

    m_boxTexture = UploadImage("../../resources/box.png", m_gpuDevice);
    m_boxRedTexture = UploadImage("../../resources/box_red.png", m_gpuDevice);
    
    SDL_GPUSamplerCreateInfo sampInfo{};
        sampInfo.min_filter = SDL_GPU_FILTER_LINEAR;
        sampInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
        sampInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;

        sampInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        sampInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        sampInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        sampInfo.mip_lod_bias = 0.0f;
        sampInfo.min_lod = 0.0f;
        sampInfo.max_lod = 1000.0f;

        sampInfo.enable_anisotropy = false;
        sampInfo.max_anisotropy = 1.0f;

        sampInfo.enable_compare = false;
        sampInfo.compare_op = SDL_GPU_COMPAREOP_ALWAYS;


        sampInfo.props = 0;

    m_dogsSampler = SDL_CreateGPUSampler(m_gpuDevice, &sampInfo);

    DEBUG_PRINT("GPU UPLOAD SUCCESS!");

    //printf("surface: %d x %d\n", surface->w, surface->h);

    //SDL_DestroySurface(surface);
    
    //LOAD SHADERS
    DEBUG_PRINT("SHADER LOADING START!");
    size_t file_size;

    void* vert_code = SDL_LoadFile(vert_shader_path.c_str(), &file_size);
    if (!vert_code){
        throw InitError();
    }
    DEBUG_PRINT("   VERTEX SHADER FILE FOUND!");

    SDL_GPUShaderCreateInfo vertInfo{};
        vertInfo.code_size = file_size;
        vertInfo.code = (Uint8*)vert_code;
        vertInfo.entrypoint = "main";
        vertInfo.format = shader_type;
        vertInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
        vertInfo.num_uniform_buffers = 1;

    SDL_GPUShader* vertex_shader = SDL_CreateGPUShader(m_gpuDevice, &vertInfo);

    if (!vertex_shader){
        throw InitError();
    }
    DEBUG_PRINT("   VERTEX SHADER CREATED!");

    void* frag_code = SDL_LoadFile(frag_shader_path.c_str(), &file_size);

    if (!frag_code){
        throw InitError();
    }
    DEBUG_PRINT("   FRAGMENT SHADER FILE FOUND!");

    
    SDL_GPUShaderCreateInfo fragInfo{};
        fragInfo.code_size = file_size;
        fragInfo.code = (Uint8*)frag_code;
        fragInfo.entrypoint = "main";
        fragInfo.format = shader_type;
        fragInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
        fragInfo.num_samplers = 1;

    SDL_GPUShader* fragment_shader = SDL_CreateGPUShader(m_gpuDevice, &fragInfo);
    
    if (!fragment_shader){
        throw InitError();
    }
    DEBUG_PRINT("   FRAGMENT SHADER CREATED!");

    SDL_free(vert_code);
    SDL_free(frag_code);

    DEBUG_PRINT("SHADERS LOADED!");

    //set up pipeline
    DEBUG_PRINT("PIPELINE CREATION START!");

    DEBUG_PRINT("   SET UP VERTEX ATTRIBUTES!");

    SDL_GPUVertexAttribute vertex_attributes[2]{};
        vertex_attributes[0].buffer_slot = 0;
        vertex_attributes[0].location = 0;
        vertex_attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
        vertex_attributes[0].offset = 0;
        DEBUG_PRINT("       POSITION SET UP!");

        vertex_attributes[1].buffer_slot = 0;
        vertex_attributes[1].location = 1;
        vertex_attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
        vertex_attributes[1].offset = sizeof(float) * 2;
        DEBUG_PRINT("       UV SET UP!");

    SDL_GPUVertexBufferDescription vertex_buffer_desc[1]{};
        vertex_buffer_desc[0].slot = 0;
        vertex_buffer_desc[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        vertex_buffer_desc[0].instance_step_rate = 0;
        vertex_buffer_desc[0].pitch = sizeof(float) * 4;
        DEBUG_PRINT("       BUFFER SET UP!");

    SDL_GPUVertexInputState vertex_input{};
        vertex_input.vertex_buffer_descriptions = vertex_buffer_desc;
        vertex_input.num_vertex_buffers = 1;
        vertex_input.vertex_attributes = vertex_attributes;
        vertex_input.num_vertex_attributes = 2;
        DEBUG_PRINT("       INPUT SET UP!");

    DEBUG_PRINT("   SET UP PIPELINE INFO!");
    SDL_GPUGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.vertex_shader = vertex_shader;
        pipeline_info.fragment_shader = fragment_shader;
        pipeline_info.vertex_input_state = vertex_input;
        pipeline_info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    
    DEBUG_PRINT("   SET UP COLOR TARGET INFO!");
    SDL_GPUColorTargetDescription color_target{};
        color_target.format = SDL_GetGPUSwapchainTextureFormat(m_gpuDevice, m_window);
        

    pipeline_info.target_info.num_color_targets = 1;
    pipeline_info.target_info.color_target_descriptions = &color_target;
    
    
    SDL_GPUGraphicsPipelineTargetInfo target_info{};
        target_info.color_target_descriptions = &color_target;
        target_info.num_color_targets = 1;
        target_info.has_depth_stencil_target = false;

    pipeline_info.target_info = target_info;

    color_target.format = SDL_GetGPUSwapchainTextureFormat(m_gpuDevice, m_window);

    color_target.blend_state.enable_blend = true;

    color_target.blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
    color_target.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;

    color_target.blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    color_target.blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    color_target.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    color_target.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;
    

    m_pipeline = SDL_CreateGPUGraphicsPipeline(m_gpuDevice, &pipeline_info);
    
    if (!m_pipeline){
        throw InitError();
    }
    DEBUG_PRINT("PIPELINE CREATED SUCCESSFULLY!");


    //create GPU vertex buffer

    DEBUG_PRINT("CREATING AND UPLOADING VERTEX BUFFER!");
    float quad_verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, 
        1.0f, -1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 1.0f, 0.0f, 

        -1.0f, -1.0f, 0.0f, 1.0f, 
        1.0f, 1.0f, 1.0f, 0.0f, 
        -1.0f, 1.0f, 0.0f, 0.0f, 
    };
    Uint32 quadSize = sizeof(quad_verts);

    SDL_GPUBufferCreateInfo vbufInfo{};
        vbufInfo.size = sizeof(quad_verts);
        vbufInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        vbufInfo.props = 0;

    m_quadBuffer = SDL_CreateGPUBuffer(m_gpuDevice, &vbufInfo);


    SDL_GPUTransferBufferCreateInfo tinfo{};
        tinfo.size  = quadSize;
        tinfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    
    SDL_GPUTransferBuffer* quad_staging = SDL_CreateGPUTransferBuffer(m_gpuDevice, &tinfo);
    if (!quad_staging) {
        throw InitError();
    }
    DEBUG_PRINT("QUAD STAGED SUCCESSFULLY!");

    void* ptr = SDL_MapGPUTransferBuffer(m_gpuDevice, quad_staging, false);
    memcpy(ptr, quad_verts, quadSize);
    SDL_UnmapGPUTransferBuffer(m_gpuDevice, quad_staging);

    SDL_GPUCommandBuffer* quad_cmd = SDL_AcquireGPUCommandBuffer(m_gpuDevice);
    SDL_GPUCopyPass* quad_copy = SDL_BeginGPUCopyPass(quad_cmd);


    SDL_GPUTransferBufferLocation quad_src{};
        quad_src.transfer_buffer = quad_staging;
        quad_src.offset = 0;

    SDL_GPUBufferRegion quad_dst{};
        quad_dst.buffer = m_quadBuffer;
        quad_dst.offset = 0;
        quad_dst.size   = quadSize;


    SDL_UploadToGPUBuffer(quad_copy, &quad_src, &quad_dst, false);

    SDL_EndGPUCopyPass(quad_copy);
    SDL_SubmitGPUCommandBuffer(quad_cmd);

    /*
    void* temp_ptr = SDL_MapGPUTransferBuffer(m_gpuDevice, quad_staging, false);
    float* f = (float*)temp_ptr;

    for (int i = 0; i < 24; i++) {
        printf("%f\n", f[i]);
    }

    SDL_UnmapGPUTransferBuffer(m_gpuDevice, quad_staging);
    */

    
    

    DEBUG_PRINT("CREATING AND UPLOADING VERTEX BUFFER SUCCESS!");
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
        DEBUG_PRINT("D3D12 shaders and Vulkan shaders supported!");
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

    SDL_BindGPUGraphicsPipeline(test_rp, m_pipeline);

    SDL_GPUBufferBinding vbind{};
    vbind.buffer = m_quadBuffer;
    vbind.offset = 0;
    SDL_BindGPUVertexBuffers(test_rp, 0, &vbind, 1);

    glm::mat4 model = glm::mat4(1.0f);   // identity
    SDL_GPUTextureSamplerBinding binding{};
        binding.texture = m_grassTexture;
        binding.sampler = m_dogsSampler;

    //printf("tex = %p, sampler = %p\n", (void*)m_dogsTexture, (void*)m_dogsSampler);


    SDL_BindGPUFragmentSamplers(test_rp, 0, &binding, 1);
    SDL_PushGPUVertexUniformData(test_cmdbuff, 0, &model, sizeof(glm::mat4));

    SDL_DrawGPUPrimitives(test_rp, 6, 1, 0, 0);

    // DRAW BOX

    glm::mat4 box_model(1.0f);
        box_model = glm::translate(box_model, glm::vec3(500, 500, 0.0f));
        box_model = glm::scale(box_model, glm::vec3(64, 64, 1.0f));

        glm::mat4 box_ortho = glm::ortho(
            0.0f, (float)WINDOW_WIDTH,
            (float)WINDOW_HEIGHT, 0.0f,
            -1.0f, 1.0f
        );

    glm::mat4 box_mvp = box_ortho * box_model;

    SDL_BindGPUVertexBuffers(test_rp, 0, &vbind, 1);

    if(box_collide){
        binding.texture = m_boxRedTexture;
    }
    else{
        binding.texture = m_boxTexture;
    }
    
    binding.sampler = m_dogsSampler;

    //printf("tex = %p, sampler = %p\n", (void*)m_dogsTexture, (void*)m_dogsSampler);

    
    SDL_BindGPUFragmentSamplers(test_rp, 0, &binding, 1);
    SDL_PushGPUVertexUniformData(test_cmdbuff, 0, &box_mvp, sizeof(glm::mat4));

    SDL_DrawGPUPrimitives(test_rp, 6, 1, 0, 0);

    //DRAW PERSON

    SDL_BindGPUVertexBuffers(test_rp, 0, &vbind, 1);


    binding.texture = m_dogsTexture;
    binding.sampler = m_dogsSampler;

    //printf("tex = %p, sampler = %p\n", (void*)m_dogsTexture, (void*)m_dogsSampler);

    
    SDL_BindGPUFragmentSamplers(test_rp, 0, &binding, 1);
    SDL_PushGPUVertexUniformData(test_cmdbuff, 0, &m_mvp, sizeof(glm::mat4));

    SDL_DrawGPUPrimitives(test_rp, 6, 1, 0, 0);


    SDL_EndGPURenderPass(test_rp);
    SDL_SubmitGPUCommandBuffer(test_cmdbuff);
}
