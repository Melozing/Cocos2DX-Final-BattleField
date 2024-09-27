#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <iostream>
#include "PlayerAttributes/PlayerAttributes.h" // Ensure to include this
#include "cocos2d.h"

class GameController
{
public:
    static GameController* getInstance(); // Singleton access method
    void GameOver(PlayerAttributes* playerAttributes, const std::function<void()>& retryAction, const std::function<void()>& exitAction); // Handles game over
    void Victory(); // Handles victory event
    void UpdateGameStatus(float elapsedTime); // Updates game status based on elapsed time

private:
    static GameController* instance; // Static instance of the singleton
    float gameTime = 0.0f; // Track game time
    void slowDownGame(); // Slow down the game gradually
};

#endif // GAMECONTROLLER_H
