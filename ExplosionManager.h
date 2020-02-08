#include <SDL2/SDL.h>

class ExplosionManager
{
protected:
    SDL_Surface* sprite;
    int num;
public:
    int* x;
    int* y;
    int* framesToDestroy;
    // Constructor
    ExplosionManager() {
        sprite = SDL_LoadBMP( "explosion.bmp" );
        num = 0;
        x = new int[20];
        y = new int[20];
        framesToDestroy = new int[20];
    }
    //Parametrized Constructor
    void add(int x, int y, int curFrame) {
        this->x[num] = x-2;
        this->y[num] = y-2;
        framesToDestroy[num] = curFrame+100;
        num++;
    }

    //Parametrized Constructor
    ~ExplosionManager() {
	    SDL_FreeSurface( sprite );
    }

    void Update(int curFrame) {
        for(int i=0; i<num; i++) {
            if(curFrame > framesToDestroy[i]) {
                framesToDestroy[i] = 0;
                // Decrease number of explosions as far as possible (clean up previously removed explosions as well)
                while(framesToDestroy[i] == 0 && i == num-1 && num > 0) {
                    i--;
                    num--;
                }
            }
        }
    }

    void PasteAll(SDL_Surface* ScreenSurface) {
        for(int i=0; i<num; i++) {
            if(framesToDestroy[i] != 0) {
                SDL_Rect dest = {x[i], y[i]};
                SDL_BlitSurface( sprite, NULL, ScreenSurface, &dest );
            }
        }
    }
};
