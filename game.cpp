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
            if(map[i][j].isNO_GRID()) continue;
            if(map[i][j].candy.kind != NO_CANDY){
                ApplySurface( 2+MAP_X+80*i, 2+MAP_Y+70*j, images[CANDYS], images[SCREEN], TransCandyClip(map[i][j].candy, clips) );
            }
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

bool isGridRange(CGrid map[][10], int m, int n)
{
    if(map[m][n].kind == NO_GRID) return false;
    return true;
}
bool isGridRange(CGrid map[][10], int m, int n, int direct[])
{
    if(map[m][n].kind == NO_GRID || map[m+direct[0]][n+direct[1]].kind == NO_GRID) return false;
    return true;
}

bool isChange(CGrid map[][10], int m, int n)
{
    if(!isGridRange(map, m, n)) return false;
    if(map[m][n].candy.kind != NO_CANDY ) return true;
    return false;
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
                ChangeCandy(map, m, n, revChangeDirect);
                return CHOCO_CHO_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == PAPER_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return CHOCO_PAP_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == HORIZON_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return CHOCO_HOR_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == VERTICAL_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return CHOCO_VER_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == NO_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return CHOCO_ONE_CLEAR;
            }
        }
        else if(map[m][n].candy.special == PAPER_SPECIAL){
            if(map[m+changeDirect[0]][n+changeDirect[1]].candy.kind == CHOCOLATE_CANDY){
                ChangeCandy(map, m, n, revChangeDirect);
                return CHOCO_CHO_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == PAPER_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return PAP_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == HORIZON_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return PAP_HOR_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == VERTICAL_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return PAP_VER_CLEAR;
            }
        }
        else if(map[m][n].candy.special == VERTICAL_SPECIAL){
            if(map[m+changeDirect[0]][n+changeDirect[1]].candy.kind == CHOCOLATE_CANDY){
                ChangeCandy(map, m, n, revChangeDirect);
                return CHOCO_VER_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == PAPER_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return PAP_VER_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == HORIZON_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return HOR_VER_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == VERTICAL_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return HOR_VER_CLEAR;
            }
        }
        else if(map[m][n].candy.special == HORIZON_SPECIAL){
            if(map[m+changeDirect[0]][n+changeDirect[1]].candy.kind == CHOCOLATE_CANDY){
                ChangeCandy(map, m, n, revChangeDirect);
                return CHOCO_HOR_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == PAPER_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return PAP_HOR_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == HORIZON_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return HOR_VER_CLEAR;
            }
            else if(map[m+changeDirect[0]][n+changeDirect[1]].candy.special == VERTICAL_SPECIAL){
                ChangeCandy(map, m, n, revChangeDirect);
                return HOR_VER_CLEAR;
            }
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

bool xy2mn(CGrid map[][10], int x, int y, int &m, int &n)
{
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(isNO_GRID(map[i][j])) continue;
            if((i+1)*80+MAP_X > x && (j+1)*70+MAP_Y > y){
                m=i;
                n=j;
                return true;
            }
        }
    }
    return false;
}

