
#include <time.h>

#include "SolveWizard.h"
#include "GameManager.h"
#include "ScoreManager.h"
#include "UIManager.h"
#include "IOManager.h"

USING_NS_CC;

SolveWizard::SolveWizard()
{
    this->fallingCount = 0;
    this->resolvingCount = 0;
    this->gameState = GameStates::Start;
}

void SolveWizard::onEnter()
{
    Node::onEnter();

    this->scheduleUpdate();
}

void SolveWizard::SolveBySwap(Cell &cellA, Cell &cellB)
{
    if (this->gameManager->map->isNeighbor(cellA, cellB))
    {
        GemColor temp = cellA.GetColor();
        cellA.SetColor(cellB.GetColor());
        cellB.SetColor(temp);
        
        if (QuickTestSolvable())
        {
            this->gameState = GameStates::Start;
        }
        else
        {
            // if swap doesn't have effect, swap cells back
            cellB.SetColor(cellA.GetColor());
            cellA.SetColor(temp);
        }
    }
}

void SolveWizard::update(float dt)
{
    switch (this->gameState)
    {
    case GameStates::Start:
        {
            this->gameManager->iOManager->setTouchEnabled(false);
            if (this->Solve() == 0)
            {
                this->gameManager->iOManager->setTouchEnabled(true);;
                this->gameState = GameStates::WaitingForUserInput;
            }
            else
            {
                this->gameState = GameStates::Resolving;
            }
        }
        break;
    case GameStates::Resolving:
        {
            if (this->resolvingCount > 0)
            {
                return;
            }
            else
            {
                this->StartToFall(DIRECTION::DIR4);
                if (this->fallingCount > 0)
                {
                    return;
                }
                else
                {
                    this->gameState = GameStates::Start;
                }
            }
        }
        break;
    }
}

int SolveWizard::Solve()
{
    MarkResolvableByDirection(DIRECTION::DIR2);
    MarkResolvableByDirection(DIRECTION::DIR3);
    MarkResolvableByDirection(DIRECTION::DIR4);
    int resolved = Resolve();
    this->gameManager->scoreManager->AddToScore(resolved);
    this->gameManager->UIManager->SetScore(this->gameManager->scoreManager->GetScore());
    return resolved;
}

bool SolveWizard::MarkResolvableByDirection(const DIRECTION dir)
{
    std::vector<Cell*> heads;
    GenerateHeads(dir, heads);
    bool marked = false;

    for (int i = 0; i < heads.size(); i++)
    {
        Cell *begin = heads[i];
        Cell *end = begin;
        int len = 0;
        while (end != NULL && end->type != CellType::Outspace)
        {
            if (begin->GetColor() == end->GetColor())
            {
                end = this->gameManager->map->Neighbor(*end, dir);
                ++len;
            }
            else
            {
                if (len > 2)
                {
                    marked = true;
                    while (begin->GetColor() != end->GetColor())
                    {
                        begin->resolving = true;
                        begin = this->gameManager->map->Neighbor(*begin, dir);
                    }
                }
                begin = end;
                len = 0;
            }
        }
        if (len > 2)
        {
            marked = true;
            while (begin != NULL)
            {
                begin->resolving = true;
                begin = this->gameManager->map->Neighbor(*begin, dir);
            }
        }
    }

    return marked;
}

int SolveWizard::Resolve()
{
    int totalResolved = 0;
    int height = this->gameManager->map->GetHeight();
    int width = this->gameManager->map->GetWidth();
    float animDuration = 0.15f;
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            Cell* cell = this->gameManager->map->cells[row][col];
            if (cell->resolving == true)
            {
                ResolveWithAnim(cell, animDuration);
                totalResolved++;
            }
        }
    }
    return totalResolved;
}

void SolveWizard::ActionResolveEnds(Cell *cell)
{
    this->resolvingCount--;
    cell->SetColor(GemColor::Vacant);
    cell->resolving = false;
    cell->setScale(1.0f);
    cell->setOpacity(255.0f);
}

