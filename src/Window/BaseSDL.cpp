#include "./../../headers/Window/BaseSDL.h"
#include <SDL3/SDL_video.h>

#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   800

BaseSDL::BaseSDL( Uint32 flags )
{
    if (!SDL_Init( flags ) != 0)
    {
        throw InitError();
    }
    m_window = SDL_CreateWindow("fortnite", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!m_window)
    {
        throw InitError();
    }
}

BaseSDL::~BaseSDL()
{
    SDL_DestroyWindow( m_window );
    SDL_Quit();
}

// TEMP FUNCTION - STAGE WILL DRAW
void BaseSDL::draw()
{
    
    
    
    // TEMP CODE


    // Clear the window with a black background
    //SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
    //SDL_RenderClear( m_renderer );

    int rgb[] = { 203, 203, 203, // Gray
                  254, 254,  31, // Yellow
                    0, 255, 255, // Cyan
                    0, 254,  30, // Green
                  255,  16, 253, // Magenta
                  253,   3,   2, // Red
                   18,  14, 252, // Blue
                    0,   0,   0  // Black
                };

    SDL_FRect colorBar;
    double barWidthRatio = 90.0/640.0;
    int barWidth = barWidthRatio * WINDOW_WIDTH;

    colorBar.x = 0; colorBar.y = 0; colorBar.w = barWidth; colorBar.h = WINDOW_HEIGHT;

    // Render a new color bar every 0.5 seconds

    //SDL_SetRenderDrawColor( m_renderer, rgb[0], rgb[0 + 1], rgb[0 + 2], 255 );
    //SDL_RenderFillRect( m_renderer, &colorBar );


    for ( int i = 0; i != sizeof rgb / sizeof * rgb; i += 3)
    {
        colorBar.x += barWidth;
    //    SDL_SetRenderDrawColor( m_renderer, rgb[i], rgb[i + 1], rgb[i + 2], 255 );
    //    SDL_RenderFillRect( m_renderer, &colorBar );
    }

    //SDL_RenderPresent( m_renderer );
    
}
