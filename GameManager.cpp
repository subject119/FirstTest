#include "GameManager.h"
#include "AppMacros.h"

#include "event_dispatcher/CCEventListenerTouch.h"

#include "..\proj.win32\Map.h"
#include "..\proj.win32\SolveWizard.h"
#include "..\proj.win32\IOManager.h"
#include "..\proj.win32\Cell.h"

USING_NS_CC;

Scene* GameManager::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto *gameManagerlayer = GameManager::create();
    scene->addChild(gameManagerlayer);

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

    this->map = Map::create();
    this->map->gameManager = this;
    this->iOManager = IOManager::create();
    this->iOManager->gameManager = this;
    this->solveWizard = new SolveWizard();
    this->solveWizard->gameManager = this;

    this->addChild(this->map);
    this->addChild(this->iOManager);

    Cell::CacheCellTexture();
    this->map->setPosition(Point::ZERO + origin + Point(150,60));
    this->map->Reset(*(this->iOManager->GetMapData()));    

    return true;
}