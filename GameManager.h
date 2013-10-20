#ifndef __GAMEMANAGER_SCENE_H__
#define __GAMEMANAGER_SCENE_H__

#include "cocos2d.h"

class Map;
class SolveWizard;

class GameManager 
{
public:
    static cocos2d::Scene* scene();
    bool GameManager::init();

    Map *map;
    SolveWizard *solveWizard;
};

#endif // __GAMEMANAGER_SCENE_H__
