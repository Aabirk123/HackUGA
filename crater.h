#include <SDL2/SDL.h>
#include<string>

class Crater
{
protected:
    SDL_Surface* sprite;
public:
    int x;
    int y;
    int color;
    //Parametrized Constructor
    Crater(int x, int y, int color) {
        this->x = x;
        this->y = y;
        this->color = color-1;
        std::string text = "crater" + std::to_string(color) + ".bmp";
        sprite = SDL_LoadBMP( text.c_str() );
    }

    //Parametrized Constructor
    ~Crater() {
	    SDL_FreeSurface( sprite );
    }

    void Update(int xSpeed, int speed) {
        x += xSpeed;
        y += speed;
    }

    void CheckOffscreen(int screenWidth) {
        if(x < -100) {
            x = screenWidth;
        }
        if(x > screenWidth) {
            x = -100;
        }
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