#include "Controller/GameController.h"
#include "Controller/SceneController.h"
#include "Panel/GameOverPanel.h"
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
        auto retryAction = [director, createSceneFunc]() {
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

        this->slowDownGame();
        gameOver = true;
    }
}

// Handles victory event
void GameController::Victory() {
    CCLOG("Victory!");
}

// Updates game status based on elapsed time
void GameController::UpdateGameStatus(float elapsedTime) {
    gameTime += elapsedTime;
    if (gameTime >= 300.0f) {
        Victory();
    }
}

// Slow down the game gradually
void GameController::slowDownGame() {
    float slowDuration = 3.0f;
    float initialSpeed = Director::getInstance()->getAnimationInterval();
    float finalSpeed = initialSpeed * 2.0f;

    auto director = Director::getInstance();
    director->getScheduler()->schedule([=](float dt) mutable {
        float currentSpeed = director->getAnimationInterval();
        if (currentSpeed < finalSpeed) {
            director->setAnimationInterval(currentSpeed + (finalSpeed - initialSpeed) * dt / slowDuration);
        }
        else {
            director->setAnimationInterval(finalSpeed);
            director->getScheduler()->unschedule("slowDown", this);
        }
        }, this, 0.0f, false, "slowDown");
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
            auto pausePanel = ui::Layout::create();
            pausePanel->setContentSize(director->getVisibleSize());
            pausePanel->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
            pausePanel->setBackGroundColor(Color3B(0, 0, 0));
            pausePanel->setBackGroundColorOpacity(150);
            pausePanel->setVisible(true);
            runningScene->addChild(pausePanel, 100, "PausePanel");

            auto continueButton = ui::Button::create("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Play_BTN_Active.png");
            continueButton->setPosition(Vec2(pausePanel->getContentSize().width / 2, pausePanel->getContentSize().height / 2));
            continueButton->addClickEventListener([pausePanel](Ref* sender) {
                Director::getInstance()->resume();
                pausePanel->removeFromParent();
                });
            pausePanel->addChild(continueButton);

            director->pause();
            paused = true;
        }
    }
}

// Add a method to resume the game
void GameController::resumeGame() {
    if (paused) {
        auto director = Director::getInstance();
        auto runningScene = director->getRunningScene();

        if (runningScene) {
            auto pausePanel = runningScene->getChildByName("PausePanel");
            if (pausePanel) {
                pausePanel->removeFromParent();
            }
            director->resume();
            paused = false;
        }
    }
}

// Add a method to check if the game is paused
bool GameController::isPaused() const {
    return paused;
}

// Add a method to replay the game
void GameController::replayGame() {
    auto director = Director::getInstance();
    auto runningScene = director->getRunningScene();

    if (runningScene) {
        std::string currentSceneName = runningScene->getName();

        auto newScene = SceneController::getInstance()->createScene(currentSceneName);
        if (newScene) {
            director->replaceScene(newScene);
        }
    }
}
