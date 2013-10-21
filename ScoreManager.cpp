#include "ScoreManager.h"
#include "GameManager.h"

ScoreManager::ScoreManager()
{
    this->score = 0;
}

int ScoreManager::CalcScore(const int numResolved)
{
    // simple calc, 3 resolve = 100, 4 == 200. 5 == 300, etc...
    int resScore = ((numResolved - 2) * 100);
    this->score += resScore;
    return resScore;
}