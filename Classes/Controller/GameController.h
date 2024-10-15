#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "PlayerAttributes/PlayerAttributes.h"

class GameController {
public:
    static GameController* getInstance();

    void GameOver(const std::function<void()>& exitAction, const std::function<cocos2d::Scene* ()>& createSceneFunc, const std::string& soundtrackPath);
    void Victory(const std::function<void()>& exitAction, const std::function<cocos2d::Scene* ()>& createSceneFunc, const std::string& soundtrackPath);
    bool isGameOver() const;
    void pauseGame(const std::function<void()>& exitAction, const std::function<cocos2d::Scene* ()>& createSceneFunc, const std::string& soundtrackPath);
    void resumeGame();
    bool isPaused() const;
    void replayGame(const std::string& soundtrackPath);
    void init();

private:
    GameController();
    static GameController* instance;
    void resetGameState();
    void showEndGamePanel(cocos2d::Layer* panel, const std::function<void()>& retryAction, const std::string& soundtrackPath);
    float gameTime;
    bool gameOver;
    bool paused;
};

#endif // __GAME_CONTROLLER_H__