void SolveWizard::ResolveWithAnim(Cell *cell, const float animDuration)
{
    this->resolvingCount++;
    auto *fadeOut = FadeOut::create(animDuration);
    auto *scale = ScaleTo::create(animDuration, 0.25f);
    auto actionResolveEnds = CallFunc::create(CC_CALLBACK_0(SolveWizard::ActionResolveEnds, this, cell));
    Sequence *seq = Sequence::create(scale, actionResolveEnds, NULL);
    cell->runAction(fadeOut);
    cell->runAction(seq);
}

void SolveWizard::StartToFall(const DIRECTION dir)
{
    long fallingTime = this->gameManager->map->GetTimer();
    for (int col = 0; col < this->gameManager->map->GetWidth(); col++)
    {
        int offset = 0; // offset == (newPos - pos) * height
        Cell *pos = this->gameManager->map->cells[0][col];
        Cell *newPos = pos;

        // find the lowest vacant cell
        while (pos != NULL && pos->GetColor() != GemColor::Vacant)
        {
            pos = this->gameManager->map->Neighbor(*pos, dir);
        }
        // now, pos->GetColor() == GemColor::Vacant

        newPos = pos;
        while (newPos != NULL)
        {
            // find the next non-vacant cell newPos
            if (newPos->GetColor() != GemColor::Vacant)
            {
                // 1. move cell pos to the cell newPos' position
                pos->setPosition(this->gameManager->map->GetCellOriginalPos(*newPos));
                // 2. set cell pos' color to the cell newPos', newPos's to vacant
                pos->SetColor(newPos->GetColor());
                newPos->SetColor(GemColor::Vacant);
                // 3. set pos as falling
                pos->falling = true; 
                pos->fallingTime = fallingTime;
                this->fallingCount++;
                // move pos and newPos to next cell
                pos = this->gameManager->map->Neighbor(*pos, dir);
                newPos = this->gameManager->map->Neighbor(*newPos, dir);
            }
            else
            {
                offset++; // pos + offset = newPos
                newPos = this->gameManager->map->Neighbor(*newPos, dir);
            }
        }
        // if all upper cells above pos are vacant
        while (pos != NULL)
        {
            // 1. move pos according to offset
            pos->setPosition(Map::CalcCellPositionByIndex(pos->GetRow() + offset, pos->GetCol()));
            // 2. assgin random color to pos
            pos->SetColor(Cell::RandomColor());
            // 3. set pos as falling
            pos->falling = true;
            pos->fallingTime = fallingTime;
            this->fallingCount++;
            // move pos to next
            pos = this->gameManager->map->Neighbor(*pos, dir);
        }
    }
}

bool SolveWizard::QuickTestSolvable()
{
    bool resolved = MarkResolvableByDirection(DIRECTION::DIR2) || 
        MarkResolvableByDirection(DIRECTION::DIR3) || 
        MarkResolvableByDirection(DIRECTION::DIR4);
    ClearResolvingFlags();
    return resolved;
}

void SolveWizard::ClearResolvingFlags()
{
    int height = this->gameManager->map->GetHeight();
    int width = this->gameManager->map->GetWidth();
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            this->gameManager->map->cells[row][col]->resolving = false;
        }
    }
}

void SolveWizard::GenerateHeads(const DIRECTION dir, std::vector<Cell*> &heads)
{
    switch (dir)
    {
    case DIRECTION::DIR2:
        {
            int i = 0;
            for (; i < this->gameManager->map->GetHeight(); i++)
            {
                heads.push_back(this->gameManager->map->cells[i][0]);
            }
            for (int j = 0; j < this->gameManager->map->GetWidth(); j++)
            {
                if (Map::IsOdd(j))
                {
                    heads.push_back(this->gameManager->map->cells[this->gameManager->map->GetHeight() - 1][j]);
                }
            }
            break;
        }
    case DIRECTION::DIR3:
        {
            int i = 0;
            for (; i < this->gameManager->map->GetHeight(); i++)
            {
                heads.push_back(this->gameManager->map->cells[i][0]);
            }
            for (int j = 0; j < this->gameManager->map->GetWidth(); j++)
            {
                if (!Map::IsOdd(j))
                {
                    heads.push_back(this->gameManager->map->cells[0][j]);
                }
            }
            break;
        }
    case DIRECTION::DIR4:
        {
            for (int i = 0; i < this->gameManager->map->GetWidth(); i++)
            {
                heads.push_back(this->gameManager->map->cells[0][i]);
            }
            break;
        }
    }
}
