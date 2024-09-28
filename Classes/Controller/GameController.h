#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "PlayerAttributes/PlayerAttributes.h"
#include "cocos2d.h"

class GameController {
public:
    static GameController* getInstance(); // Singleton access method

    void GameOver(PlayerAttributes* playerAttributes,
        const std::function<void()>& exitAction,
        const std::function<cocos2d::Scene* ()>& createSceneFunc); // Handles game over event

    void Victory(); // Handles victory event
    void UpdateGameStatus(float elapsedTime); // Updates game status based on elapsed time

private:
    GameController(); // Private constructor for singleton
    static GameController* instance; // Static instance for singleton
    float gameTime = 0.0f; // Variable to track game time

    void slowDownGame(); // Slow down the game gradually
};

#endif // GAMECONTROLLER_H
