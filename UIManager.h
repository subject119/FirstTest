#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "cocos2d.h"

class GameManager;

class UIManager : public cocos2d::Layer
{
public:
    GameManager *gameManager;

    virtual ~UIManager() {}

    CREATE_FUNC(UIManager);

    Layer *scoreBoardLayer;

    void InitializeUI();

    void SetScore(const int score);

private:
    cocos2d::LabelTTF *scoreBoardLabel;

    void InitializeScoreBoard();
};

#endif
