#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "mySDL.h"
#include "grid.h"
#include "file.h"
#include "defineConst.h"

enum surface
{
    SCREEN, BACKGROUNG, CANDYS, ITEMS
};

bool InitGame(SDL_Surface *images[], SDL_Rect clips[][30], TTF_Font * &font);
void Quit(SDL_Surface *images[], TTF_Font *font);
SDL_Rect *TransCandyClip(CCandy &candy, SDL_Rect clips[][30]);
void ApplyMap(CGrid map[][10], SDL_Surface *images[], SDL_Rect clips[][30]);
void ApplyGrid(CGrid map[][10], SDL_Surface *images[], SDL_Rect clips[][30]);
bool CheckGridRange(CGrid map[][10], int m, int n, int direct[]);
bool CheckChange(CGrid map[][10], int m, int n, int direct[]);
bool DisplayChange(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30]);
bool DisplayChangeFail(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30]);
bool ChangeCandy(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30]);

#endif // GAME_H_INCLUDED
