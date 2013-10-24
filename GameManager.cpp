#include "GameManager.h"
#include "AppMacros.h"

#include "event_dispatcher/CCEventListenerTouch.h"

#include "..\proj.win32\Map.h"
#include "..\proj.win32\SolveWizard.h"
#include "..\proj.win32\IOManager.h"
#include "..\proj.win32\Cell.h"
#include "..\proj.win32\ScoreManager.h"
#include "..\proj.win32\UIManager.h"

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

    this->solveWizard = SolveWizard::create();
    this->addChild(this->solveWizard);
    this->solveWizard->gameManager = this;

    this->scoreManager = new ScoreManager();
    this->scoreManager->gameManager = this;

    this->map = Map::create();
    this->map->gameManager = this;
    this->addChild(this->map);
    this->map->setPosition(Point::ZERO + origin + Point(150,60));

    this->iOManager = IOManager::create();
    this->iOManager->gameManager = this;
    this->addChild(this->iOManager);

    this->UIManager = UIManager::create();
    this->UIManager->gameManager = this;
    this->addChild(this->UIManager);
    this->UIManager->setPosition(Point::ZERO);

    srand (time(NULL));
    Cell::CacheCellTexture();
    this->UIManager->InitializeUI();
    this->map->Reset(*(this->iOManager->GetMapData())); 
    this->scoreManager->StartRecording();

    return true;
}

long GameManager::GetTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec * 1000 + now.tv_sec / 1000);
}