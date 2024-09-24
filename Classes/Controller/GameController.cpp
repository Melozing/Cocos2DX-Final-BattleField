#include "Controller/GameController.h"

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

// Handles the game over event
void GameController::GameOver()
{
    // Logic for game over (e.g., displaying a message)
    std::cout << "Game Over!" << std::endl;
}

// Handles victory event
void GameController::Victory()
{
    std::cout << "Victory!" << std::endl;
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