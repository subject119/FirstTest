#ifndef __SCOREMANAGER_H__
#define __SCOREMANAGER_H__

class GameManager;

class ScoreManager
{
public:
    ScoreManager();
    virtual ~ScoreManager() {}

    GameManager *gameManager;

    int CalcScore(const int numResolved);

private:
    int score;
};

#endif