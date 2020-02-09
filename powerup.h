#include <SDL2/SDL.h>
#include <string>

class powerUp
{
protected:
    int x;
    int y;
    SDL_Surface* sprite;
public:
    int whichOne;

    powerUp(int x, int whichOne)
    {
            this->x = x;
            this->y = -10;
            this->whichOne = whichOne -1;
            std::string text = "powerup" + std::to_string(whichOne) + ".bmp";
            sprite = SDL_LoadBMP( text.c_str() );
    }

    ~powerUp()
    {
        SDL_FreeSurface( sprite );
    }
    void update(int Yspeed)
    {
        y+= Yspeed;
    }
    void Paste(SDL_Surface* ScreenSurface) {
        SDL_Rect dest = {x, y};
        SDL_BlitSurface( sprite, NULL, ScreenSurface, &dest );
    }

    bool touchingBox(int targetX, int targetY, int width, int height) {
        if((x > targetX && x < targetX+width) || (x+20 > targetX && x+20 < targetX+width))
        {
            if((y > targetY && y < targetY+height) || (y+20 > targetY && y+20 < targetY+height))
                return true;
        }
        return false;
    }

};
    