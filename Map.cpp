#include "Map.h"
#include "GameManager.h"
#include "SolveWizard.h"
#include "IOManager.h"

Cell* Map::Neighbor(const Cell &cell, const int num)
{
    switch (num){
    case 1:
        return N1(cell);
    case 2:
        return N2(cell);
    case 3:
        return N3(cell);
    case 4:
        return N4(cell);
    case 5:
        return N5(cell);
    case 6:
        return N6(cell);
    default:
        return NULL;
    }
}

bool Map::isNeighbor(const Cell &cellA, const Cell &cellB)
{
    return ((Neighbor(cellA, 1) == &cellB) ||
        (Neighbor(cellA, 2) == &cellB) ||
        (Neighbor(cellA, 3) == &cellB) ||
        (Neighbor(cellA, 4) == &cellB) ||
        (Neighbor(cellA, 5) == &cellB) ||
        (Neighbor(cellA, 6) == &cellB));
}

Cell* Map::N1(const Cell &cell)
{
    if (cell.row > 0)
    {
        return cells[cell.row - 1][cell.col];
    }
    return NULL;
}

Cell* Map::N2(const Cell &cell)
{
    // even column
    if ((cell.col & 1) == 0)
    {
        if (cell.row > 0 && cell.col < this->width - 1)
        {
            return cells[cell.row - 1][cell.col + 1];
        }
    }
    // odd column
    else
    {
        if (cell.col < this->width - 1)
        {
            return cells[cell.row][cell.col + 1];
        }
    }

    return NULL;
}

Cell* Map::N3(const Cell &cell)
{
    // even column
    if ((cell.col & 1) == 0)
    {
        if (cell.col < this->width - 1)
        {
            return cells[cell.row][ cell.col + 1];
        }
    }
    // odd column
    else
    {
        if (cell.row < this->height - 1 && cell.col < this->width - 1)
        {
            return cells[cell.row + 1][cell.col + 1];
        }
    }

    return NULL;
}

Cell* Map::N4(const Cell &cell)
{
    if (cell.row < this->height - 1)
    {
        return cells[cell.row + 1][cell.col];
    }
    return NULL;
}

Cell* Map::N5(const Cell &cell)
{
    // even column
    if ((cell.col & 1) == 0)
    {
        if (cell.col > 0)
        {
            return cells[cell.row][cell.col - 1];
        }
    }
    // odd column
    else
    {
        if (cell.row < this->height - 1 && cell.col > 0)
        {
            return cells[cell.row + 1][cell.col - 1];
        }
    }

    return NULL;
}

Cell* Map::N6(const Cell &cell)
{
    // even column
    if ((cell.col & 1) == 0)
    {
        if (cell.row > 0 && cell.col > 0)
        {
            return cells[cell.row - 1][cell.col - 1];
        }
    }
    // odd column
    else
    {
        if (cell.col > 0)
        {
            return cells[cell.row][cell.col - 1];
        }
    }

    return NULL;
}

void Map::Reset(const MapData &mapData)
{
    InitializeMap(mapData);
    InitializeColor();
    AutoResolve();
}

void Map::InitializeMap(const MapData &mapData)
{
    this->height = mapData.height;
    this->width = mapData.width;
    cells = new Cell**[this->height];
    for (int i = 0; i < this->height; i++)
    {
        cells[i] = new Cell*[this->width];
    }

    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cells[i][j] = new Cell(i, j, CellType::Inspace);
        }
    }
}

void Map::InitializeColor()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (cells[i][j]->type == CellType::Inspace)
            {
                cells[i][j]->color = RandomColor();
                cells[i][j]->resolving = false;
            }
        }
    }
}

void Map::AutoResolve()
{
    while (this->gameManager->solveWizard->Solve() != 0)
    {
        this->gameManager->solveWizard->Refill();
    }
}

GemColor Map::RandomColor()
{
    return (GemColor)(rand() % (sizeof(GemColor)/sizeof(int8_t) - 1));
}

void Map::Draw()
{
    //Graphics g = e.Graphics;

    //for (int i = 0; i < height; i++)
    //{
    //    bool offset = false;
    //    System.Console.WriteLine("");
    //    for (int j = 0; j < width; j++)
    //    {
    //        // TODO: temp code to put pics into correct location
    //        // pic size 50 * 46
    //        Image newImage = ChooseImageByColor(cells[i, j].color);
    //        int X = j * 37;
    //        int Y = i * 46;
    //        if (offset) Y += 23;
    //        offset = !offset;
    //        g.DrawImage(newImage, X, Y);
    //    }
    //}
}

void Map::ChooseImageByColor(GemColor color)
{
    //switch (color)
    //{
    //case GemColor::Red:
    //    return Properties.Resources.r;
    //case GemColor::Green:
    //    return Properties.Resources.g;
    //case GemColor::Blue:
    //    return Properties.Resources.b;
    //case GemColor::Vacant:
    //    return Properties.Resources.v;
    //case GemColor::Purple:
    //    return Properties.Resources.p;
    //case GemColor::White:
    //    return Properties.Resources.w;
    //case GemColor::Orange:
    //    return Properties.Resources.o;
    //case GemColor::Yellow:
    //    return Properties.Resources.y;
    //default:
    //    return NULL;
    //}
}
