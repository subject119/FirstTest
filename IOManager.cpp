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
    mapData->height = 9;
    mapData->width = 9;
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

void IOManager::onTouchMoved(Touch* touch, Event  *event)
{
    if (this->selectedCell == NULL) return;

    Point mapCoord = this->gameManager->map->convertTouchToNodeSpace(touch);
    float dist = mapCoord.getDistance(this->selectedCell->getPosition());
    float threshold = this->selectedCell->getContentSize().width / 2.0;
    if (dist < threshold) return;
    
    float radian = (mapCoord - this->selectedCell->getPosition()).getAngle();
    float angle = CC_RADIANS_TO_DEGREES(radian);
    DIRECTION target;
    if (angle > 0 && angle < 60) 
    {
        target = DIRECTION::DIR3;
    }
    else if (angle > -180 && angle < -120) 
    {
        target = DIRECTION::DIR6;
    }
    else if (angle > 120 && angle < 180)
    {
        target = DIRECTION::DIR5;
    }
    else if (angle > -60 && angle < 0)
    {
        target = DIRECTION::DIR2;
    }
    else if (angle > 60 && angle < 120){
        target = DIRECTION::DIR4;
    }
    else if (angle > -120 && angle < -60){
        target = DIRECTION::DIR1;
    }

    this->gameManager->solveWizard->SolveBySwap(*this->selectedCell, 
        *this->gameManager->map->Neighbor(*this->selectedCell, target));
}

void IOManager::onTouchEnded(Touch* touch, Event  *event)
{
    //if (this->selectedCell == NULL) return;
    //this->selectedCell->setPosition(Map::CalcCellPositionByIndex(this->selectedCell->GetRow(), this->selectedCell->GetCol()));

    //Cell *targetCell = NULL;

    //Point mapCoord = this->gameManager->map->convertTouchToNodeSpace(touch);
    //int height = this->gameManager->map->GetHeight();
    //int width = this->gameManager->map->GetWidth();
    //for (int i = 0; i < height; i++)
    //{
    //    for (int j = 0; j < width; j++)
    //    {
    //        if (this->gameManager->map->cells[i][j]->getBoundingBox().containsPoint(mapCoord))
    //        {
    //            targetCell = this->gameManager->map->cells[i][j];
    //            break;
    //        }
    //    }
    //    if (targetCell != NULL) break;
    //}

    //if (targetCell != NULL)
    //{
    //    this->gameManager->solveWizard->SolveBySwap(*this->selectedCell, *targetCell);
    //}
}