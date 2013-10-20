#ifndef __IOMANAGER_H__
#define __IOMANAGER_H__

#include "cocos2d.h"

class GameManager;
class Cell;

class MapData
{
public:
    int height;
    int width;
    int bit;
};

class IOManager : public cocos2d::Layer
{
public:
    GameManager *gameManager;

    virtual ~IOManager() {}

    CREATE_FUNC(IOManager);

    Cell *selectedCell;

    virtual bool init();

    MapData *mapData;

    MapData *GetMapData();

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event);
};

#endif
