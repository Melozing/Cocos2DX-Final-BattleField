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
#include "Game3/Items/ItemPoolGane3.h"

#include "Scene/LoadingScene.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"
#include "Controller/GameController.h"
#include "Manager/BackgroundManager.h"
#include "Game2/Cursor/Cursor.h"
#include "ui/CocosGUI.h"

#include "json/document.h"
#include "json/filereadstream.h"
#include <fstream>

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

    preloadAssets();

    setupBackground();
    setupPlayer();
    initPools();
    setupCursor();
    initSpawning();
    setupContactListener();
    initHealthBar();
    initSound();

    // Create the collision area for the city
    cityCollisionArea = CityCollisionArea::createCityCollisionArea();
    this->addChild(cityCollisionArea);

    // Register event listeners
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addCustomEventListener("SHOW_BOSS_HEALTH_BAR", [this](EventCustom* event) {
        this->showBossHealthBar();
        });
    eventDispatcher->addCustomEventListener("HIDE_BOSS_HEALTH_BAR", [this](EventCustom* event) {
        this->hideBossHealthBar();
        });
    eventDispatcher->addCustomEventListener("UPDATE_BOSS_HEALTH_BAR", [this](EventCustom* event) {
        this->updateBossHealthBar(( enemyBoos->getHealth() / Constants::HealthEnemyPlaneBoss) * 100);
        });

    return true;
}

void Game3Scene::preloadAssets() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/BulletPlayer3Game.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/enemy_plane_boom.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/EnemyPlaneBoss.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/EnemyPlaneBullet.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/tank.plist");
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

void Game3Scene::initBossHealthBar() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    bossHealthBar = CustomLoadingBar::create("assets_game/UXUI/Loading/BossbarGame3.png", "assets_game/UXUI/Loading/BossbarGame3_Border.png", 0.25f);
    bossHealthBar->setLoadingBarRotation(-90);
    bossHealthBar->setLoadingBarPosition(Vec2(visibleSize.width - bossHealthBar->getLoadingBar()->getContentSize().height / 2, visibleSize.height / 2));

    // Adjust the border position to be lower than the loading bar
    auto loadingPos = bossHealthBar->getLoadingBar()->getPosition();
    float loadingBarHeight = SpriteController::calculateScreenRatio(0.0035f);
    float loadingBarWeight = SpriteController::calculateScreenRatio(0.0011f);
    loadingPos.y -= loadingBarHeight; // Move the border lower
    loadingPos.x -= loadingBarWeight; // Move the border lower
    bossHealthBar->setBorderPosition(loadingPos);

    bossHealthBar->setBorderRotation(-90);
    bossHealthBar->setPercent(0);
    bossHealthBar->setLoadingBarScale(SpriteController::updateSpriteScale(bossHealthBar->getLoadingBar(), 0.133f));
    bossHealthBar->setBorderScale(SpriteController::updateSpriteScale(bossHealthBar->getBorder(), 0.17f));
    bossHealthBar->setVisible(false);
    this->addChild(bossHealthBar, Constants::ORDER_LAYER_UI);
}

void Game3Scene::initPools() {
    BulletPoolPlayerGame3::getInstance()->initPool(10);
    EnemyPlaneBulletPool::getInstance()->initPool(10); // Initialize pool with 10 bullets
    BoomForEnemyPlanePool::getInstance()->initPool(10); // Initialize pool with 10 booms
    EnemyPlaneBoomPool::getInstance()->initPool(10); // Initialize pool with 10 booms
    EnemyPlaneBossPool::getInstance()->initPool(1); // Initialize pool with 1 boss
    BulletForEnemyPlanePool::getInstance()->initPool(10); // Initialize pool with 10 bullets for enemy plane
    UpgradeBulletItemPool::getInstance()->initPool(10);
    IncreaseBulletCountItemPool::getInstance()->initPool(10);
    HealthRecoveryItemPool::getInstance()->initPool(10);
}

void Game3Scene::initSound() {
    if (!FileUtils::getInstance()->isFileExist(Constants::pathSoundTrackGame3)) {
        CCLOG("Error: Music file does not exist!");
        return;
    }
    
    SoundController::getInstance()->preloadMusic(Constants::pathSoundTrackGame3);
    SoundController::getInstance()->preloadMusic(Constants::pathSoundBossGame3Phase1);
    
    SoundController::getInstance()->stopMusic(Constants::currentSoundTrackPath);
    Constants::currentSoundTrackPath = Constants::pathSoundTrackGame3;
    SoundController::getInstance()->playMusic(Constants::currentSoundTrackPath, false);
}

