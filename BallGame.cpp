#include<SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include<stdlib.h>
#include<memory>
#include<cstdio>
#include<ctime>

#include "ball.h"
#include "crater.h"
#include "powerup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 600;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    //TTF_Init();
    // Setup the screen
    SDL_Window *Window = SDL_CreateWindow("BallGame",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);
    SDL_Surface* ScreenSurface = SDL_GetWindowSurface( Window );
    //TTF_Font * font = TTF_OpenFont("Arial.ttf", 25);

    // Import image
    SDL_Surface* background = SDL_LoadBMP( "space.bmp" );

    SDL_Surface* powerUp1 = SDL_LoadBMP("powerup");
    SDL_Surface* powerUp2 = SDL_LoadBMP("powerup2");
    SDL_Surface* powerUp3 = SDL_LoadBMP("powerup3");

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    Crater* crater[4];
    for(int i=0; i<4; i++) {
        crater[i] = new Crater(64+i*128, 420, i+1);
    }
    
    int xVelo = 0;
    int lvlDifficulty = 5;
    int score = 0;
    SDL_Color color = {255, 255, 255};
    //SDL_Surface * scoreSurface = TTF_RenderText_Solid(font, "Score: 0", color);
    int frameCount = 0;

    std::unique_ptr<Ball> testBall[100];
    std::unique_ptr<powerUp> testPower[100];
    for(int i = 0; i <= lvlDifficulty; i++)
    {
        testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, (rand()%4)+1);
    }
    for(int j = 0; j <= 3; j++)
    {
        testPower[j] = std::make_unique<powerUp>(100+(rand()%9)*50, (rand()%3)+1);
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
                        if(xVelo < 0)
                            xVelo = 0;
                    break;

                    case SDLK_RIGHT:
                        if(xVelo > 0)
                            xVelo = 0;
                    break;
                }
            }
        }

        SDL_BlitSurface( background, NULL, ScreenSurface, NULL );
        // Moving planets
        for(int i = 0; i < 4; i++)
        {
            crater[i]->Update(xVelo, 0);
            crater[i]->CheckOffscreen(SCREEN_WIDTH);
        }
        //Power Ups
        /*
            int l = 0;
            int timer = SDL_geticks();
            if(timer % 10000 == 0)
            {
                testPower[l]->Paste(ScreenSurface)
            }
            if(l>)
        
    */
        // Moving balls
        frameCount++;
        if(frameCount % 2 == 0) {
            for(int i = 0; i <= lvlDifficulty; i++)
            {
                testBall[i]->Update(0, 1);
                testBall[i]->Paste(ScreenSurface);
                int ballColor = testBall[i]->color;
                if(testBall[i]->touchingBox(crater[ballColor]->x, crater[ballColor]->y, 100, 100)) {
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
        for(int i=0; i<4; i++) {
            crater[i]->Paste(ScreenSurface);
        }
        
        SDL_BlitSurface( scoreSurface, NULL, ScreenSurface, NULL );
        //Update the surface
        SDL_UpdateWindowSurface( Window );
        SDL_Delay(5);
    }

	//Deallocate surface
    for(int i=0; i<4; i++) {
        delete(crater[i]);
    }
	SDL_DestroyWindow( Window );
    //TTF_CloseFont(font);
    //TTF_Quit();
    SDL_Quit();

    return 0;
}
