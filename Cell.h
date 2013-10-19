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
    GemColor color;
    CellType type;
    int row;
    int col;
    bool resolving;

    Cell(int r, int c, CellType t);
};

#endif