void Game3Scene::initSpawning() {
    // Read JSON file
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("json/spawn_enemies_game3.json");
    FILE* fp = fopen(filePath.c_str(), "rb");
    if (!fp) {
        CCLOG("Failed to open JSON file");
        return;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document document;
    document.ParseStream(is);
    fclose(fp);

    if (!document.IsObject() || !document.HasMember("spawnEvents") || !document["spawnEvents"].IsArray()) {
        CCLOG("Invalid JSON format");
        return;
    }

    const auto& spawnEvents = document["spawnEvents"];
    for (const auto& event : spawnEvents.GetArray()) {
        std::string enemyType = event["enemyType"].GetString();
        const auto& spawnTimes = event["spawnTimes"].GetArray();
        const auto& skillTimes = event["skillTimes"].GetArray();
        const auto& spawnSkills = event["spawnSkills"].GetArray();

        for (size_t i = 0; i < spawnTimes.Size(); ++i) {
            float spawnTime = spawnTimes[i].GetFloat();
            float skillTime = skillTimes[i].GetFloat();
            bool spawnWithSkill = spawnSkills[i].GetBool();

            this->scheduleOnce([this, enemyType, skillTime, spawnWithSkill](float) {
                if (enemyType == "EnemyPlaneBullet") {
                    auto enemy = EnemyPlaneBulletPool::getInstance()->getEnemy();
                    if (enemy) {
                        this->addChild(enemy);
                        enemy->spawnEnemy(this, skillTime, spawnWithSkill);
                    }
                }
                else if (enemyType == "EnemyPlaneBoom") {
                    auto enemy = EnemyPlaneBoomPool::getInstance()->getEnemy();
                    if (enemy) {
                        this->addChild(enemy);
                        enemy->spawnEnemy(this, skillTime, spawnWithSkill);
                    }
                }
                else if (enemyType == "EnemyPlaneBoss") {
                    enemyBoos = EnemyPlaneBossPool::getInstance()->getEnemy();
                    if (enemyBoos) {
                        enemyBoos->updatePhase();
                        enemyBoos->spawnEnemy();
                        initBossHealthBar();
                        this->addChild(enemyBoos, Constants::ORDER_LAYER_CHARACTER);
                    }
                }
                }, spawnTime, "spawn_enemy_key_" + enemyType + std::to_string(i));
        }
    }
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
        auto player = dynamic_cast<PlayerGame3*>(nodeA);
        auto bulletPlayer = dynamic_cast<BulletPlayerGame3*>(nodeA);
        auto cityCollisionArea = dynamic_cast<CityCollisionArea*>(nodeA);

        auto enemy = dynamic_cast<EnemyPlaneBase*>(nodeB);
        auto boomForEnemyPlane = dynamic_cast<BoomForEnemyPlane*>(nodeB);
        auto bulletForEnemyPlane = dynamic_cast<BulletForEnemyPlane*>(nodeB);
        auto item = dynamic_cast<ItemBaseGame3*>(nodeB); 

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
        else if (item && cityCollisionArea) { 
            item->stopMovement();
            item->returnItemToPoolAfterDelay(7.0f); // Return item to pool after 7 seconds
        }
        else if (player && item) {
            item->stopMovement();
            item->applyPickupEffect();
        }
        else {
            player = dynamic_cast<PlayerGame3*>(nodeB);
            bulletPlayer = dynamic_cast<BulletPlayerGame3*>(nodeB);
            cityCollisionArea = dynamic_cast<CityCollisionArea*>(nodeB);

            enemy = dynamic_cast<EnemyPlaneBase*>(nodeA);
            boomForEnemyPlane = dynamic_cast<BoomForEnemyPlane*>(nodeA);
            bulletForEnemyPlane = dynamic_cast<BulletForEnemyPlane*>(nodeA);
            item = dynamic_cast<ItemBaseGame3*>(nodeA);

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
            else if (item && cityCollisionArea) {
                item->stopMovement();
                item->returnItemToPoolAfterDelay(7.0f); // Return item to pool after 7 seconds
            } 
            else if (player && item) {
                item->stopMovement();
                item->applyPickupEffect();
            }
        }
    }
    return true;
}

void Game3Scene::handleBulletBoomCollision(BulletPlayerGame3* bullet, BoomForEnemyPlane* boom) {
    bullet->returnPool();
    boom->explode();
}

void Game3Scene::handleBulletEnemyCollision(BulletPlayerGame3* bullet, EnemyPlaneBase* enemy) {
    if (auto enemyBullet = dynamic_cast<EnemyPlaneBullet*>(enemy)) {
        enemyBullet->explode();
        enemyBullet->dropRandomItem();
        bullet->returnPool();
    }
    else if (auto enemyBoom = dynamic_cast<EnemyPlaneBoom*>(enemy)) {
        enemyBoom->explode();
        enemyBoom->dropRandomItem();
        bullet->returnPool();
    }
    else if (enemyBoos = dynamic_cast<EnemyPlaneBoss*>(enemy)) {
        this->handleBossDamage(Constants::BulletDamageGame3);
        bullet->hideModelCharac();
        bullet->explode();
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

void Game3Scene::handleBossDamage(float damage) {
    if (enemyBoos) {
        enemyBoos->takeDamage(damage);
        float healthPercent = (enemyBoos->getHealth() / Constants::HealthEnemyPlaneBoss) * 100;
        updateBossHealthBar(healthPercent);
    }
}

void Game3Scene::updateBossHealthBar(float healthPercent) {
    if (bossHealthBar) {
        bossHealthBar->setPercent(healthPercent);
    }
}

void Game3Scene::showBossHealthBar() {
    if (bossHealthBar) {
        bossHealthBar->setVisible(true);
    }
}

void Game3Scene::hideBossHealthBar() {
    if (bossHealthBar) {
        bossHealthBar->setVisible(false);
    }
}