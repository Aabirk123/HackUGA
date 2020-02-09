#include <SDL2/SDL.h>
#include<string>

class Boss
{
protected:
    int frames;
    SDL_Surface* sprite;
    int yGoal;
public:
    int x;
    int y;
    int color;
    //Parametrized Constructor
    Boss(int x, int y, int yGoal) {
        this->x = x;
        this->y = y;
        this->yGoal = yGoal;
        color = (rand()%4);
        std::string text = "boss" + std::to_string(color+1) + ".bmp";
        sprite = SDL_LoadBMP( text.c_str() );
    }

    //Parametrized Constructor
    ~Boss() {
	    SDL_FreeSurface( sprite );
    }

    void Update() {
        frames++;
        if(frames %3 == 0)
            if(y < yGoal)
                y += 1;
        if(frames % 600 == 0) {
            color = (rand()%4);
            std::string text = "boss" + std::to_string(color+1) + ".bmp";
            sprite = SDL_LoadBMP( text.c_str() );
        }
    }

    void Paste(SDL_Surface* ScreenSurface) {
        SDL_Rect dest = {x, y};
        SDL_BlitSurface( sprite, NULL, ScreenSurface, &dest );
    }
};
