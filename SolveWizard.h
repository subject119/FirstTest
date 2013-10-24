#ifndef __SOLVEWIZARD_H__
#define __SOLVEWIZARD_H__

#include "cocos2d.h"

#include "Map.h"

class Cell;
class GameManager;

class SolveWizard : public cocos2d::Node
{
public:
    SolveWizard();

    GameManager *gameManager;

    CREATE_FUNC(SolveWizard);

    void SolveBySwap(Cell &cellA, Cell &cellB);

    void AutoResolve();

    bool QuickTestSolvable();

    int fallingCount;

private:

    int Resolve();

    void SchedResolve(float dt);

    void GenerateHeads(const DIRECTION dir, std::vector<Cell*> &heads);

    bool MarkResolvableByDirection(const DIRECTION dir);

    int Solve();

    void ClearResolvingFlags();

    void StartToFall(const DIRECTION dir);
};

#endif