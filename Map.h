#ifndef __MAP_H__
#define __MAP_H__

#include "Cell.h"
#include <array>

class GameManager;
class MapData;

class Map
{
public:
    GameManager *gameManager;

    int height;
    int width;

    Cell ***cells;

    ///////////////////////////////////////////////////
    // Neighbor cells on different directions
    //     _1_ 
    //  6 /   \ 2
    //  5 \___/ 3
    //      4
    ///////////////////////////////////////////////////
    Cell* Neighbor(const Cell &cell, const int num);

    bool isNeighbor(const Cell &cellA, const Cell &cellB);

    void Reset(const MapData &mapData);  

    void Draw();

    GemColor RandomColor();

private:
    Cell* N1(const Cell &cell);
    Cell* N2(const Cell &cell);
    Cell* N3(const Cell &cell);
    Cell* N4(const Cell &cell);
    Cell* N5(const Cell &cell);
    Cell* N6(const Cell &cell);

    void InitializeMap(const MapData &mapData);
    void InitializeColor();
    void AutoResolve();
    void ChooseImageByColor(GemColor color);
};

#endif