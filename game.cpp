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
            if(map[i][j].kind != NO_GRID)
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

bool isGridRange(CGrid map[][10], int m, int n, int direct[])
{
    if(map[m][n].kind == NO_GRID || map[m+direct[0]][n+direct[1]].kind == NO_GRID) return false;
    return true;
}

bool isChange(CGrid map[][10], int m, int n, int direct[])
{
    if(!isGridRange(map, m, n, direct)) return false;
    if(map[m][n].candy.kind != NO_CANDY && map[m+direct[0]][n+direct[1]].candy.kind != NO_CANDY) return true;
    return false;
}

bool DisplayChange(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30])
{
    Timer time;
    double totalT=0.5;
    for(int k=0; k<(FRAMES_PER_SECOND*totalT); k++){
        time.start();

        ApplySurface(-300, -300, images[BACKGROUNG], images[SCREEN]);
        ApplyGrid(map, images, clips);
        ApplyMap(map, images, clips);
        ApplySurface(MAP_X+80*m, MAP_Y+70*n, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
        ApplySurface(MAP_X+80*(m+direct[0]), MAP_Y+70*(n+direct[1]), images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);

        double movex = 80.f*direct[0]/FRAMES_PER_SECOND/totalT;
        double movey = 70.f*direct[1]/FRAMES_PER_SECOND/totalT;
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

void ChangeCandy(CGrid map[][10], int m, int n, int direct[])
{
    CCandy temp = map[m][n].candy;
    map[m][n].candy = map[m+direct[0]][n+direct[1]].candy;
    map[m+direct[0]][n+direct[1]].candy = temp;
}

ClearType isClear(CGrid map[][10], int m, int n, int changeDirect[])
{
    ChangeCandy(map, m, n, changeDirect);
    int revChangeDirect[2]={-changeDirect[0], -changeDirect[1]};
    int x=m, y=n;
    ClearType curType = NO_CLEAR;
    int direct[4][2]={{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    for(int i=0; i<4; i++){
        if(map[m][n].candy.kind == CHOCOLATE_CANDY){
            if(map[m+changeDirect[0]][n+changeDirect[1]].candy.kind == CHOCOLATE_CANDY){

                return CHOCO_CHO_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == PAPER_SPECIAL){

                return CHOCO_PAP_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == HORIZON_SPECIAL){

                return CHOCO_HOR_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == VERTICAL_SPECIAL){

                return CHOCO_VER_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == NO_SPECIAL){

                return CHOCO_ONE_CLEAR;
            }
        }
        else if(map[m][n].candy.special == PAPER_SPECIAL){

        }
        else if(map[m][n].candy.special == VERTICAL_SPECIAL){

        }
        else if(map[m][n].candy.special == HORIZON_SPECIAL){

        }
    }

    for(int i=0; i<2; i++){                     //Check five four three clear
        int cnt=1;
        x=m;
        y=n;
        while(0 <= x+direct[i][2] && x+direct[i][2] < 10 &&
              0 <= y+direct[i][2] && y+direct[i][2] < 10){
            if(map[x+direct[i][2]][y+direct[i][2]].candy.kind == map[m][n].candy.kind){
                cnt++;
                x+=direct[i][2];
                y+=direct[i][2];
            }
            else{
                break;
            }
        }
        x=m;
        y=n;
        while(0 <= x-direct[i][2] && x-direct[i][2] < 10 &&
              0 <= y-direct[i][2] && y-direct[i][2] < 10){
            if(map[x-direct[i][2]][y-direct[i][2]].candy.kind == map[m][n].candy.kind){
                cnt++;
                x-=direct[i][2];
                y-=direct[i][2];
            }
            else{
                break;
            }
        }
        if(cnt>=5){
            ChangeCandy(map, m, n, revChangeDirect);
            return CHOCO_CLEAR;
        }
        else if(cnt>=4 && i==0){
            ChangeCandy(map, m, n, revChangeDirect);
            return VER_CLEAR;
        }
        else if(cnt>=4 && i==1){
            ChangeCandy(map, m, n, revChangeDirect);
            return HOR_CLEAR;
        }
        else if(cnt>=3){
            ChangeCandy(map, m, n, revChangeDirect);
            return THREE_CLEAR;
        }
    }
    return NO_CLEAR;
}

bool isNO_GRID(const CGrid &grid)
{
    return grid.kind==NO_GRID;
}

void xy2mn(CGrid map[][10], int x, int y, int &m, int &n)
{
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(isNO_GRID(map[i][j])) continue;
            if((i+1)*80+MAP_X > x && (j+1)*80+MAP_Y > y){
                m=i;
                n=j;
                return;
            }
        }
    }
}

bool SelectGrid(CGrid map[][10], SDL_Event &event, SDL_Surface *images[], SDL_Rect clips[][30])
{
    int m, n;
    xy2mn(map, event.motion.x, event.motion.y, m, n);
    if(map[m][n].IsPress(event)==LEFTPRESS)
    {
        stringstream ss;
        string str="test ";
        ss << str;
        ss << m << " " << n;
        SDL_WM_SetCaption( ss.str().c_str(), NULL );
    }
}





