bool SelectGrid(int &prem, int &pren, CGrid map[][10], SDL_Event &event, SDL_Surface *images[], SDL_Rect clips[][30], bool &isSelect)
{
    int m, n;
    if(!xy2mn(map, event.motion.x, event.motion.y, m, n)) return true;
    if(!isChange(map, m, n) && !isSelect) return true;
    if(map[m][n].IsPress(event)==LEFTPRESS){
        if(!isSelect){
            ApplySurface(2+MAP_X+80*m, 2+MAP_Y+70*n, images[CANDYS], images[SCREEN], &clips[0][SEPNG]);
            prem=m;
            pren=n;
            isSelect=true;
            if( SDL_Flip( images[SCREEN] ) == -1 )
            {
                return false;
            }
        }
        else{
            if(!isChange(map, m, n) && isSelect){
                ApplyGrid(map, images, clips);
                ApplyMap(map, images, clips);
                prem=m;
                pren=n;
                isSelect=false;
                if( SDL_Flip( images[SCREEN] ) == -1 )
                {
                    return false;
                }
                return true;
            }
            ApplySurface(MAP_X+80*prem, MAP_Y+70*pren, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
            if(map[prem][pren].extra!=NO_EXTRA){}
            ApplySurface(2+MAP_X+80*prem, 2+MAP_Y+70*pren, images[CANDYS], images[SCREEN], TransCandyClip(map[prem][pren].candy, clips));
            if( SDL_Flip( images[SCREEN] ) == -1 )
            {
                return false;
            }
            int fourDirect[4][2]={{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
            int disDirect[2]={m-prem, n-pren};
            for(int i=0; i<4; i++){
                if(fourDirect[i][0] == disDirect[0] && fourDirect[i][1] == disDirect[1]){
                    if(!DisplayChange(map, prem, pren, disDirect, images, clips)) return false;
                    ChangeCandy(map, prem, pren, disDirect);
                    break;
                }
            }
            isSelect=false;
        }
    }
    return true;
}

void ClearAll(CGrid map[][10], SDL_Surface *images[], SDL_Rect clips[][30])
{
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(isNO_GRID(map[i][j])) continue;

        }
    }
}

void ClearOneTag(CGrid map[][10], int m, int n, ClearType clear, SDL_Surface *images[], SDL_Rect clips[][30])
{
    int direct[2][2]={{0, 1}, {1, 0}};
    int fourDirect[4][2]={{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    switch(clear)
    {
        case CHOCO_CHO_CLEAR:{
            break;
        }
        case CHOCO_PAP_CLEAR:{
            break;
        }
        case CHOCO_HOR_CLEAR:{
            break;
        }
        case CHOCO_VER_CLEAR:{
            break;
        }
        case CHOCO_ONE_CLEAR:{
            break;
        }
        case PAP_HOR_CLEAR:{
            break;
        }
        case PAP_VER_CLEAR:{
            break;
        }
        case CHOCO_CLEAR:{
            break;
        }
        case PAP_CLEAR:{
            break;
        }
        case HOR_CLEAR:{
            break;
        }
        case VER_CLEAR:{
            break;
        }
        case THREE_CLEAR:{
            int x=m, y=n;
            x++; y++;
            while(map[x][y].candy.kind == map[m][n].candy.kind){
                map[x][y].clear=true;
                for(int d=0; d<4; d++){
                    if(!isNO_GRID(map[x][y])){
                        map[x+direct[d][0]][y+direct[d][1]].edgeClear=true;
                    }
                }
                x++;
                y++;
            }
            x=m;
            y=n;
            x--; y--;
            while(map[x][y].candy.kind == map[m][n].candy.kind){
                map[x][y].clear=true;
                for(int d=0; d<4; d++){
                    if(!isNO_GRID(map[x][y])){
                        map[x-direct[d][0]][y-direct[d][1]].edgeClear=true;
                    }
                }
            }
            break;
        }
        case NO_CLEAR:{
            break;
        }
    }
}

void ClearCandy(CGrid map[][10])
{
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(!isNO_GRID(map[i][j])) continue;
            if(map[i][j].clear){
                if(map[i][j].extra == NO_EXTRA){
                    //map[i][j].
                }
            }
        }
    }
}

bool fallDown(CGrid map[][10], SDL_Surface *images[], SDL_Rect clips[][30])
{
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(map[i][j].isNO_GRID()) continue;
            if(map[i][j].candy.kind == NO_CANDY){
                CCandy newCandy;
                newCandy.special=NO_SPECIAL;
                int kind=rand()%6;
                switch(kind)
                {
                    case 0:{
                        newCandy.kind=RED_CANDY;
                    }
                    case 1:{
                        newCandy.kind=ORANGE_CANDY;
                    }
                    case 2:{
                        newCandy.kind=YELLOW_CANDY;
                    }
                    case 3:{
                        newCandy.kind=GREEN_CANDY;
                    }
                    case 4:{
                        newCandy.kind=BLUE_CANDY;
                    }
                    case 5:{
                        newCandy.kind=PURPLE_CANDY;
                    }
                }

                Timer time;
                double totalT=0.5;
                int direct[2]={0, 1};           //fall down y+1
                for(int k=0; k<(FRAMES_PER_SECOND*totalT); k++){
                    time.start();

                    ApplySurface(-300, -300, images[BACKGROUNG], images[SCREEN]);
                    ApplyGrid(map, images, clips);
                    ApplyMap(map, images, clips);
                    for(int t=0; t<j; t++){
                        ApplySurface(MAP_X+80*i, MAP_Y+70*t, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
                    }

                    double movex = 80.f*direct[0]/FRAMES_PER_SECOND/totalT;
                    double movey = 70.f*direct[1]/FRAMES_PER_SECOND/totalT;
                    for(int t=0; t<j; t++){
                        ApplySurface( (2+MAP_X+80*i)+k*movex, (2+MAP_Y+70*t)+k*movey, images[CANDYS], images[SCREEN], TransCandyClip(map[i][t].candy, clips) );
                    }
                    SDL_Delay(5000);

                    if( SDL_Flip( images[SCREEN] ) == -1 )
                    {
                        return false;
                    }
                    if(time.getTicks() < 1000 / FRAMES_PER_SECOND)
                    {
                        SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - time.getTicks() );
                    }
                }

                ApplySurface(-300, -300, images[BACKGROUNG], images[SCREEN]);
                ApplyGrid(map, images, clips);
                ApplyMap(map, images, clips);
                for(int t=0; t<j; t++){
                    ApplySurface(MAP_X+80*i, MAP_Y+70*t, images[ITEMS], images[SCREEN], &clips[1][GRIDPNG]);
                }
                for(int t=0; t<j; t++){
                    ApplySurface( (2+MAP_X+80*i), (2+MAP_Y+70*(t+1)), images[CANDYS], images[SCREEN], TransCandyClip(map[i][t].candy, clips) );
                }
                if( SDL_Flip( images[SCREEN] ) == -1 )
                {
                    return false;
                }
                return true;


            }
        }
    }
}




























































