#include "Cell.h"

USING_NS_CC;

Cell::Cell(int r, int c, CellType t)
{
    this->color = GemColor::Vacant;
    this->resolving = false;
    this->type = t;
    this->row = r;
    this->col = c;
}