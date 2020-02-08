#include<SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[])
{

    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    // Setup the screen
    SDL_Window *Screen = SDL_CreateWindow(name,
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);
    SDL_Renderer* renderer = SDL_CreateRenderer(Screen, -1, 0);
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    while(Stopper)
    {
        // Clear screen
        SDL_RenderClear(renderer);
    
        // Paste this frame onto the game
        SDL_RenderPresent(renderer);
    }

    exit(0);
}