#ifndef __CELL_H__
#define __CELL_H__

#include "cocos2d.h"

#include <cstdint>

class Map;

enum class DIRECTION : int8_t
{
    DIR1,
    DIR2,
    DIR3,
    DIR4,
    DIR5,
    DIR6
};

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
    Orange = 6,
    S5 = 7
};

enum class GemType : int8_t
{
    Normal = 0,
    Cross2 = 1,
    Cross3 = 2,
    Straight4 = 3,
    Straight5 = 4,
    Circle = 5
};

class Cell : public cocos2d::Sprite
{
public:
    virtual ~Cell() {}

    void Initialize(Map *map, const int r, const int c, const CellType t, const GemColor color);

    static Cell* createWithTexture(cocos2d::Texture2D *texture);

    void SetColorGemTypeDir(const GemColor color, const GemType type, const DIRECTION dir);
    GemColor GetColor();
    GemType GetGemType();
    
    DIRECTION GetDirection();
    
    CellType type;

    int GetRow() const;
    int GetCol() const;

    int resolving;
    bool falling;
    bool exploded;

    static cocos2d::CCTexture2D* GetCellTexture(const GemColor color, const GemType);
    static GemColor RandomColor();
    static void CacheCellTexture();
    void Explode();

    unsigned int fallingTime;

private:
    DIRECTION highDir;
    GemType gemType;
    GemColor color;
    int row;
    int col;
    Map *map;

    void SetDirection(const DIRECTION dir);
};

#endif