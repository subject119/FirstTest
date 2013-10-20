#ifndef __GAMEMANAGER_SCENE_H__
#define __GAMEMANAGER_SCENE_H__

#include "cocos2d.h"

class Map;
class SolveWizard;
class IOManager;

class GameManager : public cocos2d::Layer
{
public:
    virtual ~GameManager() {}

    static cocos2d::Scene* scene();
    virtual bool init();

    CREATE_FUNC(GameManager);

    Map *map;
    SolveWizard *solveWizard;
    IOManager *iOManager;
};

#endif // __GAMEMANAGER_SCENE_H__
