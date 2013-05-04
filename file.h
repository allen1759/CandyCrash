#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <SDL/SDL.h>

enum candysPNG
{
    RPNG, OPNG, YPNG, GPNG, BPNG, PPNG,
    RHPNG, OHPNG, YHPNG, GHPNG, BHPNG, PHPNG,
    RVPNG, OVPNG, YVPNG, GVPNG, BVPNG, PVPNG,
    RPPNG, OPPNG, YPPNG, GPPNG, BPPNG, PPPNG,
    SEPNG, CHOPNG, JELLYPNG, JELLY2PNG, STONEPNG, ROPEPNG
};
enum itemsPNG
{
    GRIDPNG
};

void setCandysClip(SDL_Rect candysClip[]);
void setItemsClip(SDL_Rect itemsClip[]);

#endif // FILE_H_INCLUDED
