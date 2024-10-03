#include "Controller/GameController.h"
#include "Controller/SceneController.h"
#include "Panel/GameOverPanel.h"
#include "Panel/PausePanel.h"
#include "Panel/VictoryPanel.h" 
#include "Constants/Constants.h" 
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

// Initialize the static instance
GameController* GameController::instance = nullptr;

// Private constructor for singleton
GameController::GameController() : gameTime(0.0f), gameOver(false), paused(false) {}

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
        auto retryAction = [this, director, createSceneFunc]() {
            this->resetGameState(); // Reset game state before replaying
            Scene* newScene = createSceneFunc();
            if (newScene) {
                director->replaceScene(newScene);
            }
            };

        auto panel = GameOverPanel::createPanel(retryAction, exitAction);
        if (panel) {
            runningScene->addChild(panel, 100);
            panel->setOpacity(0);
            panel->runAction(FadeIn::create(1.0f));
        }

        director->pause(); // Directly pause the game without showing the pause panel
        gameOver = true;
    }
}

// Handles victory event
void GameController::Victory() {
    if (gameOver) return;

    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();

    if (runningScene) {
        auto victoryPanel = VictoryPanel::createPanel([this]() {
            this->replayGame();
            }, []() {
                Director::getInstance()->end();
                });

        if (victoryPanel) {
            runningScene->addChild(victoryPanel, 100);
            victoryPanel->setOpacity(0);
            victoryPanel->runAction(FadeIn::create(1.0f));
        }

        director->pause();
        gameOver = true;
    }
}

// Updates game status based on elapsed time
void GameController::UpdateGameStatus(float elapsedTime) {
    gameTime += elapsedTime;
    if (gameTime >= Constants::TIME_TO_WIN) {
        Victory();
    }
}

// Add a method to check if the game is over
bool GameController::isGameOver() const {
    return gameOver;
}

// Add a method to pause the game
void GameController::pauseGame() {
    if (!paused) {
        auto director = Director::getInstance();
        auto runningScene = director->getRunningScene();

        if (runningScene) {
            // Check if the pause panel already exists
            auto pausePanel = runningScene->getChildByName("PausePanel");
            if (!pausePanel) {
                // Create and show the pause panel
                pausePanel = PausePanel::createPanel([this]() {
                    this->resumeGame();
                    });
                if (pausePanel) {
                    runningScene->addChild(pausePanel, 100);
                    pausePanel->setOpacity(0);
                    pausePanel->runAction(FadeIn::create(1.0f));
                }
            }
            else {
                // Show the existing pause panel
                pausePanel->setVisible(true);
            }

            director->pause();
            paused = true;
        }
    }
}

// Add a method to resume the game
void GameController::resumeGame() {
    if (paused) {
        auto director = Director::getInstance();
        director->resume();
        paused = false;

        // Hide the pause panel
        auto runningScene = director->getRunningScene();
        if (runningScene) {
            auto pausePanel = runningScene->getChildByName("PausePanel");
            if (pausePanel) {
                pausePanel->setVisible(false);
            }
        }
    }
}

// Add a method to check if the game is paused
bool GameController::isPaused() const {
    return paused;
}

// Add a method to replay the game
void GameController::replayGame() {
    this->resetGameState(); // Reset game state before replaying
    std::string currentSceneName = Director::getInstance()->getRunningScene()->getName();

    auto newScene = SceneController::getInstance()->createScene(currentSceneName);
    if (newScene) {
        Director::getInstance()->replaceScene(newScene);
    }
}

// Add a method to reset the game state
void GameController::resetGameState() {
    auto director = Director::getInstance();
    director->resume(); // Ensure the game is not paused
    gameTime = 0.0f;
    gameOver = false;
    paused = false;
    // Reset any other game-specific states here
}
