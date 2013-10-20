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
    GameManager *gameManagerlayer = GameManager::create();
    scene->addChild(gameManagerlayer);
    scene->addChild(gameManagerlayer->map);
    gameManagerlayer->map->Reset(*(gameManagerlayer->iOManager->GetMapData()));

    return scene;
}

// on "init" you need to initialize your instance
bool GameManager::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    map = new Map();
    map->gameManager = this;
    solveWizard = new SolveWizard();
    solveWizard->gameManager = this;
    iOManager = new IOManager();

    return true;
}