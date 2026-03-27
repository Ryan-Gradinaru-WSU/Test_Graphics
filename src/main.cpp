#include "./../headers/Window/BaseSDL.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

int main(int argc, char* argv[])
{
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_TRACE);

    bool quit = false;

    DEBUG_PRINT("SDL3 lib path: " << SDL_GetBasePath());

    
    try
    {
        BaseSDL b_sdl = BaseSDL(SDL_INIT_VIDEO);


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
                
                
            }
            //TEMP REMOVE
            b_sdl.draw();
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
