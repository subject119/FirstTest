#include "GameManager.h"
#include "AppMacros.h"

#include "event_dispatcher/CCEventListenerTouch.h"

#include "..\proj.win32\Map.h"
#include "..\proj.win32\SolveWizard.h"
#include "..\proj.win32\IOManager.h"

USING_NS_CC;

Scene* GameManager::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    auto *ioLayer = IOManager::create();
    scene->addChild(ioLayer);

    return scene;
}

// on "init" you need to initialize your instance
bool GameManager::init()
{
    map = new Map();
    map->gameManager = this;
    solveWizard = new SolveWizard();
    solveWizard->gameManager = this;
    
    return true;
}