#include "./../headers/Window/BaseSDL.h"
#include <SDL3/SDL_error.h>

int main(int argc, char* argv[])
{
    bool quit = false;

    try
    {
        BaseSDL b_sdl( SDL_INIT_VIDEO);

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
                b_sdl.draw();
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
