#include "IOManager.h"
#include "GameManager.h"
#include "SolveWizard.h"
#include "Map.h"
#include "Cell.h"

USING_NS_CC;

bool IOManager::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    this->setTouchEnabled(true);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
}

MapData* IOManager::GetMapData()
{
    mapData = new MapData();
    mapData->height = 5;
    mapData->width = 5;
    return mapData;
}

bool IOManager::onTouchBegan(Touch* touch, Event  *event)
{
    Point mapCoord = this->gameManager->map->convertTouchToNodeSpace(touch);
    this->selectedCell = NULL;
    
    int height = this->gameManager->map->GetHeight();
    int width = this->gameManager->map->GetWidth();
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            if (this->gameManager->map->cells[row][col]->getBoundingBox().containsPoint(mapCoord))
            {
                this->selectedCell = this->gameManager->map->cells[row][col];
                break;
            }
        }
        if (this->selectedCell != NULL) break;
    }
    
    return true;
}

void IOManager::onTouchEnded(Touch* touch, Event  *event)
{
    if (this->selectedCell == NULL) return;

    Cell *targetCell = NULL;

    Point mapCoord = this->gameManager->map->convertTouchToNodeSpace(touch);
    int height = this->gameManager->map->GetHeight();
    int width = this->gameManager->map->GetWidth();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (this->gameManager->map->cells[i][j]->getBoundingBox().containsPoint(mapCoord))
            {
                targetCell = this->gameManager->map->cells[i][j];
                break;
            }
        }
        if (targetCell != NULL) break;
    }

    if (targetCell != NULL)
    {
        this->gameManager->solveWizard->SolveBySwap(*this->selectedCell, *targetCell);
    }
}