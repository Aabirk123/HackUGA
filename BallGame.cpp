#include<SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<stdlib.h>
#include<iostream>
#include<memory>
#include<cstdio>
#include<ctime>

#include "ball.h"
#include "crater.h"
#include "powerup.h"
#include "ExplosionManager.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    // Setup the screen
    SDL_Window *Window = SDL_CreateWindow("BallGame",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);
    SDL_Surface* ScreenSurface = SDL_GetWindowSurface( Window );
    TTF_Font * font = TTF_OpenFont("arial.ttf", 25);

    // Import image
    SDL_Surface* background = SDL_LoadBMP( "space.bmp" );
    SDL_Surface* Earth = SDL_LoadBMP("earth.bmp");
    SDL_Surface* Lose = SDL_LoadBMP("lose.bmp");
    //SDL_Rect earthPos = {0, SCREEN_HEIGHT-90};
    int earthX = 0;

    //Main loop flag
    bool quit = false;
    bool playing = true;

    //Event handler
    SDL_Event e;

    Crater* crater[4];
    for(int i=0; i<4; i++) {
        crater[i] = new Crater(i*(SCREEN_WIDTH+100)/4, 350, i+1);
    }
    int speedIncrease = 2;
    int updateSpeed = 1;
    int baseSlowness = 2;
    int lvlDifficulty = 3;

    SDL_Color color = {255, 255, 255};
    SDL_Surface * scoreSurface = TTF_RenderText_Solid(font, "Score: 0", color);

    ExplosionManager explosions;
    std::unique_ptr<Ball> testBall[100];
    std::unique_ptr<powerUp> testPower;

    while(playing) {
        int combo = 0;
        int collisions = 0;
        int timeSlow = baseSlowness;
        int xVelo = 0;
        int score = 0;
        std::string scoreStr = "Score: " + std::to_string(score);
        scoreSurface = TTF_RenderText_Solid(font, scoreStr.c_str(), color);

        int frameCount = 0; int powerCount = 0;
        int currentFrame = 0;
        quit = false;

        for(int i = 0; i <= lvlDifficulty; i++)
        {
            testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, -10-(rand()%9)*50, (rand()%4)+1);
        }

        while( !quit )
        {
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                    playing = false;
                } //User presses a key
                else if( e.type == SDL_KEYDOWN )
                {
                    switch( e.key.keysym.sym )
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            playing = false;
                        break;

                        case SDLK_LEFT:
                            xVelo = -speedIncrease;
                        break;

                        case SDLK_RIGHT:
                            xVelo = speedIncrease;
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
            // Earth SHAKING aftre being hit
            if(earthX != 0 && frameCount %2 == 0) {
                if(earthX > 0)
                    earthX = -earthX+1;
                else if(earthX < 0) {
                    earthX = -earthX-1;
                }
            }
            SDL_Rect earthPos = {earthX, SCREEN_HEIGHT-80};
            SDL_BlitSurface( Earth, NULL, ScreenSurface, &earthPos);
            // Moving planets
            for(int i = 0; i < 4; i++)
            {
                crater[i]->Update(xVelo, 0);
                crater[i]->CheckOffscreen(SCREEN_WIDTH);
                crater[i]->Paste(ScreenSurface);
            }

            // Increases amount of lives
            if(powerCount %10000 == 0 && collisions != 0)
            {
                collisions--;

            }
            //Power Ups
            powerCount++;
            if(powerCount % 3000 == 0)
            {
                // Make a new powerup
                testPower = std::make_unique<powerUp>(100+(rand()%9)*50, (rand()%3)+1);
            }
            if(testPower != NULL) {
                testPower->update(1);
                testPower->Paste(ScreenSurface);

                bool collided = false;
                for(int i = 0; i< 4; i++ )
                {
                    if(testPower -> touchingBox(crater[i]->x, crater[i]->y, 50, 50)) {
                        collided = true;
                    }
                }

                if(collided)
                {
                    if(testPower-> whichOne == 0)
                    {
                        timeSlow+=5;
                        currentFrame = 1;
                    }
                    else if(testPower-> whichOne == 1)
                    {
                        for(int i = 0; i<=lvlDifficulty; i++)
                        {
                            explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                            //makes new ball set erasing the old one
                            testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, -10-(rand()%9)*50, (rand()%4)+1);
                            score+=10*lvlDifficulty;
                            std::string scoreStr = "Score: " + std::to_string(score);
                            scoreSurface = TTF_RenderText_Solid(font, scoreStr.c_str(), color);
                        }
                    }
                    else if(testPower-> whichOne == 2)
                    {
                        for (int i = 0; i <= lvlDifficulty; i++)
                        {
                            testBall[i]->changeColor(5);
                        }
                    }
                    testPower = NULL;
                }
            }
            if(currentFrame % 1000 == 0 && timeSlow != baseSlowness)
            {
                timeSlow = baseSlowness;
            }

        
            // Moving balls
            frameCount++;
            currentFrame++;
            if(frameCount % timeSlow == 0) {
                for(int i = 0; i <= lvlDifficulty; i++)
                {
                    testBall[i]->Update(0, updateSpeed);
                    testBall[i]->Paste(ScreenSurface);
                    int ballColor = testBall[i]->color;
                    if(ballColor < 4) {
                        if(testBall[i]->touchingBox(crater[ballColor]->x, crater[ballColor]->y, 50, 50)){
                            explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                            // Make new ball
                            testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, -10-(rand()%9)*50, (rand()%4)+1);
                            score+=10;
                            std::string scoreStr = "Score: " + std::to_string(score);
                            scoreSurface = TTF_RenderText_Solid(font, scoreStr.c_str(), color);
                            combo++;
                        }
                    } else {
                        for(int j=0; j<4; j++) {
                            if(testBall[i]->touchingBox(crater[j]->x, crater[j]->y, 50, 50)) {
                                explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                                testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, -10-(rand()%9)*50, (rand()%4)+1);
                                score+=10;
                                std::string scoreStr = "Score: " + std::to_string(score);
                                scoreSurface = TTF_RenderText_Solid(font, scoreStr.c_str(), color);
                                combo++;
                            }
                        }
                    }

                    // Offscreen
                    if(testBall[i]->touchingBox(-1000, SCREEN_HEIGHT-50, 2000, 50)) {
                        explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                        earthX = 30;
                        // Make new ball
                        testBall[i] = std::make_unique<Ball>(100+(rand()%9)*50, -10-(rand()%9)*50, (rand()%4)+1);
                        collisions++;
                        combo = 0;
                    }

                    //Game over man, game over!
                    if(collisions ==  3)
                    {
                        // game over screen
                        quit = true;
                    }
                }
            } else {
                for(int i = 0; i <= lvlDifficulty; i++)
                {
                    testBall[i]->Paste(ScreenSurface);
                }
            }
            explosions.Update(frameCount);
            explosions.PasteAll(ScreenSurface);

            SDL_BlitSurface( scoreSurface, NULL, ScreenSurface, NULL );
            //Update the surface
            SDL_UpdateWindowSurface( Window );
            SDL_Delay(5);
            //updates difficulty 
            if(score >= 200) {
                speedIncrease++;
                lvlDifficulty+=1;
                updateSpeed+=3;
                baseSlowness+=2;
                score = 0;
                quit = true;
            }
        }

        if(collisions < 3 && playing) {
            SDL_Surface * cleared = TTF_RenderText_Solid(font, "Level Cleared!", color);
            SDL_Rect dest = {SCREEN_WIDTH/2-140/2, SCREEN_HEIGHT/2};
            SDL_BlitSurface( cleared, NULL, ScreenSurface, &dest );
            SDL_UpdateWindowSurface( Window );
            SDL_Delay(5000);
        }
        if(collisions >= 3) {
            // Game over screen
            SDL_Color gameOverColor = {255, 180, 180};
            SDL_Surface * gameOver = TTF_RenderText_Solid(font, "GAME OVER", gameOverColor);
            SDL_Rect destA = {SCREEN_WIDTH/2-110/2, SCREEN_HEIGHT/2-30};

            SDL_Surface * gameOverRestart = TTF_RenderText_Solid(font, "Press enter to restart", color);
            SDL_Rect destB = {SCREEN_WIDTH/2-200/2, SCREEN_HEIGHT/2};

            quit = false;
            while(!quit) {
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                        playing = false;
                    } //User presses a key
                    if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_ESCAPE:
                                quit = true;
                                playing = false;
                            break;
                            case SDLK_RETURN:
                                quit = true;
                            break;
                        }
                    }
                }
                SDL_BlitSurface( gameOver, NULL, ScreenSurface, &destA );
                SDL_BlitSurface( gameOverRestart, NULL, ScreenSurface, &destB );
                SDL_UpdateWindowSurface( Window );
                SDL_Delay(5);
            }
        }

        explosions.reset();
    }

	//Deallocate surface
    for(int i=0; i<4; i++) {
        delete(crater[i]);
    }
    SDL_DestroyWindow( Window );
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
