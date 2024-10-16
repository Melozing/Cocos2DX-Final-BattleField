#include "Controller/GameController.h"
#include "Controller/SoundController.h"
#include "Controller/SceneController.h"
#include "Panel/GameOverPanel.h"
#include "utils/Music/AudioUtils.h"
#include "Panel/PausePanel.h"
#include "Panel/VictoryPanel.h"
#include "Constants/Constants.h"
#include "utils/Music/MusicAnalyzer.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h" // Include the AudioEngine header

USING_NS_CC;
using namespace cocos2d::experimental; // Use the experimental namespace for AudioEngine

// Initialize the static instance
GameController* GameController::instance = nullptr;

// Private constructor for singleton
GameController::GameController() : gameTime(0.0f), gameOver(false), paused(false), audioID(AudioEngine::INVALID_AUDIO_ID), musicStarted(false) {}

// Singleton access method
GameController* GameController::getInstance() {
    if (instance == nullptr) {
        instance = new GameController();
    }
    return instance;
}

void GameController::init() {
    CCLOG("GameController initialized");

    // Initialize game state
    gameTime = 0.0f;
    gameOver = false;
    paused = false;
    audioID = AudioEngine::INVALID_AUDIO_ID; // Reset audio ID
}

// Handles the game over event
void GameController::GameOver(const std::function<void()>& exitAction, const std::function<Scene* ()>& createSceneFunc, const std::string& soundtrackPath) {
    auto& playerAttributes = PlayerAttributes::getInstance();
    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();
    MusicAnalyzer::getInstance()->stopMusic();

    if (runningScene) {
        auto retryAction = [this, director, createSceneFunc, soundtrackPath]() {
            this->resetGameState(); // Reset game state before replaying
            Scene* newScene = createSceneFunc();
            if (newScene) {
                director->replaceScene(newScene);
            }
            MusicAnalyzer::getInstance()->replayMusic(); // Replay the music
            };

        auto panel = GameOverPanel::createPanel(retryAction, exitAction);
        showEndGamePanel(panel, retryAction, soundtrackPath);
    }
}

void GameController::Victory(const std::function<void()>& exitAction, const std::function<Scene* ()>& createSceneFunc, const std::string& soundtrackPath) {
    if (gameOver) return;

    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();
    MusicAnalyzer::getInstance()->stopMusic();

    if (runningScene) {
        auto retryAction = [this, director, createSceneFunc, soundtrackPath]() {
            this->resetGameState(); // Reset game state before replaying
            Scene* newScene = createSceneFunc();
            if (newScene) {
                director->replaceScene(newScene);
            }
            MusicAnalyzer::getInstance()->replayMusic(); // Replay the music
            };

        auto victoryPanel = VictoryPanel::createPanel(retryAction, exitAction);
        showEndGamePanel(victoryPanel, retryAction, soundtrackPath);
    }
}

void GameController::pauseGame(const std::function<void()>& exitAction, const std::function<Scene* ()>& createSceneFunc, const std::string& soundtrackPath) {
    if (!paused) {
        auto director = Director::getInstance();
        auto runningScene = director->getRunningScene();
        MusicAnalyzer::getInstance()->pauseMusic();

        if (runningScene) {
            auto retryAction = [this, director, createSceneFunc, soundtrackPath]() {
                this->resetGameState(); // Reset game state before replaying
                Scene* newScene = createSceneFunc();
                if (newScene) {
                    director->replaceScene(newScene);
                }
                MusicAnalyzer::getInstance()->replayMusic(); // Replay the music
                };

            auto pausePanel = PausePanel::createPanel([this]() {
                this->resumeGame();
                }, retryAction, exitAction);

            if (pausePanel) {
                pausePanel->setName("PausePanel"); // Set a name for the pause panel
                showPausePanel(pausePanel, retryAction, soundtrackPath);
            }
            else {
                CCLOG("Failed to create PausePanel");
            }
        }
    }
}

void GameController::showEndGamePanel(Layer* panel, const std::function<void()>& retryAction, const std::string& soundtrackPath) {
    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();

    if (runningScene && panel) {
        panel->setOpacity(0);
        panel->runAction(FadeIn::create(1.0f));
        runningScene->addChild(panel, 100);

        director->pause();
        gameOver = true;
    }
}

void GameController::showPausePanel(cocos2d::ui::Layout* panel, const std::function<void()>& retryAction, const std::string& soundtrackPath) {
    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();

    if (runningScene && panel) {
        panel->setOpacity(0);
        panel->runAction(FadeIn::create(1.0f));
        runningScene->addChild(panel, 100);

        director->pause();
        paused = true;
    }
}

// Add a method to check if the game is over
bool GameController::isGameOver() const {
    return gameOver;
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
                pausePanel->removeFromParent(); // Remove the pause panel from the scene
            }
        }

        // Resume the music
        MusicAnalyzer::getInstance()->resumeMusic();
    }
}

// Add a method to check if the game is paused
bool GameController::isPaused() const {
    return paused;
}

void GameController::replayGame(const std::string& soundtrackPath) {
    this->resetGameState(); // Reset game state before replaying
    std::string currentSceneName = Director::getInstance()->getRunningScene()->getName();

    auto newScene = SceneController::getInstance()->createScene(currentSceneName);
    if (newScene) {
        Director::getInstance()->replaceScene(newScene);
    }
    MusicAnalyzer::getInstance()->replayMusic(); // Replay the music
}

// Add a method to reset the game state
void GameController::resetGameState() {
    auto director = Director::getInstance();
    director->resume(); // Ensure the game is not paused
    gameTime = 0.0f;
    gameOver = false;
    paused = false;
    audioID = AudioEngine::INVALID_AUDIO_ID; // Reset audio ID
    // Reset any other game-specific states here
}
