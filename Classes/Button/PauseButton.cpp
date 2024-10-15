#include "Button/PauseButton.h"
#include "ui/CocosGUI.h"
#include "Controller/GameController.h"
#include "Controller/SceneController.h"

USING_NS_CC;

bool PauseButton::init() {
    if (!Node::init()) {
        return false;
    }

    // Create the pause button
    auto pauseButton = ui::Button::create("assets_game/UXUI/Buttons/Pause_BTN.png", "assets_game/UXUI/Buttons/Pause_BTN_Active.png");
    pauseButton->setPosition(Vec2(50, 50)); // Set the position of the pause button
    pauseButton->addClickEventListener([this](Ref* sender) {
        if (!GameController::getInstance()->isGameOver()) { // Check if the game is not over
            if (GameController::getInstance()->isPaused()) {
                GameController::getInstance()->resumeGame();
            }
            else {
                auto exitAction = []() {
                    Director::getInstance()->end();
                    };
                auto createSceneFunc = []() -> Scene* {
                    return SceneController::getInstance()->createScene("GameScene");
                    };
                std::string soundtrackPath = "path/to/soundtrack.mp3";
                GameController::getInstance()->pauseGame(exitAction, createSceneFunc, soundtrackPath);
            }
        }
        });
    this->addChild(pauseButton, 1); // Set z-order to 1

    return true;
}

void PauseButton::pauseGame() {
    auto exitAction = []() {
        Director::getInstance()->end();
        };
    auto createSceneFunc = []() -> Scene* {
        return SceneController::getInstance()->createScene("GameScene");
        };
    std::string soundtrackPath = "path/to/soundtrack.mp3";
    GameController::getInstance()->pauseGame(exitAction, createSceneFunc, soundtrackPath);
}

void PauseButton::continueGame() {
    GameController::getInstance()->resumeGame();
}

bool PauseButton::isPaused() const {
    return GameController::getInstance()->isPaused();
}
