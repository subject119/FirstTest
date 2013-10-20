#ifndef __SOLVEWIZARD_H__
#define __SOLVEWIZARD_H__

#include "Cell.h"

class GameManager;

class SolveWizard
{
public:
    GameManager *gameManager;

    int SolveWizard::SwapCells(Cell &cellA, Cell &cellB);

    int Solve();

    int Resolve();

    void SolveWizard::GenerateHeads(const int dir, std::vector<Cell*> &heads);

    void MarkResolvableByDirection(const int dir);

    void Refill(const int dir);

    void SolveWizard::SolveAction(Cell &cellA, Cell &cellB);
};

#endif