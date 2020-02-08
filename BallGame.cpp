#include<SDL2/SDL.h>
#include<cstdio>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[])
{

    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    // Setup the screen
    SDL_Window *Window = SDL_CreateWindow("BallGame",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);
    SDL_Surface* ScreenSurface = SDL_GetWindowSurface( Window );

    // Import image
    SDL_Surface* gXOut = SDL_LoadBMP( "byte.bmp" );
	if( gXOut == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "03_event_driven_programming/x.bmp", SDL_GetError() );
        return 0;
	}

    for(int i=0; i<10; i++)
    {
        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        //While application is running
        while( !quit )
        {
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
            }

            //Apply the image
            SDL_Rect pos = {0, 0, 300, 400};
            SDL_Rect dest = {200, 50};
            SDL_BlitSurface( gXOut, &pos, ScreenSurface, &dest );
        
            //Update the surface
            SDL_UpdateWindowSurface( Window );
        }
    }

	//Deallocate surface
	SDL_FreeSurface( gXOut );
	SDL_DestroyWindow( Window );

    return 0;
}