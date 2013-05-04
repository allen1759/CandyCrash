#include "game.h"

bool InitGame(SDL_Surface *images[], SDL_Rect clips[][30], TTF_Font * &font)
{
    int colorTranspatent[3]={50, 100, 100};

    //Initialize
    if( InitSDL(images[SCREEN], SCREEN_H, SCREEN_W, SCREEN_B, "Candy Crash") == false )
    {
        return false;
    }

    //Load the files
    if( LoadImageFile(images[BACKGROUNG], "PNG/background.png", colorTranspatent) == false )
    {
        return false;
    }

    if( LoadImageFile(images[CANDYS], "PNG/candys.png", colorTranspatent) == false )
    {
        return false;
    }
    setCandysClip(clips[0]);

    if( LoadImageFile(images[ITEMS], "PNG/items.png", colorTranspatent) == false )
    {
        return false;
    }
    setItemsClip(clips[1]);

    if( LoadFontFile(font, "Font/TEMPSITC.TTF", 64) == false )
    {
        return false;
    }

    return true;
}

void Quit(SDL_Surface *images[], TTF_Font *font)
{
    //Free the surface and quit SDL
    CleanUpSurface(images[BACKGROUNG]);
    CleanUpSurface(images[CANDYS]);
    CleanUpSurface(images[ITEMS]);
    CleanUpFont(font);

    QuitSDL();
}

SDL_Rect *TransCandyClip(CCandy &candy, SDL_Rect clips[][30])
{
    if(candy.special == NO_SPECIAL){
        if(candy.kind == RED_CANDY){
            return &clips[0][RPNG];
        }
        else if(candy.kind == ORANGE_CANDY){
            return &clips[0][OPNG];
        }
        else if(candy.kind == YELLOW_CANDY){
            return &clips[0][YPNG];
        }
        else if(candy.kind == GREEN_CANDY){
            return &clips[0][GPNG];
        }
        else if(candy.kind == BLUE_CANDY){
            return &clips[0][BPNG];
        }
        else if(candy.kind == PURPLE_CANDY){
            return &clips[0][PPNG];
        }
    }
}

void ApplyMap(CGrid map[][10], SDL_Surface *images[], SDL_Rect clips[][30])
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            ApplySurface( 2+MAP_X+80*i, 2+MAP_Y+70*j, images[CANDYS], images[SCREEN], TransCandyClip(map[i][j].candy, clips) );
        }
    }
}

void ApplyGrid(CGrid map[][10], SDL_Surface *images[], SDL_Rect clips[][30])
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            if(map[i][j].kind == NORMAL_GRID){
                ApplySurface( MAP_X+80*i, MAP_Y+70*j, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG] );
            }
        }
    }
}

bool CheckGridRange(CGrid map[][10], int m, int n, int direct[])
{
    if(map[m][n].kind == NO_GRID || map[m+direct[0]][n+direct[1]].kind == NO_GRID) return false;
    return true;
}

bool CheckChange(CGrid map[][10], int m, int n, int direct[])
{
    if(!CheckGridRange(map, m, n, direct)) return false;
    if(map[m][n].candy.kind != NO_CANDY && map[m+direct[0]][n+direct[1]].candy.kind != NO_CANDY) return true;
    return false;
}

