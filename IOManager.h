#ifndef __IOMANAGER_H__
#define __IOMANAGER_H__

#include "cocos2d.h"

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
    MapData *mapData;

    MapData *GetMapData();

    void KeyboardInputRun();

    void ParseInput(std::string input, int *swapRequest);
};

#endif
