#include <SDL2/SDL.h>
#include<string>

class Ball
{
protected:
    SDL_Surface* sprite;
public:
    int x;
    int y;
    int color;
    //Parametrized Constructor
    Ball(int x, int y, int color) {
        this->x = x;
        this->y = y;
        this->color = color-1;
        std::string text = "ball" + std::to_string(color) + ".bmp";
        sprite = SDL_LoadBMP( text.c_str() );
	
    }

    void changeColor(int newColor) {
        this->color = newColor-1;
        std::string text = "ball" + std::to_string(newColor) + ".bmp";
        sprite = SDL_LoadBMP( text.c_str() );
    }

    //Parametrized Constructor
    ~Ball() {
	    SDL_FreeSurface( sprite );
    }

    void Update(int xSpeed, int speed) {
        x += xSpeed;
        y += speed;
    }

    bool touchingBox(int targetX, int targetY, int width, int height) {
        if((x > targetX && x < targetX+width) || (x+20 > targetX && x+20 < targetX+width))
        {
            if((y > targetY && y < targetY+height) || (y+20 > targetY && y+20 < targetY+height))
                return true;
        }
        return false;
    }
    void Paste(SDL_Surface* ScreenSurface) {
        SDL_Rect dest = {x, y};
        SDL_BlitSurface( sprite, NULL, ScreenSurface, &dest );
    }
};
