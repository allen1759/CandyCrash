#ifndef CANDY_H_INCLUDED
#define CANDY_H_INCLUDED

#include "mySDL.h"

enum CandyKind
{
    NO_CANDY,
    RED_CANDY,
    ORANGE_CANDY,
    YELLOW_CANDY,
    GREEN_CANDY,
    BLUE_CANDY,
    PURPLE_CANDY,
    CHOCOLATE_CANDY
};

enum SpecialCandy
{
    NO_SPECIAL,
    HORIZON_SPECIAL,
    VERTICAL_SPECIAL,
    PAPER_SPECIAL,
};

class CCandy// : public CButton
{
public:
    CandyKind kind;
    SpecialCandy special;

    CCandy()// : CButton( x, y, w, h )
    {
        kind=NO_CANDY;
        special=NO_SPECIAL;
    }
    void SetCandyKind(CandyKind kind)
    {
        this->kind=kind;
    }
    void SetSpecialCandy(SpecialCandy special)
    {
        this->special=special;
    }
    void TransChocolate()
    {
        this->kind=CHOCOLATE_CANDY;
        this->special=NO_SPECIAL;
    }
    void BecomeHorizon()
    {
        this->special=HORIZON_SPECIAL;
    }
    void BecomeVertical()
    {
        this->special=VERTICAL_SPECIAL;
    }
    void BecomePaper()
    {
        this->special=PAPER_SPECIAL;
    }
};

#endif // CANDY_H_INCLUDED
