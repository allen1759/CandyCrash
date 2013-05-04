#include "level.h"

void SetLevelMap(CGrid map[][10], int level)
{
    CandyKind randCandy[6]={RED_CANDY, ORANGE_CANDY, YELLOW_CANDY, GREEN_CANDY, BLUE_CANDY, PURPLE_CANDY};
    switch(level){
    case 0:{
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                map[i][j].kind = NO_GRID;
                map[i][j].extra = NO_EXTRA;
                map[i][j].ChangeRect(0, 0, 0, 0);
                map[i][j].CleanCandy();
            }
        }
        break;
    }
    case 1:{
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                map[i][j].kind = NORMAL_GRID;
                map[i][j].extra = NO_EXTRA;
                map[i][j].ChangeRect(MAP_X+80*i, MAP_Y+70*j, 80, 70);
                map[i][j].candy.kind = randCandy[rand()%6];
            }
        }
        break;
    }

    case 2:{
        break;
    }
    }
}
