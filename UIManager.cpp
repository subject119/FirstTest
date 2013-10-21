#include "UIManager.h"
#include "GameManager.h"

#include "AppMacros.h"

USING_NS_CC;

void UIManager::InitializeUI()
{
    InitializeScoreBoard();
}

void UIManager::InitializeScoreBoard()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    this->scoreBoardLabel = LabelTTF::create("Score: 0", "Arial", TITLE_FONT_SIZE);
    this->scoreBoardLabel->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - this->scoreBoardLabel->getContentSize().height));
    this->addChild(this->scoreBoardLabel, 1);
}

void UIManager::SetScore(const int score)
{
    std::string display= "Score: ";
    display += std::to_string(score);
    this->scoreBoardLabel->setString(display.c_str());
}