#ifndef __SOLVEWIZARD_H__
#define __SOLVEWIZARD_H__

#include "cocos2d.h"

#include "Map.h"

class Cell;
class GameManager;

enum class GameStates
{
    Start,
    ExplodingHighGems,
    Resolving,
    WaitingForUserInput
};

class Resolvable
{
public:
    std::vector<Cell *> gems;
    DIRECTION direction;
};

class SolveWizard : public cocos2d::Node
{
public:
    SolveWizard();

    GameManager *gameManager;

    CREATE_FUNC(SolveWizard);

    void SolveBySwap(Cell &cellA, DIRECTION dir);

    bool QuickTestSolvable();

    int fallingCount;

    int resolvingCount;

    void update(float dt);

    virtual void onEnter();

    GameStates gameState;

    Cell *swapCells[2];

    std::queue<Cell *> explosiveHighGems;

    void GenerateNewHighGem();

    bool ExplodeNowBySwap(Cell *cellA, Cell *cellB);

private:

    void Resolve();

    void ResolveWithAnim(Cell *cell, const float animDuration);

    void ActionResolveEnds(Cell *cell);

    void GenerateHeads(const DIRECTION dir, std::vector<Cell*> &heads);

    bool MarkResolvableByDirection(const DIRECTION dir);

    void ExplodeExplosiveHighGems();

    int ResolveGems();

    void ClearResolvingFlags();

    void StartToFall(const DIRECTION dir);

    Cell* GetCause(Resolvable &straight);

    void MarkStraights();

    std::vector<Resolvable> resolvingCells;

    DIRECTION swapDir;

    Cell* LowerGem(Cell *cellA, Cell *cellB);
    Cell* HigherGem(Cell *cellA, Cell *cellB);
};

#endif