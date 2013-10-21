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
    Vacant = 0,
    Red = 1,
    Green = 2,
    Blue = 3,
    Yellow = 4,
    Purple = 5,
    Orange = 6
};

class Cell : public cocos2d::Sprite
{
public:
    virtual ~Cell() {}

    void init(const int r, const int c, const CellType t);

    static Cell* createWithTexture(cocos2d::Texture2D *texture);

    void SetColor(const GemColor color);
    GemColor GetColor();

    CellType type;
    int row;
    int col;
    bool resolving;

    static cocos2d::CCTexture2D* ChooseTextureByColor(const GemColor color);
    static GemColor RandomColor();
    static void CacheCellTexture();

private:
    GemColor color;
};

#endif