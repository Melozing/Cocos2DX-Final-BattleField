﻿#include "Game3/Game3Scene.h"
#include "Game3/Player/PlayerGame3.h"
#include "Game3/enemy/EnemyPlaneBullet.h"
#include "Game3/enemy/EnemyPlaneBoom.h"
#include "Game3/enemy/EnemyPlaneBulletPool.h"
#include "Game3/enemy/EnemyPlaneBoomPool.h"
#include "Game3/enemy/EnemyPlaneBossPool.h"
#include "Game3/enemy/EnemyPlaneBoss.h"
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
    //scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);

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
    initPools();
    setupCursor();
    initSpawning();
	setupContactListener();
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

void Game3Scene::initPools() {
    BulletPool::getInstance()->initPool(10);
    EnemyPlaneBulletPool::getInstance()->initPool(10); // Initialize pool with 10 bullets
    EnemyPlaneBoomPool::getInstance()->initPool(10); // Initialize pool with 10 booms
	EnemyPlaneBossPool::getInstance()->initPool(1); // Initialize pool with 1 boss
}

void Game3Scene::initSpawning() {
    this->schedule([this](float) {
        EnemyPlaneBullet::spawnEnemy(this);
        }, 1.0f, "spawn_bullet_key");

    this->schedule([this](float) {
        EnemyPlaneBoom::spawnEnemy(this);
        }, 1.0f, "spawn_boom_key");
    this->schedule([this](float) {
        EnemyPlaneBoss::spawnEnemy(this);
        }, 1.0f, "spawn_Boss_key");
}

void Game3Scene::setupCursor() {
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
        player->onKeyPressed(keyCode, event);
        };
    eventListener->onKeyReleased = [player](EventKeyboard::KeyCode keyCode, Event* event) {
        player->onKeyReleased(keyCode, event);
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, player);
}

void Game3Scene::setupContactListener() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game3Scene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool Game3Scene::onContactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB) {
        if (dynamic_cast<Bullet*>(nodeA) && dynamic_cast<EnemyPlaneBase*>(nodeB)) {
            handleBulletEnemyCollision(static_cast<Bullet*>(nodeA), static_cast<EnemyPlaneBase*>(nodeB));
        }
        else if (dynamic_cast<Bullet*>(nodeB) && dynamic_cast<EnemyPlaneBase*>(nodeA)) {
            handleBulletEnemyCollision(static_cast<Bullet*>(nodeB), static_cast<EnemyPlaneBase*>(nodeA));
        }
    }
    return true;
}

void Game3Scene::handleBulletEnemyCollision(Bullet* bullet, EnemyPlaneBase* enemy) {
    // Return bullet to pool
    BulletPool::getInstance()->returnBullet(bullet);

    // Trigger explosion on enemy
    if (auto enemyBullet = dynamic_cast<EnemyPlaneBullet*>(enemy)) {
        enemyBullet->explode();
    }
    else if (auto enemyBoom = dynamic_cast<EnemyPlaneBoom*>(enemy)) {
        enemyBoom->explode();
    }
}