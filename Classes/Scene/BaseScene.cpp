// BaseScene.cpp
#include "BaseScene.h"
#include "cocos2d.h"
#include "Button/PauseButton.h"
#include "Controller/GameController.h" // Ensure GameController is included

USING_NS_CC;

bool BaseScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // Add keyboard event listener for pause and resume functionality
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            if (!GameController::getInstance()->isGameOver()) {
                GameController::getInstance()->pauseGame();
            }
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
            if (GameController::getInstance()->isPaused()) {
                GameController::getInstance()->resumeGame();
            }
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}