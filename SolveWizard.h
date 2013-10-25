#ifndef __SOLVEWIZARD_H__
#define __SOLVEWIZARD_H__

#include "cocos2d.h"

#include "Map.h"

class Cell;
class GameManager;

enum class GameStates
{
    Start,
    Resolving,
    WaitingForUserInput
};

class SolveWizard : public cocos2d::Node
{
public:
    SolveWizard();

    GameManager *gameManager;

    CREATE_FUNC(SolveWizard);

    void SolveBySwap(Cell &cellA, Cell &cellB);

    bool QuickTestSolvable();

    int fallingCount;

    int resolvingCount;

    void update(float dt);

    virtual void onEnter();

    GameStates gameState;

private:

    int Resolve();

    void ResolveWithAnim(Cell *cell, const float animDuration);

    void ActionResolveEnds(Cell *cell);

    void GenerateHeads(const DIRECTION dir, std::vector<Cell*> &heads);

    bool MarkResolvableByDirection(const DIRECTION dir);

    int Solve();

    void ClearResolvingFlags();

    void StartToFall(const DIRECTION dir);
};

#endif