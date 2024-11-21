#include "Game3/Game3Scene.h"
#include "Game3/Player/PlayerGame3.h"
#include "Game3/enemy/EnemyPlaneBullet.h"
#include "Game3/enemy/EnemyPlaneBoom.h"
#include "Game3/enemy/EnemyPlaneBulletPool.h"
#include "Game3/enemy/BulletForEnemyPlanePool.h"
#include "Game3/enemy/EnemyPlaneBoomPool.h"
#include "Game3/enemy/EnemyPlaneBossPool.h"
#include "Game3/enemy/EnemyPlaneBoss.h"
#include "Game3/enemy/BoomForEnemyPlanePool.h"
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
    initPools();
    setupCursor();
    initSpawning();
    setupContactListener();
    scheduleBossSpawn(); // Schedule boss spawn after 30 seconds
    initHealthBar();

    // Create the collision area for the city
    cityCollisionArea = CityCollisionArea::createCityCollisionArea();
    this->addChild(cityCollisionArea);

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

void Game3Scene::initHealthBar() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    healthBar = CustomLoadingBar::create("assets_game/UXUI/Loading/health_bar_g3_progress.png", "assets_game/UXUI/Loading/health_bar_g3_border.png", 0.25f);
    healthBar->setLoadingBarRotation(-90);
    healthBar->setLoadingBarPosition(Vec2(healthBar->getLoadingBar()->getContentSize().height + SpriteController::calculateScreenRatio(0.03f) / 2, visibleSize.height / 2));

    // Adjust the border position to be lower than the loading bar
    auto loadingPos = healthBar->getLoadingBar()->getPosition();
    float loadingBarHeight = SpriteController::calculateScreenRatio(0.01f);
    loadingPos.y -= loadingBarHeight; // Move the border lower
    healthBar->setBorderPosition(loadingPos);

    healthBar->setBorderRotation(-90);
    healthBar->setPercent(100);
    healthBar->setLoadingBarScale(SpriteController::updateSpriteScale(healthBar->getLoadingBar(), 0.133f));
    healthBar->setBorderScale(SpriteController::updateSpriteScale(healthBar->getBorder(), 0.155f));

    this->addChild(healthBar, Constants::ORDER_LAYER_UI);
}


void Game3Scene::initPools() {
    BulletPool::getInstance()->initPool(10);
    EnemyPlaneBulletPool::getInstance()->initPool(10); // Initialize pool with 10 bullets
    BoomForEnemyPlanePool::getInstance()->initPool(10); // Initialize pool with 10 booms
    EnemyPlaneBoomPool::getInstance()->initPool(10); // Initialize pool with 10 booms
    EnemyPlaneBossPool::getInstance()->initPool(1); // Initialize pool with 1 boss
    BulletForEnemyPlanePool::getInstance()->initPool(10); // Initialize pool with 10 bullets for enemy plane
}


void Game3Scene::initSpawning() {
    this->schedule([this](float) {
        EnemyPlaneBullet::spawnEnemy(this);
        }, 3.0f, "spawn_bullet_key");

    this->schedule([this](float) {
        EnemyPlaneBoom::spawnEnemy(this);

        }, 3.0f, "spawn_boom_key");
    this->schedule([this](float) {

        EnemyPlaneBoss::spawnEnemy(this);
        }, 30.0f, "spawn_boss_key");
}

void Game3Scene::scheduleBossSpawn() {
    this->scheduleOnce([this](float) {
        EnemyPlaneBoss::spawnEnemy(this);
        }, 30.0f, "spawn_boss_key");
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
        auto bulletPlayer = dynamic_cast<Bullet*>(nodeA);
        auto cityCollisionArea = dynamic_cast<CityCollisionArea*>(nodeA);

        auto enemy = dynamic_cast<EnemyPlaneBase*>(nodeB);
        auto boomForEnemyPlane = dynamic_cast<BoomForEnemyPlane*>(nodeB);
        auto bulletForEnemyPlane = dynamic_cast<BulletForEnemyPlane*>(nodeB);

        if (bulletPlayer && enemy) {
            handleBulletEnemyCollision(bulletPlayer, enemy);
        }
        else if (bulletPlayer && boomForEnemyPlane) {
            handleBulletBoomCollision(bulletPlayer, boomForEnemyPlane);
        }
        else if (boomForEnemyPlane && cityCollisionArea) {
            handleBoomCityCollision(boomForEnemyPlane);
        }
        else if (bulletForEnemyPlane && cityCollisionArea) {
            handleBulletForEnemyCityCollision(bulletForEnemyPlane);
        }
        else {
            bulletPlayer = dynamic_cast<Bullet*>(nodeB);
            cityCollisionArea = dynamic_cast<CityCollisionArea*>(nodeB);

            enemy = dynamic_cast<EnemyPlaneBase*>(nodeA);
            boomForEnemyPlane = dynamic_cast<BoomForEnemyPlane*>(nodeA);
            bulletForEnemyPlane = dynamic_cast<BulletForEnemyPlane*>(nodeA);

            if (bulletPlayer && enemy) {
                handleBulletEnemyCollision(bulletPlayer, enemy);
            }
            else if (bulletPlayer && boomForEnemyPlane) {
                handleBulletBoomCollision(bulletPlayer, boomForEnemyPlane);
            }
            else if (boomForEnemyPlane && cityCollisionArea) {
                handleBoomCityCollision(boomForEnemyPlane);
            }
            else if (bulletForEnemyPlane && cityCollisionArea) {
                handleBulletForEnemyCityCollision(bulletForEnemyPlane);
            }
        }
    }
    return true;
}

void Game3Scene::handleBulletBoomCollision(Bullet* bullet, BoomForEnemyPlane* boom) {
    bullet->removeFromParent();
    boom->explode();
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

void Game3Scene::handleBulletForEnemyCityCollision(BulletForEnemyPlane* bulletForEnemy) {
    bulletForEnemy->explode();

    if (healthBar->getPercent() <= 0) return;
    // Assuming you have a method to get the current health
    float currentHealth = healthBar->getPercent();
    float newHealth = currentHealth - 5; // Decrease health by 10 (example value)
    updateHealthBar(newHealth);
}


void Game3Scene::handleBoomCityCollision(BoomForEnemyPlane* boom) {
    boom->explode();

    if (healthBar->getPercent() <= 0) return;
    float currentHealth = healthBar->getPercent();
    float newHealth = currentHealth - 10; // Decrease health by 10 (example value)
    updateHealthBar(newHealth);
}

void Game3Scene::checkHealthBar() {
    if (healthBar->getPercent() <= 0) {
        this->stopAllActions();
        GameController::getInstance()->GameOver(
            []() { Director::getInstance()->end(); }, // Exit action
            []() -> Scene* { return Game3Scene::createScene(); }, // Create scene function
            Constants::pathSoundTrackGame1 // Soundtrack path
        );
    }
}

void Game3Scene::updateHealthBar(float health) {
    healthBar->setPercent(health);
    checkHealthBar();
}