bool DisplayChange(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30])
{
    Timer time;
    SDL_Surface *tmp=0;
    for(int k=0; k<(FRAMES_PER_SECOND*1); k++){
        time.start();

        ApplySurface(-300, -300, images[BACKGROUNG], images[SCREEN]);
        ApplyGrid(map, images, clips);
        ApplyMap(map, images, clips);
        ApplySurface(MAP_X+80*m, MAP_Y+70*n, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
        ApplySurface(MAP_X+80*(m+direct[0]), MAP_Y+70*(n+direct[1]), images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);

        double movex = 80.f*direct[0]/FRAMES_PER_SECOND/1;
        double movey = 70.f*direct[1]/FRAMES_PER_SECOND/1;
        ApplySurface( (2+MAP_X+80*m)+k*movex, (2+MAP_Y+70*n)+k*movey, images[CANDYS], images[SCREEN], TransCandyClip(map[m][n].candy, clips) );
        ApplySurface( (2+MAP_X+80*(m+direct[0]))-k*movex, (2+MAP_Y+70*(n+direct[1]))-k*movey, images[CANDYS], images[SCREEN], TransCandyClip(map[m+direct[0]][n+direct[1]].candy, clips) );

        if( SDL_Flip( images[SCREEN] ) == -1 )
        {
            return false;
        }
        if(time.getTicks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - time.getTicks() );
        }
    }
    ApplySurface(MAP_X+80*m, MAP_Y+70*n, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
    ApplySurface(MAP_X+80*(m+direct[0]), MAP_Y+70*(n+direct[1]), images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
    ApplySurface( (2+MAP_X+80*(m+direct[0])), (2+MAP_Y+70*(n+direct[1])), images[CANDYS], images[SCREEN], TransCandyClip(map[m][n].candy, clips) );
    ApplySurface( (2+MAP_X+80*m), (2+MAP_Y+70*n), images[CANDYS], images[SCREEN], TransCandyClip(map[m+direct[0]][n+direct[1]].candy, clips) );
    if( SDL_Flip( images[SCREEN] ) == -1 )
    {
        return false;
    }
    return true;
}

bool DisplayChangeFail(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30])
{
    Timer time;
    for(int k=0; k<(FRAMES_PER_SECOND*1); k++){
        time.start();

        ApplySurface(-300, -300, images[BACKGROUNG], images[SCREEN]);
        ApplyGrid(map, images, clips);
        ApplyMap(map, images, clips);
        ApplySurface(MAP_X+80*m, MAP_Y+70*n, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
        ApplySurface(MAP_X+80*(m+direct[0]), MAP_Y+70*(n+direct[1]), images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);

        double movex = 80.f*direct[0]/FRAMES_PER_SECOND/1;
        double movey = 70.f*direct[1]/FRAMES_PER_SECOND/1;
        ApplySurface( (2+MAP_X+80*m)+k*movex, (2+MAP_Y+70*n)+k*movey, images[CANDYS], images[SCREEN], TransCandyClip(map[m][n].candy, clips) );
        ApplySurface( (2+MAP_X+80*(m+direct[0]))-k*movex, (2+MAP_Y+70*(n+direct[1]))-k*movey, images[CANDYS], images[SCREEN], TransCandyClip(map[m+direct[0]][n+direct[1]].candy, clips) );

        if( SDL_Flip( images[SCREEN] ) == -1 )
        {
            return false;
        }
        if(time.getTicks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - time.getTicks() );
        }
    }

    for(int k=0; k<(FRAMES_PER_SECOND*1); k++){
        time.start();

        ApplySurface(-300, -300, images[BACKGROUNG], images[SCREEN]);
        ApplyGrid(map, images, clips);
        ApplyMap(map, images, clips);
        ApplySurface(MAP_X+80*m, MAP_Y+70*n, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
        ApplySurface(MAP_X+80*(m+direct[0]), MAP_Y+70*(n+direct[1]), images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);

        double movex = 80.f*direct[0]/FRAMES_PER_SECOND/1;
        double movey = 70.f*direct[1]/FRAMES_PER_SECOND/1;
        ApplySurface( (2+MAP_X+80*(m+direct[0]))-k*movex, (2+MAP_Y+70*(n+direct[0]))-k*movey, images[CANDYS], images[SCREEN], TransCandyClip(map[m][n].candy, clips) );
        ApplySurface( (2+MAP_X+80*m)+k*movex, (2+MAP_Y+70*n)+k*movey, images[CANDYS], images[SCREEN], TransCandyClip(map[m+direct[0]][n+direct[1]].candy, clips) );

        if( SDL_Flip( images[SCREEN] ) == -1 )
        {
            return false;
        }
        if(time.getTicks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - time.getTicks() );
        }
    }

    ApplySurface(MAP_X+80*m, MAP_Y+70*n, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
    ApplySurface(MAP_X+80*(m+direct[0]), MAP_Y+70*(n+direct[1]), images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
    ApplySurface( (2+MAP_X+80*m), (2+MAP_Y+70*n), images[CANDYS], images[SCREEN], TransCandyClip(map[m][n].candy, clips) );
    ApplySurface( (2+MAP_X+80*(m+direct[0])), (2+MAP_Y+70*(n+direct[1])), images[CANDYS], images[SCREEN], TransCandyClip(map[m+direct[0]][n+direct[1]].candy, clips) );

    if( SDL_Flip( images[SCREEN] ) == -1 )
    {
        return false;
    }

    return true;
}

bool ChangeCandy(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30])
{
    if(!DisplayChange(map, m, n, direct, images, clips)) return false;
    CCandy temp = map[m][n].candy;
    map[m][n].candy = map[m+direct[0]][n+direct[1]].candy;
    map[m+direct[0]][n+direct[1]].candy = temp;
    return true;
}
