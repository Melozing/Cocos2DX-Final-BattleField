#include "Controller/GameController.h"
#include "Controller/SoundController.h"
#include "Controller/SceneController.h"
#include "Panel/GameOverPanel.h"
#include "Panel/PausePanel.h"
#include "Panel/VictoryPanel.h"
#include "Constants/Constants.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;

GameController* GameController::instance = nullptr;

GameController::GameController() : gameTime(0.0f), gameOver(false), paused(false), audioID(AudioEngine::INVALID_AUDIO_ID), musicStarted(false), currentSoundtrackPath("") {}

GameController* GameController::getInstance() {
    if (instance == nullptr) {
        instance = new GameController();
    }
    return instance;
}

void GameController::init() {
    CCLOG("GameController initialized");

    gameTime = 0.0f;
    gameOver = false;
    paused = false;
    audioID = AudioEngine::INVALID_AUDIO_ID;
    currentSoundtrackPath = "";
}

void GameController::GameOver(const std::function<void()>& exitAction, const std::function<Scene* ()>& createSceneFunc, const std::string& soundtrackPath) {
    auto& playerAttributes = PlayerAttributes::getInstance();
    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();
    SoundController::getInstance()->stopMusic(soundtrackPath);

    if (runningScene) {
        auto retryAction = [this, director, createSceneFunc, soundtrackPath]() {
            this->resetGameState();
            Scene* newScene = createSceneFunc();
            if (newScene) {
                director->replaceScene(newScene);
            }
            };

        auto panel = GameOverPanel::createPanel(retryAction, exitAction);
        showEndGamePanel(panel, retryAction, soundtrackPath);
    }
}

void GameController::Victory(const std::function<void()>& exitAction, const std::function<Scene* ()>& createSceneFunc, const std::string& soundtrackPath) {
    if (gameOver) return;

    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();
    SoundController::getInstance()->stopMusic(soundtrackPath);

    if (runningScene) {
        auto retryAction = [this, director, createSceneFunc, soundtrackPath]() {
            this->resetGameState();
            Scene* newScene = createSceneFunc();
            if (newScene) {
                director->replaceScene(newScene);
            }
            };

        auto victoryPanel = VictoryPanel::createPanel(retryAction, exitAction);
        showEndGamePanel(victoryPanel, retryAction, soundtrackPath);
    }
}

void GameController::pauseGame(const std::function<void()>& exitAction, const std::function<Scene* ()>& createSceneFunc, const std::string& soundtrackPath) {
    if (!paused) {
        auto director = Director::getInstance();
        auto runningScene = director->getRunningScene();
        SoundController::getInstance()->pauseMusic(soundtrackPath);
        currentSoundtrackPath = soundtrackPath; 

        if (runningScene) {
            auto retryAction = [this, director, createSceneFunc, soundtrackPath]() {
                this->resetGameState();
                Scene* newScene = createSceneFunc();
                if (newScene) {
                    director->replaceScene(newScene);
                }
                };

            auto pausePanel = PausePanel::createPanel([this]() {
                this->resumeGame();
                }, retryAction, exitAction);

            if (pausePanel) {
                pausePanel->setName("PausePanel");
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

bool GameController::isGameOver() const {
    return gameOver;
}

void GameController::resumeGame() {
    if (paused) {
        auto director = Director::getInstance();
        director->resume();
        paused = false;

        auto runningScene = director->getRunningScene();
        if (runningScene) {
            auto pausePanel = runningScene->getChildByName("PausePanel");
            if (pausePanel) {
                pausePanel->removeFromParent();
            }
        }

        SoundController::getInstance()->resumeMusic(currentSoundtrackPath);
    }
}

bool GameController::isPaused() const {
    return paused;
}

void GameController::replayGame(const std::string& soundtrackPath) {
    this->resetGameState();
    std::string currentSceneName = Director::getInstance()->getRunningScene()->getName();

    auto newScene = SceneController::getInstance()->createScene(currentSceneName);
    if (newScene) {
        Director::getInstance()->replaceScene(newScene);
    }
}

void GameController::resetGameState() {
    auto director = Director::getInstance();
    director->resume();
    gameTime = 0.0f;
    gameOver = false;
    paused = false;
    audioID = AudioEngine::INVALID_AUDIO_ID;
    currentSoundtrackPath = "";
}

