#ifndef __SOLVEWIZARD_H__
#define __SOLVEWIZARD_H__

#include "Cell.h"

class GameManager;

class SolveWizard
{
public:
    GameManager *gameManager;

    int SwapCells(const int* swapRequest);

    int Solve();

    int Resolve();

    void GenerateHeads(const int dir, Cell** &heads, int &length);

    void MarkResolvableByDirection(const int dir);

    void Refill();
};

#endif