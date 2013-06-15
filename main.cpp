#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <ctime>
#include "mySDL.h"
#include "grid.h"
#include "file.h"
#include "level.h"
#include "game.h"
#include "defineConst.h"
using namespace std;

int main( int argc, char* args[] )
{
    srand(time(0));
    //Make sure the program waits for a quit
    bool quit = false;
    int frame=0;
    Timer time;

    SDL_Event event;

    SDL_Surface *images[10]={NULL};
    SDL_Rect clips[2][30];
    TTF_Font *font=NULL;

    CGrid map[10][10];

    CButton click(0, 0, 100, 100);

    if(!InitGame(images, clips, font)) return 1;

    SDL_Color textColor = { 255, 255, 255 };
    //Render the text
    SDL_Surface *message = TTF_RenderText_Solid( font, "Test the text...", textColor );

    //If there was an error in rendering the text
    if( message == NULL )
    {
        return 1;
    }

    SetLevelMap(map, 0);
    SetLevelMap(map, 1);

    map[4][4].candy.kind = NO_CANDY;
    map[4][4].candy.special = NO_SPECIAL;

    ApplySurface(-300, -300, images[BACKGROUNG], images[SCREEN]);
    ApplyGrid(map, images, clips);
    ApplyMap(map, images, clips);

    //Update the screen
    if( SDL_Flip( images[SCREEN] ) == -1 )
    {
        return 1;
    }

    bool isSelect=false;
    int m, n;

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        time.start();


        //While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
            time.start();
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
            if( event.type == SDL_MOUSEBUTTONDOWN){
                if(!SelectGrid(m, n, map, event, images, clips, isSelect)) return 1;
                //SDL_Delay(3000);
                //fallDown(map, images, clips);
            }

        }
    }

    Quit(images, font);
    return 0;
}

