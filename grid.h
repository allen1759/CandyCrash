#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include "candy.h"
#include "defineConst.h"

enum GridKind
{
    NO_GRID,
    NORMAL_GRID,
    JELLY_GRID,
    JELLY2_GRID,
    ROCK_GRID
};
enum GridExtra
{
    NO_EXTRA,
    ROPE_EXTRA,
    CHOCO_EXTRA
};

class CGrid : public CButton
{
public:
    GridKind kind;
    GridExtra extra;
    CCandy candy;

    CGrid();
    void SetGridKind(GridKind kind);
    void SetGridExtra(GridExtra extra);
    void CleanCandy();
};

#endif // GRID_H_INCLUDED
