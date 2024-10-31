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
#include "ui/CocosGUI.h"

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
    setupLoadingBar();

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
    _cursor->setInitialPosition();
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

void Game3Scene::setupLoadingBar() {
    // Create loading bars
    auto loadingBar = cocos2d::ui::LoadingBar::create("assets_game/UXUI/Loading/City.png");
    auto loadingBar_black = cocos2d::Sprite::create("assets_game/UXUI/Loading/City_Black.png");
    if (!loadingBar || !loadingBar_black) {
        CCLOG("Failed to create LoadingBars");
        return;
    }

    // Set initial properties
    loadingBar->setPercent(0);
    loadingBar_black->setScale(0.5f);
    loadingBar->setScale(0.5f);

    // Position loading bars
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto loadingBarSize = loadingBar->getContentSize() * 0.5f;
    Vec2 position(visibleSize.width - loadingBarSize.width / 2, visibleSize.height - loadingBarSize.height / 2);
    loadingBar->setPosition(position);
    loadingBar_black->setPosition(position);

    // Add loading bars to the scene
    this->addChild(loadingBar_black, Constants::ORDER_LAYER_UI);
    this->addChild(loadingBar, Constants::ORDER_LAYER_UI + 100);

    // Schedule loading bar updates
    float duration = 60.0f; // 1 minute
    float increment = 100.0f / (duration * 60.0f); // Increment per frame (assuming 60 FPS)

    this->schedule([loadingBar, increment](float dt) {
        float percent = loadingBar->getPercent();
        if (percent < 100) {
            percent += increment; // Increase loading percentage
            loadingBar->setPercent(percent);
        }
        else {
            loadingBar->setPercent(100);
            // Transition to the next scene or perform any other action
        }
        }, 1.0f / 60.0f, "loading_bar_update_key");
}






