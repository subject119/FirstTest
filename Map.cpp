#include "Map.h"
#include "GameManager.h"
#include "SolveWizard.h"
#include "IOManager.h"

USING_NS_CC;

bool Map::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
}


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
    if (cell.GetRow() > 0)
    {
        return cells[cell.GetRow() - 1][cell.GetCol()];
    }
    return NULL;
}

Cell* Map::N2(const Cell &cell)
{
    // even column
    if (!IsOdd(cell.GetCol()))
    {
        if (cell.GetRow() > 0 && cell.GetCol() < this->width - 1)
        {
            return cells[cell.GetRow() - 1][cell.GetCol() + 1];
        }
    }
    // odd column
    else
    {
        if (cell.GetCol() < this->width - 1)
        {
            return cells[cell.GetRow()][cell.GetCol() + 1];
        }
    }

    return NULL;
}

Cell* Map::N3(const Cell &cell)
{
    // even column
    if (!IsOdd(cell.GetCol()))
    {
        if (cell.GetCol() < this->width - 1)
        {
            return cells[cell.GetRow()][ cell.GetCol() + 1];
        }
    }
    // odd column
    else
    {
        if (cell.GetRow() < this->height - 1 && cell.GetCol() < this->width - 1)
        {
            return cells[cell.GetRow() + 1][cell.GetCol() + 1];
        }
    }

    return NULL;
}

Cell* Map::N4(const Cell &cell)
{
    if (cell.GetRow() < this->height - 1)
    {
        return cells[cell.GetRow() + 1][cell.GetCol()];
    }
    return NULL;
}

Cell* Map::N5(const Cell &cell)
{
    // even column
    if (!IsOdd(cell.GetCol()))
    {
        if (cell.GetCol() > 0)
        {
            return cells[cell.GetRow()][cell.GetCol() - 1];
        }
    }
    // odd column
    else
    {
        if (cell.GetRow() < this->height - 1 && cell.GetCol() > 0)
        {
            return cells[cell.GetRow() + 1][cell.GetCol() - 1];
        }
    }

    return NULL;
}

Cell* Map::N6(const Cell &cell)
{
    // even column
    if (!IsOdd(cell.GetCol()))
    {
        if (cell.GetRow() > 0 && cell.GetCol() > 0)
        {
            return cells[cell.GetRow() - 1][cell.GetCol() - 1];
        }
    }
    // odd column
    else
    {
        if (cell.GetCol() > 0)
        {
            return cells[cell.GetRow()][cell.GetCol() - 1];
        }
    }

    return NULL;
}

void Map::Reset(const MapData &mapData)
{
    InitializeMap(mapData);
    InitializeColor();
    this->gameManager->solveWizard->AutoResolve();
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

    for (int row = 0; row < this->height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            cells[row][col] = Cell::createWithTexture(Cell::ChooseTextureByColor(GemColor::Vacant));
            cells[row][col]->init(row, col, CellType::Inspace);
            cells[row][col]->setPosition(CalcCellPositionByIndex(row, col));

            // Don't forget to add Sprites to Layer, otherwise later app crashes when doing clean up
            this->addChild(cells[row][col]);
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
                cells[i][j]->SetColor(Cell::RandomColor());
                cells[i][j]->resolving = false;
            }
        }
    }
}

int Map::GetHeight()
{
    return this->height;
}

int Map::GetWidth()
{
    return this->width;
}

bool Map::IsOdd(const int num)
{
    return ((num & 1) == 1);
}

Point Map::CalcCellPositionByIndex(const int row, const int col)
{
    //double cellWidth = cells[row][col]->getContentSize().width;
    //double cellHeight = cells[row][col]->getContentSize().height;
    //double XInterval= cellWidth * 3/4;
    //double YInterval = cellHeight * sqrt(3.0);

    // hard code numbers here: image 50*43 pixels
    double XInterval = 18.75;
    double YInterval = 21.6506351;

    double X = XInterval * col;
    double Y = YInterval * row;

    // if it is odd column, move cells up
    if (IsOdd(col)) Y += (YInterval / 2.0);

    return Point(X, Y);
}