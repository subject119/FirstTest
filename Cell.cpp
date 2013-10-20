#include "Cell.h"

USING_NS_CC;

void Cell::init(int r, int c, CellType t)
{
    this->color = GemColor::Vacant;
    this->resolving = false;
    this->type = t;
    this->row = r;
    this->col = c;
}

Cell* Cell::create()
{
    return (Cell*)Sprite::create();
}

void Cell::SetColor(GemColor color)
{
    this->color = color;
    cocos2d::CCTexture2D *pTexture = ChooseTextureByColor(color);
    this->setTexture(pTexture);
}

GemColor Cell::GetColor()
{
    return this->color;
}

cocos2d::CCTexture2D* Cell::ChooseTextureByColor(GemColor color)
{
    cocos2d::TextureCache *cache = cocos2d::CCTextureCache::sharedTextureCache();
    cache->addImage("r.png");
    cache->addImage("g.png");
    cache->addImage("b.png");
    cache->addImage("v.png");
    cache->addImage("y.png");
    cache->addImage("w.png");
    cache->addImage("o.png");
    cache->addImage("p.png");

    std::string path = "";
    switch (color)
    {
    case GemColor::Red:
        path = "r.png";
        break;
    case GemColor::Green:
        path = "g.png";
        break;
    case GemColor::Blue:
        path = "b.png";
        break;
    case GemColor::Vacant:
        path = "v.png";
        break;
    case GemColor::Purple:
        path = "p.png";
        break;
    case GemColor::White:
        path = "w.png";
        break;
    case GemColor::Orange:
        path = "o.png";
        break;
    case GemColor::Yellow:
        path = "y.png";
        break;
    }
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
    return cocos2d::CCTextureCache::sharedTextureCache()->getTextureForKey(fullPath);
}

