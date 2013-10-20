#ifndef __CELL_H__
#define __CELL_H__

#include "cocos2d.h"

#include <cstdint>

enum class CellType : int8_t
{
    Inspace,
    Outspace
};

enum class GemColor : int8_t 
{
    Vacant,
    Red,
    Green,
    Blue,
    White,
    Yellow,
    Purple,
    Orange
};

class Cell : public cocos2d::Sprite
{
public:
    void init(int r, int c, CellType t);

    static Cell* create();

    void SetColor(GemColor color);
    GemColor GetColor();

    CellType type;
    int row;
    int col;
    bool resolving;

private:
    GemColor color;
    cocos2d::CCTexture2D* ChooseTextureByColor(GemColor color);

};

#endif