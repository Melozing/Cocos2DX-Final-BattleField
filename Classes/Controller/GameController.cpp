#include "Controller/GameController.h"
#include "cocos2d.h"

// Initialize the static instance
GameController* GameController::instance = nullptr;

// Singleton access method
GameController* GameController::getInstance()
{
    if (instance == nullptr)
    {
        instance = new GameController();
    }
    return instance;
}

GameController::GameController()
{
    CCLOG("GameController initialized."); // Log initialization message
}

// Handles the game over event
void GameController::GameOver(PlayerAttributes* playerAttributes)
{
    // Logic for game over (e.g., displaying a message)
    CCLOG("Game Over!"); // Log game over message

    // Log the player's final health
    if (playerAttributes) {
        CCLOG("Player's final health: %d", playerAttributes->GetHealth()); // Log final health
    }
}

// Handles victory event
void GameController::Victory()
{
    CCLOG("Victory!"); // Log victory message
}

// Updates game status based on elapsed time
void GameController::UpdateGameStatus(float elapsedTime)
{
    gameTime += elapsedTime;
    if (gameTime >= 300.0f) // Check if 5 minutes have passed
    {
        Victory(); // Call victory method
    }
}
