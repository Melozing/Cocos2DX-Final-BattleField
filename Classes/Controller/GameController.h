#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "PlayerAttributes/PlayerAttributes.h"

class GameController {
public:
    static GameController* getInstance();

    void GameOver(const std::function<void()>& exitAction, const std::function<cocos2d::Scene* ()>& createSceneFunc, const std::string& soundtrackPath);
    void Victory(const std::string& soundtrackPath);
    void UpdateGameStatus(float elapsedTime);
    bool isGameOver() const;
    void pauseGame();
    void resumeGame();
    bool isPaused() const;
    void replayGame();
    void replayGame(const std::string& soundtrackPath);

private:
    GameController();
    static GameController* instance;
    void resetGameState();
    float gameTime;
    bool gameOver;
    bool paused;
};

#endif // __GAME_CONTROLLER_H__
