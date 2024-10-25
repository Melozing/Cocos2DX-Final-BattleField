// Game2Scene.cpp
#include "Game2/Game2Scene.h"
#include "Manager/PhysicsManager.h"
#include "Constants/Constants.h"


#include "Game2/Enemy/Enemyh/MeleeEnemy.h"
#include "Game2/Enemy/Enemyh/SniperEnemy.h"
#include "Game2/Enemy/Enemyh/InvEnemy.h"
#include "Game2/Enemy/Enemyh/SuicideBomberEnemy.h"
#include "Game2/Enemy/Enemyh/BossEnemy.h"

USING_NS_CC;

const uint32_t PLAYER_BITMASK = 0x0001;
const uint32_t TILE_BITMASK = 0x0002;

cocos2d::Scene* Game2Scene::createScene() {
    return Game2Scene::create();
}

bool Game2Scene::init() {
    if (!BaseScene::init()) {
        CCLOG("Failed to initialize Scene");
        return false;
    }

    this->setSceneCreationFunc([]() -> cocos2d::Scene* {
        return Game2Scene::createScene();
        });

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Load the background image
    auto background = Sprite::create("assets_game/gameplay/game2/game2.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
    this->addChild(background, -1); // Add background at a lower z-order

    // Create the player at the center of the screen
    _player = PlayerGame2::createPlayerGame2();
    if (!_player) {
        CCLOG("Failed to create PlayerGame2");
        return false;
    }
    _player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    _player->setName("PlayerGame2");
    this->addChild(_player);

    // Setup keyboard event listeners
    setupKeyboardEventListeners();

    // Setup cursor
    setupCursor();

    // Spawn enemies at specific points
    this->schedule([this](float delta) {
        spawnEnemies();
        }, 5.0f, "spawn_enemy_key");

    CCLOG("Game2Scene initialized successfully");

    this->scheduleUpdate();

    return true;
}

void Game2Scene::setupKeyboardEventListeners() {
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (_player) {
            _player->onKeyPressed(keyCode, event);
        }
        };

    eventListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (_player) {
            _player->onKeyReleased(keyCode, event);
        }
        };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void Game2Scene::setupCursor() {
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    _cursor = Cursor::create("assets_game/player/tam.png");
    if (!_cursor) {
        CCLOG("Failed to create Cursor");
        return;
    }
    this->addChild(_cursor);
}

void Game2Scene::update(float delta) {
    if (_player) {
        _player->update(delta);
        // Ensure the player stays within the screen bounds
        const auto visibleSize = Director::getInstance()->getVisibleSize();
        const Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 pos = _player->getPosition();

        pos.x = std::max(origin.x, std::min(pos.x, origin.x + visibleSize.width));
        pos.y = std::max(origin.y, std::min(pos.y, origin.y + visibleSize.height));

        _player->setPosition(pos);
    }
}

void Game2Scene::spawnEnemies() {
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Define spawn points in the original resolution (1920x1080)
    Vec2 spawnPoints[] = {
        Vec2(585, 330),
        Vec2(1520, 460),
        Vec2(1020, 900)
    };

    // Adjust spawn points based on the current screen size
    for (const auto& point : spawnPoints) {
        float x = point.x * (visibleSize.width / 1920.0f);
        float y = point.y * (visibleSize.height / 1080.0f);

        auto enemy = MeleeEnemy::create();
        if (enemy) {
            enemy->setPosition(Vec2(x + origin.x, y + origin.y));
            this->addChild(enemy);
            enemy->scheduleUpdate();
        }
    }
}
