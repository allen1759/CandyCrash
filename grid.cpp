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
    candy.kind = NO_CANDY;
    candy.special = NO_SPECIAL;
}

