// Game3Scene.cpp
#include "Game3/Game3Scene.h"
#include "Game3/Player/PlayerGame3.h"
#include "Game3/enemy/EnemyPlan3.h"

#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "Controller/GameController.h"
#include "Background/Background3.h"
#include "Game2/Cursor/Cursor.h"

USING_NS_CC;

cocos2d::Scene* Game3Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = Game3Scene::create();
    scene->addChild(layer);

    return scene;
}

bool Game3Scene::init() {
    if (!BaseScene::init()) {
        return false;
    }

    // Add sprite background
    background3 = Background3::createBackground("assets_game/gameplay/Game3.2.png");
    if (!background3) {
        return false;
    }
    this->addChild(background3);

    // Add sprite playergame3
    auto _player = PlayerGame3::createPlayerGame3();
    if (!_player) {
        CCLOG("Failed to create PlayerGame3");
        return false;
    }
    this->addChild(_player);



    // Add keyboard event listener
    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = [_player](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _player->onKeyPressed(keyCode, event);
            break;
        default:
            break;
        }
        };

    eventListener->onKeyReleased = [_player](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _player->onKeyReleased(keyCode, event);
            break;
        default:
            break;
        }
        };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    // Add sprite enemy game 3
    auto enemyPlane = EnemyPlan3::createEnemyPlan3();
    if (!enemyPlane) {
        CCLOG("Failed to create EnemyPlane3");
        return false;
    }
    this->addChild(enemyPlane);

    // Add cursor
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/player/tam.png");
    if (!_cursor) {
        CCLOG("Failed to create Cursor");
        return false;
    }
    this->addChild(_cursor);
    this->schedule([this](float delta) {
        _cursor->updateCursor(delta);
        }, "update_cursor_key");

    return true;
}


