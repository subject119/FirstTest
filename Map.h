#ifndef __MAP_H__
#define __MAP_H__

#include "cocos2d.h"

#include "Cell.h"

class GameManager;
class MapData;

class Map : public cocos2d::Layer
{
public:
    virtual ~Map() {}

    virtual bool init();

    GameManager *gameManager;

    int GetHeight();
    int GetWidth();

    Cell ***cells;

    CREATE_FUNC(Map);

    /////////////////////////////////////////////////////////////
    // Neighbor cells on different directions
    //     _1_                              _4_
    //  6 /   \ 2 (windows form app)     5 /   \ 3 (phone app)
    //  5 \___/ 3                        6 \___/ 2
    //      4                                1
    /////////////////////////////////////////////////////////////
    Cell* Neighbor(const Cell &cell, const int num);

    bool isNeighbor(const Cell &cellA, const Cell &cellB);

    void Reset(const MapData &mapData);  

    void Draw();

    static bool IsOdd(const int num);

private:
    int height;
    int width;

    Cell* N1(const Cell &cell);
    Cell* N2(const Cell &cell);
    Cell* N3(const Cell &cell);
    Cell* N4(const Cell &cell);
    Cell* N5(const Cell &cell);
    Cell* N6(const Cell &cell);

    void InitializeMap(const MapData &mapData);
    void InitializeColor();
};

#endif