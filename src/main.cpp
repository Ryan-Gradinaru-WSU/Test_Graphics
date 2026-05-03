#include "./../headers/Window/BaseSDL.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

int main(void/*int argc, char* argv[]*/)
{
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_TRACE);

    bool quit = false;

    DEBUG_PRINT("SDL3 lib path: " << SDL_GetBasePath());

    float mouse_x = 0;
    float mouse_y = 0;

    float w_pressed = 0;
    float a_pressed = 0;
    float s_pressed = 0;
    float d_pressed = 0;

    
    try
    {
        BaseSDL b_sdl = BaseSDL(SDL_INIT_VIDEO);

        uint64_t lastTime = SDL_GetTicks();
        uint64_t frames = 0;
        float fps = 0.0f;


        while(!quit)
        {
            frames++;

            uint64_t now = SDL_GetTicks();
            uint64_t delta = now - lastTime;
         
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
                    case SDLK_W:
                        w_pressed = 1;
                        break;
                    case SDLK_A:
                        a_pressed = 1;
                        break;
                    case SDLK_S:
                        s_pressed = 1;
                        break;
                    case SDLK_D:
                        d_pressed = 1;
                        break;
                    }
                }

                else if(e.type == SDL_EVENT_KEY_UP)
                {
                    switch (e.key.key)
                    {
                    case SDLK_W:
                        w_pressed = 0;
                        break;
                    case SDLK_A:
                        a_pressed = 0;
                        break;
                    case SDLK_S:
                        s_pressed = 0;
                        break;
                    case SDLK_D:
                        d_pressed = 0;
                        break;
                    }
                }

                else if (e.type == SDL_EVENT_MOUSE_MOTION){
                    mouse_x = e.motion.x;
                    mouse_y = e.motion.y;
                }

                //DEBUG_PRINT(mouse_x << " " << mouse_y);
                
                
            }

            b_sdl.m_ty += w_pressed * -3.0f;
            b_sdl.m_tx += a_pressed * -3.0f;
            b_sdl.m_ty += s_pressed * 3.0f;
            b_sdl.m_tx += d_pressed * 3.0f;
            
            if(b_sdl.m_ty - (b_sdl.m_scaleY) < 0){
                b_sdl.m_ty = b_sdl.m_scaleY;
            }
            if(b_sdl.m_tx - (b_sdl.m_scaleX) < 0){
                b_sdl.m_tx = b_sdl.m_scaleX;
            }
            if(b_sdl.m_ty + (b_sdl.m_scaleY) > WINDOW_HEIGHT){
                b_sdl.m_ty = WINDOW_HEIGHT - b_sdl.m_scaleY;
            }
            if(b_sdl.m_tx + (b_sdl.m_scaleX) > WINDOW_WIDTH){
                b_sdl.m_tx = WINDOW_WIDTH - b_sdl.m_scaleX;
            }
            
            
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(b_sdl.m_tx, b_sdl.m_ty, 0.0f));
            model = glm::rotate(model, std::atan2(mouse_y - b_sdl.m_ty, mouse_x - b_sdl.m_tx), glm::vec3(0,0,1));
            model = glm::scale(model, glm::vec3(b_sdl.m_scaleX, b_sdl.m_scaleY, 1.0f));

            glm::mat4 ortho = glm::ortho(
                0.0f, (float)WINDOW_WIDTH,
                (float)WINDOW_HEIGHT, 0.0f,
                -1.0f, 1.0f
            );

            b_sdl.m_mvp = ortho * model;

            
            float deltaX = std::abs(b_sdl.m_tx - 500);
            float deltaY = std::abs(b_sdl.m_ty - 500);

            float sumHalfWidths = b_sdl.m_scaleX + 34;
            float sumHalfHeights = b_sdl.m_scaleY + 34; 
                

            b_sdl.box_collide = (deltaX < sumHalfWidths) && (deltaY < sumHalfHeights);

            //TEMP REMOVE
            b_sdl.draw();

            if (delta >= 1000)  // one second passed
            {
                fps = frames * 1000.0f / delta;
                frames = 0;
                lastTime = now;

                printf("FPS: %.2f\n", fps);
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
