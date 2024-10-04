#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "PlayerAttributes/PlayerAttributes.h"

class GameController {
public:
    static GameController* getInstance();

    void GameOver(const std::function<void()>& exitAction, const std::function<cocos2d::Scene* ()>& createSceneFunc);
    void Victory();
    void UpdateGameStatus(float elapsedTime);
    bool isGameOver() const;
    void pauseGame();
    void resumeGame();
    bool isPaused() const;
    void replayGame();

private:
    GameController();
    static GameController* instance;
    float gameTime;
    bool gameOver;
    bool paused;

    void resetGameState();
};

#endif // __GAME_CONTROLLER_H__
