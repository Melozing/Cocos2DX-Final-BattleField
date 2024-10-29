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

void Game3Scene::setupLoadingBar() {
    // Tạo một thanh tải (loading bar) với hình ảnh từ đường dẫn "assets_game/UXUI/Loading/City.png"
    auto loadingBar = cocos2d::ui::LoadingBar::create("assets_game/UXUI/Loading/City.png");
    if (!loadingBar) {
        // Nếu không tạo được thanh tải, ghi log lỗi và thoát khỏi hàm
        CCLOG("Failed to create LoadingBar");
        return;
    }
    // Đặt phần trăm ban đầu của thanh tải là 0
    loadingBar->setPercent(0);
    // Đặt vị trí của thanh tải ở giữa màn hình
    loadingBar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));
    // Đặt tên cho thanh tải là "LoadingBar"
    loadingBar->setName("LoadingBar");
    // Thêm thanh tải vào cảnh với thứ tự lớp (layer) là Constants::ORDER_LAYER_UI + 100
    this->addChild(loadingBar, Constants::ORDER_LAYER_UI + 100);
    CCLOG("LoadingBar added to the scene");

    // Lên lịch hàm cập nhật để cập nhật phần trăm của thanh tải
    this->schedule([loadingBar](float dt) {
        // Lấy phần trăm hiện tại của thanh tải
        float percent = loadingBar->getPercent();
        // Nếu phần trăm hiện tại nhỏ hơn 100
        if (percent < 100) {
            // Tăng phần trăm lên 1
            percent += 1;
            // Cập nhật phần trăm mới cho thanh tải
            loadingBar->setPercent(percent);
        }
        // Lên lịch hàm cập nhật mỗi 0.1 giây với tên "update_loading_bar"
        }, 0.1f, "update_loading_bar");
}
