#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <iostream>
#include "PlayerAttributes/PlayerAttributes.h" // Make sure to include this

class GameController
{
public:
    static GameController* getInstance(); // Singleton access method
    void GameOver(PlayerAttributes* playerAttributes); // Pass player attributes to handle game over
    void Victory();   // Handles victory event
    void UpdateGameStatus(float elapsedTime); // Updates game status based on elapsed time

private:
    GameController(); // Private constructor for singleton
    static GameController* instance; // Static instance of the singleton
    float gameTime = 0.0f; // Track game time
};

#endif // GAMECONTROLLER_H
