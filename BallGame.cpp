#include<SDL2/SDL.h>
#include<cstdio>
#include "ball.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[])
{
    //srand(time(NULL));
    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    // Setup the screen
    SDL_Window *Window = SDL_CreateWindow("BallGame",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);
    SDL_Surface* ScreenSurface = SDL_GetWindowSurface( Window );

    // Import image
    SDL_Surface* gXOut = SDL_LoadBMP( "byte.bmp" );
    SDL_Surface* background = SDL_LoadBMP( "space.bmp" );

    SDL_Surface* crator = SDL_LoadBMP( "crater1.bmp" );
    SDL_Surface* crator2 = SDL_LoadBMP( "crater2.bmp" );
    SDL_Surface* crator3 = SDL_LoadBMP( "crater3.bmp" );
    SDL_Surface* crator4 = SDL_LoadBMP( "crater4.bmp" );
	if( gXOut == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "03_event_driven_programming/x.bmp", SDL_GetError() );
        return 0;
	}

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    SDL_Rect dest = {200, 300};
    SDL_Rect C1 = {};
    SDL_Rect C2 = {};
    SDL_Rect C3 = {};
    SDL_Rect C4 = {};
    
    int xVelo = 0;
    //int xtime = 0;
    int lvlDifficulty = 2;

    Ball* testBall[100];
    for(int i = 0; i <= lvlDifficulty; i++)
    {
        testBall[i] = new Ball(200+i*50, i);
    }

    while( !quit )
    {
        //Timer for dropping fall
        /*int timePassed = SDL_GetTicks();
        if(timePassed % 600 == 0)
        {
            xtime = -1;
        }*/
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            } //User presses a key
            else if( e.type == SDL_KEYDOWN )
            {
                switch( e.key.keysym.sym )
                {
                    case SDLK_ESCAPE:
                        quit = true;
                    break;

                    case SDLK_LEFT:
                        xVelo = -1;
                    break;

                    case SDLK_RIGHT:
                        xVelo = 1;
                    break;
                }
            } else if( e.type == SDL_KEYUP) {
                switch( e.key.keysym.sym )
                {
                    case SDLK_LEFT:
                        xVelo = 0;
                    break;

                    case SDLK_RIGHT:
                        xVelo = 0;
                    break;
                }
            }
        }

        SDL_BlitSurface( background, NULL, ScreenSurface, NULL );
        for(int i = 0; i <= lvlDifficulty; i++)
        {
            testBall[i]->Update(1);
            testBall[i]->Paste(ScreenSurface);
        }
        // Move player image's position
        dest.x += xVelo;
        //Apply the image
        SDL_BlitSurface( gXOut, NULL, ScreenSurface, &dest );
    
        //Update the surface
        SDL_UpdateWindowSurface( Window );
        SDL_Delay(5);
    }

	//Deallocate surface
	SDL_FreeSurface( gXOut );
	SDL_DestroyWindow( Window );

    return 0;
}
