#include "Cell.h"

USING_NS_CC;

void Cell::init(const int r, const int c, const CellType t)
{
    this->color = GemColor::Vacant;
    this->resolving = false;
    this->type = t;
    this->row = r;
    this->col = c;
}

Cell* Cell::createWithTexture(Texture2D *texture)
{
    return (Cell*)Sprite::createWithTexture(texture);
}

void Cell::SetColor(const GemColor color)
{
    this->color = color;
    CCTexture2D *pTexture = ChooseTextureByColor(color);
    this->setTexture(pTexture);
}

GemColor Cell::GetColor()
{
    return this->color;
}

CCTexture2D* Cell::ChooseTextureByColor(const GemColor color)
{
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
    case GemColor::Orange:
        path = "o.png";
        break;
    case GemColor::Yellow:
        path = "y.png";
        break;
    }
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
    return CCTextureCache::sharedTextureCache()->getTextureForKey(fullPath);
}

GemColor Cell::RandomColor()
{
    // don't generate Vacant, which is 0. so range is 1-6
    return (GemColor)((rand() % 6) + 1);
}

void Cell::CacheCellTexture()
{
    // TODO: temporary put it here
    TextureCache *cache = TextureCache::sharedTextureCache();
    cache->addImage("r.png");
    cache->addImage("g.png");
    cache->addImage("b.png");
    cache->addImage("v.png");
    cache->addImage("y.png");
    cache->addImage("o.png");
    cache->addImage("p.png");
}
