#include "grid.h"

CGrid::CGrid()
{
    kind = NORMAL_GRID;
    extra = NO_EXTRA;
    candy.kind = NO_CANDY;
    candy.special = NO_SPECIAL;
}

void CGrid::SetGridKind(GridKind kind)
{
    this->kind=kind;
}

void CGrid::SetGridExtra(GridExtra extra)
{
    this->extra=extra;
}

void CGrid::CleanCandy()
{
    if(this->extra == NO_EXTRA){
        this->candy.kind = NO_CANDY;
    }
    else if(this->extra == ROPE_EXTRA){}
}

void CGrid::CleanEdge()
{
    if(this->candy.kind == ROCK_CANDY){
        this->candy.kind = NO_CANDY;
    }
    else if(this->candy.kind == CHOCO_CANDY){
        this->candy.kind = NO_CANDY;
    }
}
