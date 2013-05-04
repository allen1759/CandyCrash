#include "file.h"

void setCandysClip(SDL_Rect candysClip[])
{
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            candysClip[i*6+j].x=75*j;
            candysClip[i*6+j].y=65*i;
            candysClip[i*6+j].w=75;
            candysClip[i*6+j].h=65;
        }
    }
}

void setItemsClip(SDL_Rect itemsClip[])
{
    itemsClip[GRIDPNG].x=0;
    itemsClip[GRIDPNG].y=0;
    itemsClip[GRIDPNG].w=80;
    itemsClip[GRIDPNG].h=70;
}
