#include "BaseScene.h"
#include "Controller/GameController.h"
#include "Controller/SceneController.h"
#include "Constants/Constants.h"


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
                _cursor->setVisible(false);
                auto exitAction = []() {
                    Director::getInstance()->end();
                    };
                std::string soundtrackPath = Constants::pathSoundTrackGame1;
                GameController::getInstance()->pauseGame(exitAction, _sceneCreationFunc, soundtrackPath);
            }
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
            if (GameController::getInstance()->isPaused()) {
                _cursor->setVisible(true);
                GameController::getInstance()->resumeGame();
            }
        }
        };

    // Create and add the custom cursor
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/UXUI/Main_Menu/pointer.png");
    _cursor->setAnchorPoint(Vec2(0.5, 0.5));
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f));
    _cursor->setVisible(true);
    if (_cursor) {
        _cursor->setPosition(visibleSize / 2); // Set initial position
        this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR); // Add cursor to the scene with z-order 1
    }

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void BaseScene::setSceneCreationFunc(const std::function<cocos2d::Scene* ()>& func) {
    _sceneCreationFunc = func;
}
