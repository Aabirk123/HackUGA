#include <SDL2/SDL.h>
#include<string>

class Ball
{
protected:
    int x;
    int y;
    SDL_Surface* sprite;
public:
    //Parametrized Constructor
    Ball(int x, int color) {
        this->x = x;
        this->y = -10;
        std::string text = "ball" + std::to_string(color) + ".bmp";
        sprite = SDL_LoadBMP( text.c_str() );
    }

    void Update(int speed) {
        y += speed;
    }

    bool touchingBox(int targetX, int targetY, int width, int height) {
        if((x > targetX && x < targetX+width) || (x+40 > targetX && x+40 < targetX+width))
        {
            if((y > targetY && y < targetY+height) || (y+40 > targetY && y+40 < targetY+height))
                return true;
        }
        return false;
    }
    void Paste(SDL_Surface* ScreenSurface) {
        SDL_Rect dest = {x, y};
        SDL_BlitSurface( sprite, NULL, ScreenSurface, &dest );
    }
};