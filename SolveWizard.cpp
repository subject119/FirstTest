#include "SolveWizard.h"
#include "GameManager.h"
#include "ScoreManager.h"
#include "Map.h"

void SolveWizard::SolveBySwap(Cell &cellA, Cell &cellB)
{
    int resolved = SwapCells(cellA, cellB);
    if (resolved > 0) AutoResolve();
}

int SolveWizard::SwapCells(Cell &cellA, Cell &cellB)
{
    int resolved = 0;
    if (this->gameManager->map->isNeighbor(cellA, cellB))
    {
        GemColor temp = cellA.GetColor();
        cellA.SetColor(cellB.GetColor());
        cellB.SetColor(temp);
        resolved = Solve();
        if (resolved == 0)
        {
            // if swap doesn't have effect, swap cells back
            cellB.SetColor(cellA.GetColor());
            cellA.SetColor(temp);
        }
    }
    return resolved;
}

int SolveWizard::Solve()
{
    MarkResolvableByDirection(2);
    MarkResolvableByDirection(3);
    MarkResolvableByDirection(4);
    int resolved = Resolve();
    this->gameManager->scoreManager->CalcScore(resolved);
    return resolved;
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

void SolveWizard::GenerateHeads(const int dir, std::vector<Cell*> &heads)
{
    switch (dir)
    {
        case 2:
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
        case 3:
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
        case 4:
            {
                for (int i = 0; i < this->gameManager->map->GetWidth(); i++)
                {
                    heads.push_back(this->gameManager->map->cells[0][i]);
                }
                break;
            }
    }
}

void SolveWizard::MarkResolvableByDirection(const int dir)
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
            while (begin != NULL)
            {
                begin->resolving = true;
                begin = this->gameManager->map->Neighbor(*begin, dir);
            }
        }
    }
}

// TODO: different device has different coordinate system. firection might be upside down
void SolveWizard::Refill(const int dir)
{
    for (int col = 0; col < this->gameManager->map->GetWidth(); col++)
    {
        Cell *pos = this->gameManager->map->cells[0][col];
        Cell *newPos = pos;
        while (pos != NULL && newPos != NULL)
        {
            if (pos->GetColor() != GemColor::Vacant)
            {
                pos = this->gameManager->map->Neighbor(*pos, dir);
                newPos = pos;
                continue;
            }
            // now we have find the first upmost vacant cell , pos and newPos are pointing to it

            // use newPos to find the next non-vacant cell, 
            // pos and newPos move to next cells
            if (newPos->GetColor() != GemColor::Vacant)
            {
                pos->SetColor(newPos->GetColor());
                newPos->SetColor(GemColor::Vacant);
                pos = this->gameManager->map->Neighbor(*pos, dir);
                newPos = this->gameManager->map->Neighbor(*newPos, dir);
            }
            else
            {
                newPos = this->gameManager->map->Neighbor(*newPos, dir);
            }
        }
        // if all upper cells above pos are vacant
        while (pos != NULL)
        {
            pos->SetColor(Cell::RandomColor());
            pos = this->gameManager->map->Neighbor(*pos, dir);
        }
    }
}

void SolveWizard::AutoResolve()
{
    this->gameManager->solveWizard->Refill(4);
    while (this->gameManager->solveWizard->Solve() != 0)
    {
        this->gameManager->solveWizard->Refill(4);
    }
}