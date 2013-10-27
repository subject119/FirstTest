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
    Orange = 6
};

enum class GemType : int8_t
{
    Normal,
    Straight4,
    Straight5,
    Cross2,
    Cross3,
    Circle
};

class Cell : public cocos2d::Sprite
{
public:
    virtual ~Cell() {}

    void Initialize(Map *map, const int r, const int c, const CellType t, const GemColor color);

    static Cell* createWithTexture(cocos2d::Texture2D *texture);

    void SetColorGemType(const GemColor color, const GemType type);
    GemColor GetColor();
    GemType GetGemType();
    void SetDirection(const DIRECTION dir);
    DIRECTION GetDirection();
    
    CellType type;

    int GetRow() const;
    int GetCol() const;

    int resolving;
    bool falling;

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
};

#endif