#include "SolveWizard.h"
#include "GameManager.h"
#include "Map.h"

int SolveWizard::SwapCells(const int* swapRequest)
{
    int resolved = 0;
    Cell *cellA = this->gameManager->map->cells[swapRequest[0]][swapRequest[1]];
    Cell *cellB = this->gameManager->map->cells[swapRequest[2]][swapRequest[3]];
    if (this->gameManager->map->isNeighbor(*cellA, *cellB))
    {
        GemColor temp = cellA->GetColor();
        cellA->SetColor(cellB->GetColor());
        cellB->SetColor(temp);
        resolved = Solve();
        if (resolved == 0)
        {
            // if swap doesn't have effect, swap cells back
            cellB->SetColor(cellA->GetColor());
            cellA->SetColor(temp);
        }
    }
    return resolved;
}

int SolveWizard::Solve()
{
    //MarkResolvableByDirection(2);
    //MarkResolvableByDirection(3);
    //MarkResolvableByDirection(4);
    return Resolve();
}

int SolveWizard::Resolve()
{
    int totalResolved = 0;
    int height = this->gameManager->map->height;
    int width = this->gameManager->map->width;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Cell* cell = this->gameManager->map->cells[i][j];
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

void SolveWizard::GenerateHeads(const int dir, Cell** &heads, int &length)
{
    switch (dir)
    {
        case 2:
            {
                length = (this->gameManager->map->width + 1) / 2 + this->gameManager->map->height;
                heads = new Cell*[length];
                int i = 0;
                for (; i < this->gameManager->map->height; i++)
                {
                    heads[i] = this->gameManager->map->cells[i][0];
                }
                for (int j = 0; j < this->gameManager->map->width; j++)
                {
                    if ((j & 1) == 1)
                    {
                        heads[i++] = this->gameManager->map->cells[this->gameManager->map->height - 1][j];
                    }
                }
                break;
            }
        case 3:
            {
                length = (this->gameManager->map->width + 1) / 2 + this->gameManager->map->height;
                heads = new Cell*[length];
                int i = 0;
                for (; i < this->gameManager->map->height; i++)
                {
                    heads[i] = this->gameManager->map->cells[i][0];
                }
                for (int j = 0; j < this->gameManager->map->width; j++)
                {
                    if ((j & 1) == 0)
                    {
                        heads[i++] = this->gameManager->map->cells[0][j];
                    }
                }
                break;
            }
        case 4:
            {
                length = this->gameManager->map->width;
                heads = new Cell*[length];
                for (int i = 0; i < length; i++)
                {
                    heads[i] = this->gameManager->map->cells[0][i];
                }
                break;
            }
    }
}

void SolveWizard::MarkResolvableByDirection(const int dir)
{
    Cell** heads = NULL;
    int headsLen = 0;
    GenerateHeads(dir, heads, headsLen);

    for (int i = 0; i < headsLen; i++)
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

void SolveWizard::Refill()
{
    for (int row = this->gameManager->map->height - 1; row > 0; row--)
    {
        for (int col = 0; col < this->gameManager->map->width; col++)
        {
            Cell *cell = this->gameManager->map->cells[row][col];
            if (this->gameManager->map->cells[row][col]->resolving == true)
            {
                Cell *upCell = this->gameManager->map->Neighbor(*cell, 1);
                if (upCell != NULL && upCell->GetColor() != GemColor::Vacant)
                {
                    cell->SetColor(upCell->GetColor());
                }
                else
                {
                    cell->SetColor(this->gameManager->map->RandomColor());
                }
                cell->resolving = false;
            }
        }
    }

    for (int col = 0; col < this->gameManager->map->width; col++)
    {
        Cell *pos = this->gameManager->map->cells[this->gameManager->map->height - 1][col];
        Cell *newPos = pos;
        while (pos != NULL && newPos != NULL)
        {
            if (pos->GetColor() != GemColor::Vacant)
            {
                pos = this->gameManager->map->Neighbor(*pos, 1);
                newPos = pos;
                continue;
            }

            if (newPos->GetColor() != GemColor::Vacant)
            {
                pos->SetColor(newPos->GetColor());
                pos = this->gameManager->map->Neighbor(*pos, 1);
                newPos->SetColor(GemColor::Vacant);
                newPos = this->gameManager->map->Neighbor(*newPos, 1);
            }
            else
            {
                newPos = this->gameManager->map->Neighbor(*newPos, 1);
            }
        }
        while (pos != NULL)
        {
            pos->SetColor(this->gameManager->map->RandomColor());
            pos = this->gameManager->map->Neighbor(*pos, 1);
        }
    }
}
