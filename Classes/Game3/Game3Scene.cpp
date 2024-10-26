#include "Game3/Game3Scene.h"
#include "Game3/Player/PlayerGame3.h"
#include "Game3/enemy/EnemyPlane3.h"
#include "Game3/enemy/EnemyPlane1.h" 
#include "Scene/LoadingScene.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "Controller/GameController.h"
#include "Manager/BackgroundManager.h"
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

    this->setSceneCreationFunc([]() -> cocos2d::Scene* {
        return Game3Scene::createScene();
        });

    // Set the background using BackgroundManager
    BackgroundManager::getInstance()->setBackground(this, "assets_game/gameplay/Game3Background.png", Constants::ORDER_LAYER_BACKGROUND);

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
    auto enemyPlane = EnemyPlane3::createEnemyPlan3();
    if (!enemyPlane) {
        CCLOG("Failed to create EnemyPlane3");
        return false;
    }
    this->addChild(enemyPlane);

    // Add sprite enemy plane 1
    auto enemyPlane1 = EnemyPlane1::createEnemyPlane1();
    if (!enemyPlane1) {
        CCLOG("Failed to create EnemyPlane1");
        return false;
    }
    this->addChild(enemyPlane1);

    //Spawn enemy after delay
    EnemyPlane3::spawnEnemyAfterDelay(3.0f, this);
    EnemyPlane1::spawnEnemyAfterDelay(6.0f, this);
    
    // Add cursor
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/player/tam.png");
    if (!_cursor) {
        CCLOG("Failed to create Cursor");
        return false;
    }
    this->addChild(_cursor, Constants::ORDER_LAYER_UI + 99);
    return true;
}