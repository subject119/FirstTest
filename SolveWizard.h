#ifndef __SOLVEWIZARD_H__
#define __SOLVEWIZARD_H__

#include "cocos2d.h"

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

private:
    int fallingCount;

    int Resolve();

    void SchedResolve(float dt);

    void GenerateHeads(const int dir, std::vector<Cell*> &heads);

    bool MarkResolvableByDirection(const int dir);

    int Solve();

    void Refill(const int dir);

    void Fall(Cell *newPos, Cell *Pos, const int offset);
    
    void ActionFallEnds();
};

#endif