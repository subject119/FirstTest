#include "Cell.h"
#include "Map.h"

USING_NS_CC;

int Cell::GetRow() const
{
    return this->row;
}

int Cell::GetCol() const
{
    return this->col;
}

DIRECTION Cell::GetDirection()
{
    return this->highDir;
}

void Cell::SetDirection(const DIRECTION dir)
{
    this->highDir = dir;
    switch (dir)
    {
    case DIRECTION::DIR1:
    case DIRECTION::DIR4:
        this->setRotation(0);
        break;
    case DIRECTION::DIR2:
    case DIRECTION::DIR5:
        this->setRotation(120);
        break;
    case DIRECTION::DIR3:
    case DIRECTION::DIR6:
        this->setRotation(60);
        break;
    }
}

void Cell::Explode(){
    if (this->gemType == GemType::Normal)
    {
        return;
    }

    switch (this->gemType)
    {
    case GemType::Straight4:
        {
            this->map->MarkResolvingInDirection(this, this->highDir);
            this->map->MarkResolvingInDirection(this, Map::OppositeDirection(this->highDir));
            // don't change color
            this->SetColorGemType(this->color, GemType::Normal);
            this->SetDirection(DIRECTION::DIR1);
        }
        break;
    case GemType::Straight5:
        {
            this->SetColorGemType(this->color, GemType::Normal);
        }
        break;
    case GemType::Cross2:
        {
            this->SetColorGemType(this->color, GemType::Normal);
        }
        break;
    case GemType::Cross3:
        {
            this->SetColorGemType(this->color, GemType::Normal);
        }
        break;
    case GemType::Circle:
        {
            this->SetColorGemType(this->color, GemType::Normal);
        }
        break;
    }
}

void Cell::Initialize(Map *map, const int r, const int c, const CellType t, const GemColor color)
{
    this->map = map;
    this->color = GemColor::Vacant;
    this->gemType = GemType::Normal;
    this->highDir = DIRECTION::DIR1;
    this->resolving = 0;
    this->falling = false;
    this->type = t;
    this->row = r;
    this->col = c;
    this->fallingTime = 0;
    this->color = color;
    this->initWithTexture(GetCellTexture(this->color, this->gemType));
}

Cell* Cell::createWithTexture(Texture2D *texture)
{
    return (Cell*)Sprite::createWithTexture(texture);
}

void Cell::SetColorGemType(const GemColor color, const GemType type)
{
    this->color = color;
    this->gemType = type;
    CCTexture2D *pTexture = GetCellTexture(this->color, this->gemType);
    this->setTexture(pTexture);
}

GemColor Cell::GetColor()
{
    return this->color;
}

GemType Cell::GetGemType()
{
    return this->gemType;
}

CCTexture2D* Cell::GetCellTexture(const GemColor color, const GemType type)
{
    std::string path = "";
    switch (color)
    {
    case GemColor::Red:
        switch (type)
        {
        case GemType::Normal:
            path = "r.png";
            break;
        case GemType::Straight4:
            path = "rs4.png";
            break;
        case GemType::Straight5:
            path = "s5.png";
            break;
        case GemType::Cross2:
            path = "rc2.png";
            break;
        case GemType::Cross3:
            path = "c3.png";
            break;
        }
        break;
    case GemColor::Green:
        switch (type)
        {
        case GemType::Normal:
            path = "g.png";
            break;
        case GemType::Straight4:
            path = "gs4.png";
            break;
        case GemType::Straight5:
            path = "s5.png";
            break;
        case GemType::Cross2:
            path = "gc2.png";
            break;
        case GemType::Cross3:
            path = "c3.png";
            break;
        }
        break;
    case GemColor::Blue:
        switch (type)
        {
        case GemType::Normal:
            path = "b.png";
            break;
        case GemType::Straight4:
            path = "bs4.png";
            break;
        case GemType::Straight5:
            path = "s5.png";
            break;
        case GemType::Cross2:
            path = "bc2.png";
            break;
        case GemType::Cross3:
            path = "c3.png";
            break;
        }
        break;
    case GemColor::Vacant:
        path = "v.png";
        break;
    case GemColor::Purple:
        switch (type)
        {
        case GemType::Normal:
            path = "p.png";
            break;
        case GemType::Straight4:
            path = "ps4.png";
            break;
        case GemType::Straight5:
            path = "s5.png";
            break;
        case GemType::Cross2:
            path = "pc2.png";
            break;
        case GemType::Cross3:
            path = "c3.png";
            break;
        }
        break;
    case GemColor::Orange:
        switch (type)
        {
        case GemType::Normal:
            path = "o.png";
            break;
        case GemType::Straight4:
            path = "os4.png";
            break;
        case GemType::Straight5:
            path = "s5.png";
            break;
        case GemType::Cross2:
            path = "oc2.png";
            break;
        case GemType::Cross3:
            path = "c3.png";
            break;
        }
        break;
    case GemColor::Yellow:
        switch (type)
        {
        case GemType::Normal:
            path = "y.png";
            break;
        case GemType::Straight4:
            path = "ys4.png";
            break;
        case GemType::Straight5:
            path = "s5.png";
            break;
        case GemType::Cross2:
            path = "yc2.png";
            break;
        case GemType::Cross3:
            path = "c3.png";
            break;
        }
        break;
    }
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
    return CCTextureCache::getInstance()->getTextureForKey(fullPath);
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
    cache->addImage("rs4.png");
    cache->addImage("gs4.png");
    cache->addImage("bs4.png");
    cache->addImage("ys4.png");
    cache->addImage("os4.png");
    cache->addImage("ps4.png");
    cache->addImage("s5.png");
    cache->addImage("rc2.png");
    cache->addImage("gc2.png");
    cache->addImage("bc2.png");
    cache->addImage("yc2.png");
    cache->addImage("oc2.png");
    cache->addImage("pc2.png");
    cache->addImage("c3.png");
}
