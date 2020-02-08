#include <SDL2/SDL.h>

class Explosion
{
protected:
    SDL_Surface* sprite;
    int ticksToDestroy;
public:
    int x;
    int y;
    //Parametrized Constructor
    Explosion(int x, int y) {
        this->x = x-2;
        this->y = y-2;
        sprite = SDL_LoadBMP( "explosion.bmp" );
        ticksToDestroy = SDL_GetTicks()+1000;
    }

    //Parametrized Constructor
    ~Explosion() {
	    SDL_FreeSurface( sprite );
    }

    void Paste(SDL_Surface* ScreenSurface) {
        SDL_Rect dest = {x, y};
        SDL_BlitSurface( sprite, NULL, ScreenSurface, &dest );
        if(SDL_GetTicks() > ticksToDestroy) {
            delete this;
        }
    }
};
