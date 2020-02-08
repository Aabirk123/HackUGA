#include<SDL2/SDL.h>
#include<stdlib.h>
#include<memory>

#include<cstdio>
#include<ctime>

#include "ball.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 600;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    // Setup the screen
    SDL_Window *Window = SDL_CreateWindow("BallGame",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);
    SDL_Surface* ScreenSurface = SDL_GetWindowSurface( Window );

    // Import image
    SDL_Surface* background = SDL_LoadBMP( "space.bmp" );

    SDL_Surface* crator1 = SDL_LoadBMP( "crater1.bmp" );
    SDL_Surface* crator2 = SDL_LoadBMP( "crater2.bmp" );
    SDL_Surface* crator3 = SDL_LoadBMP( "crater3.bmp" );
    SDL_Surface* crator4 = SDL_LoadBMP( "crater4.bmp" );

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    SDL_Rect C1[4] = {{60,440},
                      {200, 440},
                      {340, 440},
                      {480, 440}};
    
    int xVelo = 0;
    //int xtime = 0;
    int lvlDifficulty = 5;
    int score = 0;
    int frameCount = 0;

    std::unique_ptr<Ball> testBall[100];
    for(int i = 0; i <= lvlDifficulty; i++)
    {
        testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, (rand()%4)+1);
    }

    while( !quit )
    {
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
                        xVelo = -2;
                    break;

                    case SDLK_RIGHT:
                        xVelo = 2;
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
        frameCount++;
        if(frameCount % 2 == 0) {
            for(int i = 0; i <= lvlDifficulty; i++)
            {
                testBall[i]->Update(xVelo, 1);
                testBall[i]->Paste(ScreenSurface);
                int ballColor = testBall[i]->color;
                if(testBall[i]->touchingBox(C1[ballColor].x, C1[ballColor].y, 100, 100)) {
                    // Make new ball
                    testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, (rand()%4)+1);
                    score+=10;
                }

                // Offscreen
                if(testBall[i]->touchingBox(-100, SCREEN_HEIGHT, 1000, 10)) {
                    // Make new ball
                    testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, (rand()%4)+1);
                }
            }
        } else {
            for(int i = 0; i <= lvlDifficulty; i++)
            {
                testBall[i]->Paste(ScreenSurface);
            }
        }
        // Move player image's position

        //Apply the image
        SDL_BlitSurface(crator1, NULL, ScreenSurface, &C1[0]);
        SDL_BlitSurface(crator2, NULL, ScreenSurface, &C1[1]);
        SDL_BlitSurface(crator3, NULL, ScreenSurface, &C1[2]);
        SDL_BlitSurface(crator4, NULL, ScreenSurface, &C1[3]);
        
        //Update the surface
        SDL_UpdateWindowSurface( Window );
        SDL_Delay(5);
    }

	//Deallocate surface
	SDL_DestroyWindow( Window );

    return 0;
}