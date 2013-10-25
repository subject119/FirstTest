
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
            if (this->Resolve() == 0)
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

int SolveWizard::Resolve()
{
    this->resolvingCells.clear();
    MarkResolvableByDirection(DIRECTION::DIR2);
    MarkResolvableByDirection(DIRECTION::DIR3);
    MarkResolvableByDirection(DIRECTION::DIR4);
    Resolving();

    // TODO: temp, simple, score calculating
    int resolved = 0;
    for (auto i : this->resolvingCells)
    {
        resolved += i.size();
    }
    this->gameManager->scoreManager->AddToScore(resolved);
    this->gameManager->UIManager->SetScore(this->gameManager->scoreManager->GetScore());

    return resolved;
}

bool SolveWizard::MarkResolvableByDirection(const DIRECTION dir)
{
    std::vector<Cell*> heads;
    GenerateHeads(dir, heads);

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
                    this->resolvingCells.push_back(std::vector<Cell *>());
                    while (begin->GetColor() != end->GetColor())
                    {
                        this->resolvingCells.back().push_back(begin);
                        begin->resolving++;
                        begin = this->gameManager->map->Neighbor(*begin, dir);
                    }
                }
                begin = end;
                len = 0;
            }
        }
        if (len > 2)
        {
            this->resolvingCells.push_back(std::vector<Cell *>());
            while (begin != NULL)
            {
                this->resolvingCells.back().push_back(begin);
                begin->resolving++;
                begin = this->gameManager->map->Neighbor(*begin, dir);
            }
        }
    }

    return this->resolvingCells.size();
}

void SolveWizard::Resolving()
{
    float animDuration = 0.15f;
    for (auto i : this->resolvingCells)
    {
        for (auto j : i)
        {
            ResolveWithAnim(j, animDuration);

            if (j->resolving == 2)
            {
                
            }
            if (j->resolving == 3)
            {
                
            }
            if (i.size() == 4)
            {
            }
            if (i.size() == 5)
            {
            }
        }
    }
}

void SolveWizard::ActionResolveEnds(Cell *cell)
{
    this->resolvingCount--;
    cell->SetColor(GemColor::Vacant);
    cell->resolving = 0;
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
    unsigned int fallingTime = this->gameManager->map->GetTimer();
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
    this->resolvingCells.clear();
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
            this->gameManager->map->cells[row][col]->resolving = 0;
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
