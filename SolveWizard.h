#ifndef __SOLVEWIZARD_H__
#define __SOLVEWIZARD_H__

#include "Cell.h"

class GameManager;

class SolveWizard
{
public:
    GameManager *gameManager;

    void SolveBySwap(Cell &cellA, Cell &cellB);

    void AutoResolve();

private:

    int Resolve();

    void GenerateHeads(const int dir, std::vector<Cell*> &heads);

    void MarkResolvableByDirection(const int dir);

    int SwapCells(Cell &cellA, Cell &cellB);

    int Solve();

    void Refill(const int dir);
};

#endif