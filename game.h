#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "mySDL.h"
#include "grid.h"
#include "file.h"
#include "defineConst.h"

#include <string>
#include <sstream>

enum surface
{
    SCREEN, BACKGROUNG, CANDYS, ITEMS
};

enum ClearType
{
    CHOCO_CHO_CLEAR,
    CHOCO_PAP_CLEAR,
    CHOCO_HOR_CLEAR,
    CHOCO_VER_CLEAR,
    CHOCO_ONE_CLEAR,
    PAP_HOR_CLEAR,
    PAP_VER_CLEAR,
    CHOCO_CLEAR,
    PAP_CLEAR,
    HOR_CLEAR,
    VER_CLEAR,
    THREE_CLEAR,
    NO_CLEAR
};

bool InitGame(SDL_Surface *images[], SDL_Rect clips[][30], TTF_Font * &font);
void Quit(SDL_Surface *images[], TTF_Font *font);
SDL_Rect *TransCandyClip(CCandy &candy, SDL_Rect clips[][30]);
void ApplyMap(CGrid map[][10], SDL_Surface *images[], SDL_Rect clips[][30]);
void ApplyGrid(CGrid map[][10], SDL_Surface *images[], SDL_Rect clips[][30]);
bool isGridRange(CGrid map[][10], int m, int n, int direct[]);
bool isChange(CGrid map[][10], int m, int n, int direct[]);
bool DisplayChange(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30]);
bool DisplayChangeFail(CGrid map[][10], int m, int n, int direct[], SDL_Surface *images[], SDL_Rect clips[][30]);
void ChangeCandy(CGrid map[][10], int m, int n, int direct[]);
ClearType isClear(CGrid map[][10], int m, int n, int changeDirect[]);
bool isNO_GRID(const CGrid &grid);
bool xy2mn(CGrid map[][10], int x, int y, int &m, int &n);
bool SelectGrid(int &prem, int &pren, CGrid map[][10], SDL_Event &event, SDL_Surface *images[], SDL_Rect clips[][30], bool &isSelect);


#endif // GAME_H_INCLUDED
