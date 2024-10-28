﻿#include "Game2/Game2Scene.h"
#include "Manager/PhysicsManager.h"
#include "Constants/Constants.h"
#include "Manager/BackgroundManager.h"

#include "Game2/Enemy/Enemyh/MeleeEnemy.h"
#include "Game2/Enemy/Enemyh/SniperEnemy.h"
#include "Game2/Enemy/Enemyh/InvEnemy.h"
#include "Game2/Enemy/Enemyh/SuicideBomberEnemy.h"
#include "Game2/Enemy/Enemyh/BossEnemy.h"

USING_NS_CC;

const uint32_t PLAYER_BITMASK = 0x0001;
const uint32_t TILE_BITMASK = 0x0002;

cocos2d::Scene* Game2Scene::createScene() {
    auto scene = Scene::createWithPhysics(); // Create scene with physics
    scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game2Scene::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

bool Game2Scene::init() {
    if (!BaseScene::init()) {
        CCLOG("Failed to initialize BaseScene");
        return false;
    }

    this->setSceneCreationFunc([]() -> cocos2d::Scene* {
        return Game2Scene::createScene();
        });

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Load the background image
    BackgroundManager::getInstance()->setBackground(this, "assets_game/gameplay/game2/game2.png", Constants::ORDER_LAYER_BACKGROUND);

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

    // Initialize and setup cursor
    //_cursor = Cursor::create("assets_game/UXUI/Main_Menu/pointer.png");
    //if (_cursor) {
    //    _cursor->setAnchorPoint(Vec2(0.5, 0.5));
    //    _cursor->setScale(0.03f); // Adjust scale as needed
    //    _cursor->setPosition(visibleSize / 2); // Set initial position
    //    this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR); // Add cursor to the scene with z-order
    //    setupCursor();
    //}
    //else {
    //    CCLOG("Failed to create cursor");
    //}

    // Spawn enemies at specific points
    this->schedule([this](float delta) {
        spawnEnemies();
        }, 5.0f, "spawn_enemy_key");

    CCLOG("Game2Scene initialized successfully");

    this->scheduleUpdate();

    return true;
}

void Game2Scene::setupCursor() {
    if (_cursor) {
        CCLOG("Changing cursor sprite...");
        _cursor->changeSprite("assets_game/player/tam.png");
    }
    else {
        CCLOG("Cursor is not initialized");
    }
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

        auto enemy = SniperEnemy::create();
        if (enemy) {
            enemy->setPosition(Vec2(x + origin.x, y + origin.y));
            this->addChild(enemy);
            enemy->scheduleUpdate();
        }
    }
}
