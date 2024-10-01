#include "Panel/GameOverPanel.h"
#include "Controller/GameController.h"
#include "cocos2d.h"

USING_NS_CC;

// Initialize the static instance
GameController* GameController::instance = nullptr;

// Private constructor for singleton
GameController::GameController() : gameTime(0.0f) {
    // Initialization code can go here if needed
}

// Singleton access method
GameController* GameController::getInstance() {
    if (instance == nullptr) {
        instance = new GameController();
    }
    return instance;
}

// Handles the game over event
void GameController::GameOver(PlayerAttributes* playerAttributes, const std::function<void()>& exitAction, const std::function<Scene* ()>& createSceneFunc) {
    if (playerAttributes == nullptr) {
        CCLOG("PlayerAttributes is null!");
        return;
    }

    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();

    if (runningScene) {
        auto retryAction = [director, createSceneFunc]() {
            Scene* newScene = createSceneFunc(); // Call the passed scene creation function
            if (newScene) {
                director->replaceScene(newScene); // Replace with the new scene
            }
            };

        // Create the GameOverPanel with the provided callbacks
        auto panel = GameOverPanel::createPanel(retryAction, exitAction);
        if (panel) {
            runningScene->addChild(panel, 100); // Ensure it's on top
            panel->setOpacity(0); // Start invisible
            panel->runAction(FadeIn::create(1.0f)); // Fade in over 1 second
        }

        // Slow down the game
        this->slowDownGame();
    }
}

// Handles victory event
void GameController::Victory() {
    CCLOG("Victory!"); // Log victory message
}

// Updates game status based on elapsed time
void GameController::UpdateGameStatus(float elapsedTime) {
    gameTime += elapsedTime;
    if (gameTime >= 300.0f) { // Check if 5 minutes have passed
        Victory(); // Call victory method
    }
}

// Slow down the game gradually
void GameController::slowDownGame() {
    float slowDuration = 3.0f; // Duration of the slowdown
    float initialSpeed = Director::getInstance()->getAnimationInterval();
    float finalSpeed = initialSpeed * 2.0f; // Final speed (2x slower)

    // Schedule a lambda function to decrease speed
    auto director = Director::getInstance();
    director->getScheduler()->schedule([=](float dt) mutable {
        float currentSpeed = director->getAnimationInterval();
        if (currentSpeed < finalSpeed) {
            director->setAnimationInterval(currentSpeed + (finalSpeed - initialSpeed) * dt / slowDuration);
        }
        else {
            director->setAnimationInterval(finalSpeed);
            director->getScheduler()->unschedule("slowDown", this); // Stop the scheduler when done
        }
        }, this, 0.0f, false, "slowDown");
}
