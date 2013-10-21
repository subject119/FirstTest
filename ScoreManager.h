#ifndef __SCOREMANAGER_H__
#define __SCOREMANAGER_H__

class GameManager;

class ScoreManager
{
public:
    ScoreManager();
    virtual ~ScoreManager() {}

    GameManager *gameManager;

    void AddToScore(const int numResolved);
    int GetScore();

    void StartRecording();

    void StopRecording();

private:
    int score;
    bool recording;
};

#endif