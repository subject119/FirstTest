#include "SolveWizard.h"
#include "GameManager.h"
#include "ScoreManager.h"
#include "UIManager.h"
#include "IOManager.h"

USING_NS_CC;

SolveWizard::SolveWizard()
{
    this->fallingCount = 0;
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
            AutoResolve();
        }
        else
        {
            // if swap doesn't have effect, swap cells back
            cellB.SetColor(cellA.GetColor());
            cellA.SetColor(temp);
        }
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

int SolveWizard::Resolve()
{
    int totalResolved = 0;
    int height = this->gameManager->map->GetHeight();
    int width = this->gameManager->map->GetWidth();
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            Cell* cell = this->gameManager->map->cells[row][col];
            if (cell->resolving == true)
            {
                cell->SetColor(GemColor::Vacant);
                totalResolved++;
            }
            cell->resolving = false;
        }
    }
    return totalResolved;
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
                if ((j & 1) == 1)
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
                if ((j & 1) == 0)
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

// TODO: different device has different coordinate system. firection might be upside down
void SolveWizard::Refill(const DIRECTION dir)
{
    for (int col = 0; col < this->gameManager->map->GetWidth(); col++)
    {
        int offset = 0; // offset == (newPos - pos) * height
        Cell *pos = this->gameManager->map->cells[0][col];
        Cell *newPos = pos;
        while (pos != NULL && newPos != NULL)
        {
            if (pos->GetColor() != GemColor::Vacant && !pos->falling)
            {
                pos = this->gameManager->map->Neighbor(*pos, dir);
                newPos = pos;
                continue;
            }
            // now we have find the first upmost vacant cell
            // pos and newPos are pointing to it

            // use newPos to find the next non-vacant cell, 
            // pos and newPos move to next cells
            if (newPos->GetColor() != GemColor::Vacant)
            {
                this->fallingCount++;
                newPos->falling = true;
                pos->falling = false;
                Fall(newPos, pos, offset);

                pos = this->gameManager->map->Neighbor(*pos, dir);
                newPos = this->gameManager->map->Neighbor(*newPos, dir);
            }
            else
            {
                newPos = this->gameManager->map->Neighbor(*newPos, dir);
                offset++;
            }
        }
        // if all upper cells above pos are vacant
        while (pos != NULL)
        {
            this->fallingCount++;
            pos->falling = false;
            Fall(NULL, pos, offset);
            pos = this->gameManager->map->Neighbor(*pos, dir);
        }
    }
}

// Move the Cell up to the start point of falling
void SolveWizard::Fall(Cell *newPos, Cell *pos, const int offset)
{
    Point toPos = this->gameManager->map->GetCellOriginalPos(*pos);
    if (newPos != NULL)
    {
        pos->setPosition(this->gameManager->map->GetCellOriginalPos(*newPos));
        pos->SetColor(newPos->GetColor());
    }
    else
    {
        Point pNewPos = this->gameManager->map->GetCellOriginalPos(*pos);
        pNewPos.y += offset * pos->getContentSize().height;
        pos->setPosition(pNewPos);
        pos->SetColor(Cell::RandomColor());
    }

    double distY = pos->getPositionY() - toPos.y;
    double speed = 100;
    double duration = distY / speed;

    MoveTo *fall = MoveTo::create(duration, toPos);
    auto actionFallEnds = CallFunc::create(CC_CALLBACK_0(SolveWizard::ActionFallEnds, this));
    Sequence *seq = Sequence::create(fall, actionFallEnds, NULL);
    pos->runAction(seq);
}

void SolveWizard::ActionFallEnds()
{
    this->fallingCount--;
}

void SolveWizard::SchedResolve(float dt)
{
    // cells are still falling, keep wait
    if (this->gameManager->solveWizard->fallingCount > 0) return;

    if (this->gameManager->solveWizard->Solve() == 0) 
    {
        // stop scheduler, auto resolve stops here
        this->unschedule(schedule_selector(SolveWizard::SchedResolve));
        this->gameManager->iOManager->setTouchEnabled(true);;
    }
    else
    {
        this->gameManager->solveWizard->Refill(DIRECTION::DIR4);
    }
}

void SolveWizard::AutoResolve()
{
    // disable user input
    this->gameManager->iOManager->setTouchEnabled(false);;
    // start auto resolving
    const float loopInterval = 0.2;
    this->schedule(schedule_selector(SolveWizard::SchedResolve), loopInterval);
}
