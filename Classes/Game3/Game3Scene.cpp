#include "Game3/Game3Scene.h"
#include "Game3/Player/PlayerGame3.h"
#include "Game3/enemy/EnemyPlaneBullet.h"
#include "Game3/enemy/EnemyPlaneBoom.h" 
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

    setupBackground();
    setupPlayer();
    setupEnemies();
    setupCursor();

    return true;
}

void Game3Scene::setupBackground() {
    BackgroundManager::getInstance()->setBackground(this, "assets_game/gameplay/Game3Background.png", Constants::ORDER_LAYER_BACKGROUND);
}

void Game3Scene::setupPlayer() {
    auto player = PlayerGame3::createPlayerGame3();
    if (!player) {
        CCLOG("Failed to create PlayerGame3");
        return;
    }
    this->addChild(player);
    setupEventListeners(player);
}

void Game3Scene::setupEnemies() {
    auto enemyPlaneBullet = EnemyPlaneBullet::createEnemyBullet();
    if (!enemyPlaneBullet) {
        CCLOG("Failed to create EnemyPlaneBullet");
        return;
    }

    auto enemyPlaneBoom = EnemyPlaneBoom::createEnemyPlaneBoom();
    if (!enemyPlaneBoom) {
        CCLOG("Failed to create EnemyPlaneBoom");
        return;
    }
    this->addChild(enemyPlaneBoom);
    this->addChild(enemyPlaneBullet);

    EnemyPlaneBullet::spawnEnemyAfterDelay(3.0f, this);
    EnemyPlaneBoom::spawnEnemyAfterDelay(6.0f, this);
}

void Game3Scene::setupCursor() {
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/player/bullseye_white.png");
    if (!_cursor) {
        CCLOG("Failed to create Cursor");
        return;
    }
    _cursor->setName("Cursor");
    this->addChild(_cursor, Constants::ORDER_LAYER_UI + 99);
}

void Game3Scene::setupEventListeners(PlayerGame3* player) {
    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = [player](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            player->onKeyPressed(keyCode, event);
            break;
        default:
            break;
        }
        };

    eventListener->onKeyReleased = [player](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            player->onKeyReleased(keyCode, event);
            break;
        default:
            break;
        }
        };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}
