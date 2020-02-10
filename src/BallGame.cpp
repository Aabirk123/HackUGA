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
#include "boss.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void makeNewBall(std::unique_ptr<Ball> testBall[], int target, int minDist) {
    // Max Y
    int maxY = SCREEN_HEIGHT;
    for(int i=0; testBall[i] != NULL; i++) {
        if(testBall[i]->y < maxY)
            maxY = testBall[i]->y;
    }
    testBall[target] = std::make_unique<Ball>(100+(rand()%9)*50, maxY-minDist-(rand()%10)*20, (rand()%4)+1);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    // Setup the screen
    SDL_Window *Window = SDL_CreateWindow("SPACE-BYTE",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    SDL_Surface* ScreenSurface = SDL_GetWindowSurface( Window );
    TTF_Font * font = TTF_OpenFont("arial.ttf", 25);

    // Import image
    SDL_Surface* background = SDL_LoadBMP( "space.bmp" );
    SDL_Surface* Earth = SDL_LoadBMP("earth.bmp");
    SDL_Surface* life = SDL_LoadBMP("life.bmp");
    SDL_Surface* byte = SDL_LoadBMP( "byte.bmp" );
    //SDL_Rect earthPos = {0, SCREEN_HEIGHT-90};
    int earthX = 0;
    int spaceX = 0;

    //Main loop flag
    bool quit = false;
    bool playing = true;

    //Event handler
    SDL_Event e;

    Crater* crater[4];
    for(int i=0; i<4; i++) {
        crater[i] = new Crater(i*(SCREEN_WIDTH+100)/4, 340, i+1);
    }
    int updateSpeed = 1;
    int speedIncrease = 2;
    int baseSlowness = 2;
    int level = 0;
    int lvlDifficulty = 3;
    int minDist = 50;

    SDL_Color color = {255, 255, 255};
    SDL_Surface * scoreSurface = TTF_RenderText_Solid(font, "Score: 0", color);
    SDL_Surface * ready = TTF_RenderText_Solid(font, "Giga-Byte Ready!", color);
    SDL_Surface * pressButton = TTF_RenderText_Solid(font, "Press Any Button!", color);
    SDL_Surface * comboSurface;

    ExplosionManager explosions;
    std::unique_ptr<Ball> testBall[100];
    std::unique_ptr<powerUp> testPower;
    std::unique_ptr<Boss> boss;

    while(playing) {
        int combo = 0;
        int collisions = 0;
        int timeSlow = baseSlowness;
        int xVelo = 0;
        int score = 0;
        int speedBoost = 0;

        int frameCount = 0; int powerCount = 0;
        int currentFrame = 0;
        quit = false;

        testPower = NULL;
        // Make first ball at top of screen
        makeNewBall(testBall, 0, minDist); testBall[0]->y = -20;
        // Make all other balls
        for(int i = 1; i <= lvlDifficulty; i++)
        {
            makeNewBall(testBall, i, minDist);
        }
        
        // Boss level stuff
        if(level == 4) {
            boss = std::make_unique<Boss>(SCREEN_WIDTH/2-100, -200, SCREEN_HEIGHT/5);
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
                            printf("In-game Escape\n");
                            quit = true;
                            playing = false;
                        break;

                        case SDLK_LEFT:
                            xVelo = -speedIncrease;
                        break;

                        case SDLK_RIGHT:
                            xVelo = speedIncrease;
                        break;
                        
                        case SDLK_DOWN:
                            speedBoost = 2;
                        break;

                        default:
                        if(combo >= 10 )
                        {
                            combo = 0;
                            for(int i = 0; i<=lvlDifficulty; i++)
                            {
                                if(testBall[i]->y > -20) {
                                    explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                                    //makes new ball set erasing the old one
                                    makeNewBall(testBall, i, minDist);
                                    score+=10;
                                }
                            }
                            spaceX = 20;
                        }
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

                        case SDLK_DOWN:
                            speedBoost = 0;
                        break;
                    }
                }
            }

            if(spaceX != 0 && frameCount % 10 == 0) {
                if(spaceX > 0)
                    spaceX = -spaceX+1;
                else if(spaceX < 0) {
                    spaceX = -spaceX-1;
                }
            }
            SDL_Rect spacePos = {spaceX, 0};
            SDL_BlitSurface( background, NULL, ScreenSurface, &spacePos );
            // Earth SHAKING aftre being hit
            if(earthX != 0 && frameCount %  2 == 0) {
                if(earthX > 0)
                    earthX = -earthX+1;
                else if(earthX < 0) {
                    earthX = -earthX-1;
                }
            }
            SDL_Rect earthPos = {earthX+spaceX, SCREEN_HEIGHT-70};
            SDL_BlitSurface( Earth, NULL, ScreenSurface, &earthPos);
            // Moving planets
            for(int i = 0; i < 4; i++)
            {
                crater[i]->Update(xVelo, 0);
                crater[i]->CheckOffscreen(SCREEN_WIDTH);
                crater[i]->Paste(ScreenSurface, spaceX);
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
                testPower->update(updateSpeed+speedBoost);
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
                            if(testBall[i]->y > -20) {
                                explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                                //makes new ball set erasing the old one
                                makeNewBall(testBall, i, minDist);
                                score+=10;
                            }
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
                    testBall[i]->Update(0, updateSpeed+speedBoost);
                    testBall[i]->Paste(ScreenSurface);
                    int ballColor = testBall[i]->color;
                    if(ballColor < 4) {
                        if(testBall[i]->touchingBox(crater[ballColor]->x, crater[ballColor]->y, 50, 50)){
                            explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                            // Make new ball
                            makeNewBall(testBall, i, minDist);
                            score+=10;
                            combo++;
                        }
                    } else {
                        for(int j=0; j<4; j++) {
                            if(testBall[i]->touchingBox(crater[j]->x, crater[j]->y, 50, 50)) {
                                explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                                makeNewBall(testBall, i, minDist);
                                score+=10;
                                combo++;
                            }
                        }
                    }

                    // Offscreen
                    if(testBall[i]->touchingBox(-1000, SCREEN_HEIGHT-50, 2000, 50)) {
                        explosions.add(testBall[i]->x, testBall[i]->y, frameCount);
                        earthX = 30;
                        // Make new ball
                        makeNewBall(testBall, i, minDist);
                        collisions++;
                        combo = 0;
                    }

                    //Game over man, game over!
                    if(collisions ==  3)
                    {
                        // game over screen
                        quit = true;
                        spaceX = 0;
                        earthX = 0;
                    }
                }
            } else {
                for(int i = 0; i <= lvlDifficulty; i++)
                {
                    testBall[i]->Paste(ScreenSurface);
                }
            }
            explosions.Update(frameCount);
            explosions.PasteAll(ScreenSurface, spaceX);
            
            // Boss level stuff
            if(level == 4) {
                boss->Update();
                boss->Paste(ScreenSurface);

                if(frameCount % 700 == 0 && lvlDifficulty < 20) {
                    lvlDifficulty++;
                    testBall[lvlDifficulty] = std::make_unique<Ball>(boss->x+(rand()%150)+10, boss->y, boss->color);
                }
                for(int i=0; i<=lvlDifficulty; i++) {
                    if(testBall[i]->y < 0) {
                        testBall[i]->x = boss->x+(rand()%150)+10;
                        testBall[i]->y = boss->y;
                    }
                    if(testBall[i]->y < boss->y && testBall[i]->x > boss->x && testBall[i]->x < boss->x+200) {
                        testBall[i]->x = boss->x+(rand()%150)+10;
                        testBall[i]->y = boss->y;
                        testBall[i]->changeColor(boss->color);
                    }
                    if(testBall[i]->color != boss->color && testBall[i]->touchingBox(boss->x, boss->y, 200, 120)) {
                        testBall[i]->changeColor(boss->color+1);
                    }
                }
            }

            // Byte
            SDL_Rect byteClip = {1,1,56,71};
            if(score >= 200+level*100) {
                byteClip.x = 117;
            } else if(spaceX != 0) {
                byteClip.x = 175;
            } else if(earthX != 0) {
                byteClip.x = 233;
            } else if(frameCount % 1000 < 800) {
                byteClip.x = 1;
            } else {
                byteClip.x = 59;
            }
            SDL_Rect bytePos = {SCREEN_WIDTH/2-33+earthX+spaceX, SCREEN_HEIGHT-95};
            SDL_BlitSurface( byte, &byteClip, ScreenSurface, &bytePos);

            // Lives
            for(int i=collisions; i<3; i++) {
                SDL_Rect dest = {10+(i-collisions)*60, SCREEN_HEIGHT-60};
                SDL_BlitSurface( life, NULL, ScreenSurface, &dest );
            }
            // Score text
            std::string scoreStr = "Score: " + std::to_string(score) + "/" + std::to_string(200+level*100);
            scoreSurface = TTF_RenderText_Solid(font, scoreStr.c_str(), color);
            SDL_BlitSurface( scoreSurface, NULL, ScreenSurface, NULL );
            // Combo text
            if(combo >= 10 )
            {
                SDL_Rect dest = {SCREEN_WIDTH-200, 0};
                SDL_BlitSurface( ready, NULL, ScreenSurface, &dest );

                SDL_Rect dest2 = {SCREEN_WIDTH-195, 25};
                SDL_BlitSurface( pressButton, NULL, ScreenSurface, &dest2 );
                SDL_UpdateWindowSurface( Window );
            } else if(combo > 1) {
                std::string comboMessage = std::to_string(combo) + "x combo!";
                comboSurface = TTF_RenderText_Solid(font, comboMessage.c_str(), color);
                SDL_Rect dest = {SCREEN_WIDTH-170, 0};
                SDL_BlitSurface( comboSurface, NULL, ScreenSurface, &dest );
                SDL_UpdateWindowSurface( Window );
            }
            //Update the surface
            SDL_UpdateWindowSurface( Window );
            SDL_Delay(5);
            //updates difficulty 
            if(score >= 200+level*100) {
                level++;
                lvlDifficulty+=1;
                if(speedIncrease < 4)
                    speedIncrease++;
                if(minDist > 0)
                    minDist -= 5;
                if(lvlDifficulty % 3 == 0) {
                    updateSpeed+=1;
                }
                if(level == 4) {
                    updateSpeed--;
                }
                //baseSlowness+=1;
                score = 0;
                quit = true;
            }
        } // End regular game loop

        if(level == 5) {
            SDL_Surface* deadBoss = SDL_LoadBMP( "bossexplosion.bmp" );
            SDL_Rect ded = {boss->x, boss->y};
            SDL_Surface * cleared = TTF_RenderText_Solid(font, "You saved the earth!", color);
            SDL_Rect dest = {SCREEN_WIDTH/2-150/2, SCREEN_HEIGHT/2};
            SDL_Surface * thanks = TTF_RenderText_Solid(font, "Thank you for playing!", color);
            SDL_Rect destB = {SCREEN_WIDTH/2-190/2, SCREEN_HEIGHT/2+20};

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
                                playing = false;
                                quit = true;
                            break;
                            default:
                                quit = true;
                            break;
                        }
                    }
                }
                SDL_BlitSurface( deadBoss, NULL, ScreenSurface, &ded );
                SDL_BlitSurface( cleared, NULL, ScreenSurface, &dest );
                SDL_BlitSurface( thanks, NULL, ScreenSurface, &destB );
                SDL_UpdateWindowSurface( Window );
                SDL_Delay(50);
            }
            boss = NULL;
        }
        else if(collisions < 3 && playing) {
            SDL_Surface * cleared = TTF_RenderText_Solid(font, "Level Cleared!", color);
            SDL_Rect dest = {SCREEN_WIDTH/2-140/2, SCREEN_HEIGHT/2};
            SDL_BlitSurface( cleared, NULL, ScreenSurface, &dest );

            SDL_UpdateWindowSurface( Window );
            SDL_Delay(5000);
        }
        else if(collisions >= 3) {
            // Game over screen
            SDL_Color gameOverColor = {255, 180, 180};
            SDL_Surface * gameOver = TTF_RenderText_Solid(font, "GAME OVER", gameOverColor);
            SDL_Rect destA = {SCREEN_WIDTH/2-110/2, SCREEN_HEIGHT/2-30};

            SDL_Surface * gameOverRestart = TTF_RenderText_Solid(font, "Press any key to restart", color);
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
                                playing = false;
                                quit = true;
                            break;
                            default:
                                quit = true;
                            break;
                        }
                    }
                }
                SDL_BlitSurface( gameOver, NULL, ScreenSurface, &destA );
                SDL_BlitSurface( gameOverRestart, NULL, ScreenSurface, &destB );
                SDL_UpdateWindowSurface( Window );
                SDL_Delay(50);
            }
            // Start game over
            if(collisions >= 3 || level == 5) {
                speedIncrease = 2;
                baseSlowness = 2;
                level = 0;
                lvlDifficulty = 3;
                minDist = 50;
                updateSpeed = 1;
            }
        }

        for(int i=0; i<lvlDifficulty; i++) {
            if(testBall[i] != NULL) {
                testBall[i]->y = -20;
            }
        }
        explosions.reset();
    }

	//Deallocate surface
    for(int i=0; i<4; i++) {
        delete(crater[i]);
    }
    SDL_FreeSurface(ScreenSurface);
    SDL_FreeSurface(ready);
    SDL_FreeSurface(pressButton);
    SDL_FreeSurface(background);
    SDL_FreeSurface(Earth);
    SDL_FreeSurface(life);
    SDL_FreeSurface(byte);

    SDL_DestroyWindow( Window );
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
