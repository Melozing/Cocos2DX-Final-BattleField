#include "Game3/Game3Scene.h"
#include "Game3/Player/PlayerGame3.h"

#include "Game3/Items/ItemPoolGane3.h"
#include "Manager/ObjectPoolGame3.h"

#include "Scene/LoadingScene.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"
#include "Controller/GameController.h"

#include "Manager/BackgroundManager.h"
#include "Scene/SceneFinishGame.h"
#include "ui/CocosGUI.h"

#include "json/document.h"
#include "json/filereadstream.h"
#include <fstream>

USING_NS_CC;

cocos2d::Scene* Game3Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game3Scene::create();
    scene->addChild(layer);

    return scene;
}


bool Game3Scene::init() {
    if (!BaseScene::init()) return false;

    this->setSceneCreationFunc([]() -> cocos2d::Scene* {
        return Game3Scene::createScene();
        });

    preloadAssets();

    setupBackground();
    setupPlayer();
    initPools();
    setupCursor();
    initSpawning(Constants::JSON_GAME3_PHASE_1_PATH);
    initBossSpawning(Constants::JSON_GAME3_BOSS_PHASE_1_PATH);
    setupContactListener();
    initHealthBar();
    initBossHealthBar();
    initSound();
    initBulletBadge();
    initUltimateSkillBadge();

    // Create the collision area for the city
    cityCollisionArea = CityCollisionArea::createCityCollisionArea();
    this->addChild(cityCollisionArea);

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
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/rocket.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/warning_rocket.plist");
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
    player->setName(Constants::PlayerGame3Name);
    this->addChild(player);
    setupEventListeners(player);
}

void Game3Scene::initBulletBadge() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float padding = SpriteController::calculateScreenRatio(0.05f); // Adjust the padding as needed
    
    auto spriteBadge = Sprite::create("assets_game/UXUI/Panel/Table_03.png");
    bulletBadge = Badge::createBadge("assets_game/UXUI/Panel/Table_03.png", Constants::FONT_GAME, 24);
    bulletBadge->setScale(SpriteController::updateSpriteScale(spriteBadge, 0.07f)); // Adjust the scale as needed
    bulletBadge->updateLabel("Bullets: " + std::to_string(Constants::QuantityBulletPlayerGame3));
    bulletBadge->setBadgePosition(Vec2(visibleSize.width - bulletBadge->getContentSize().width / 2 - padding, bulletBadge->getContentSize().height / 2 + padding));
    this->addChild(bulletBadge);

    // Register to listen for bullet count change notifications
    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::updateBulletLabel),
        "BulletCountChanged",
        nullptr
    );

    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::blinkRedBadge),
        "BlinkRedBadge",
        nullptr
    );
}

void Game3Scene::initUltimateSkillBadge() {
    ultimateSkillBadge = Badge::createBadge("assets_game/UXUI/Panel/Table_03.png", Constants::FONT_GAME, 24);
    auto spriteBadge = Sprite::create("assets_game/UXUI/Panel/Table_03.png");
    ultimateSkillBadge = Badge::createBadge("assets_game/UXUI/Panel/Table_03.png", Constants::FONT_GAME, 24);
    ultimateSkillBadge->setBadgePosition(Vec2(50, Director::getInstance()->getVisibleSize().height - 50));
    ultimateSkillBadge->setScale(SpriteController::updateSpriteScale(spriteBadge, 0.07f)); // Adjust the scale as needed
    std::string timeText = std::to_string(static_cast<int>(ultimateSkillTimeRemaining));
    ultimateSkillBadge->updateLabel(timeText);
    this->addChild(ultimateSkillBadge);
    ultimateSkillBadge->setVisible(false);
    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::hideUltimateSkillBadge),
        "HIDE_ULTIMATE_SKILL_BADGE",
        nullptr
    );

    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::showUltimateSkillBadge),
        "SHOW_ULTIMATE_SKILL_BADGE",
        nullptr
    );
    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::handleBossUltimateSkill),
        "HANDLE_ULTIMATE_SKILL_BADGE",
        nullptr
    );
}

void Game3Scene::updateUltimateSkillCountdown(float dt) {
    ultimateSkillTimeRemaining -= dt;
    if (ultimateSkillTimeRemaining < 0) {
        this->unschedule("UltimateSkillCountdown");
    }
    else {
        std::string timeText = std::to_string(static_cast<int>(ultimateSkillTimeRemaining));
        ultimateSkillBadge->updateLabel(timeText);
    }
}

void Game3Scene::handleBossUltimateSkill(Ref* sender) {
    ultimateSkillTimeRemaining = Constants::TimeToUltimate;
    this->schedule([this](float dt) {
        this->updateUltimateSkillCountdown(1);
        }, 1.0f, "UltimateSkillCountdown");
}

void Game3Scene::updateBulletLabel(Ref* sender) {
    std::string bulletText = "Bullets: " + std::to_string(Constants::QuantityBulletPlayerGame3);
    bulletBadge->updateLabel(bulletText);
}

void Game3Scene::blinkRedBadge(Ref* sender) {
    bulletBadge->blinkRed();
}

Game3Scene::~Game3Scene() {
    // Remove observer when the scene is destroyed
    __NotificationCenter::getInstance()->removeObserver(this, "HANDLE_ULTIMATE_SKILL_BADGE");
    __NotificationCenter::getInstance()->removeObserver(this, "SHOW_ULTIMATE_SKILL_BADGE");
    __NotificationCenter::getInstance()->removeObserver(this, "HIDE_ULTIMATE_SKILL_BADGE");
    __NotificationCenter::getInstance()->removeObserver(this, "BulletCountChanged");
    __NotificationCenter::getInstance()->removeObserver(this, "SHOW_BOSS_HEALTH_BAR");
    __NotificationCenter::getInstance()->removeObserver(this, "HIDE_BOSS_HEALTH_BAR");
    __NotificationCenter::getInstance()->removeObserver(this, "UPDATE_BOSS_HEALTH_BAR");
    __NotificationCenter::getInstance()->removeObserver(this, "BlinkRedBadge");
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

    // Register observers
    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::showBossHealthBar),
        "SHOW_BOSS_HEALTH_BAR",
        nullptr
    );
    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::hideBossHealthBar),
        "HIDE_BOSS_HEALTH_BAR",
        nullptr
    );
    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::updateBossHealthBar),
        "UPDATE_BOSS_HEALTH_BAR",
        nullptr
    );
}

void Game3Scene::initPools() {
    BulletPoolPlayerGame3::getInstance()->initPool(50);
    EnemyPlaneBulletPool::getInstance()->initPool(10); // Initialize pool with 10 bullets
    BoomForEnemyPlanePool::getInstance()->initPool(10); // Initialize pool with 10 booms
    EnemyPlaneBoomPool::getInstance()->initPool(10); // Initialize pool with 10 booms
    EnemyPlaneBossPool::getInstance()->initPool(1); // Initialize pool with 1 boss
    BulletForEnemyPlanePool::getInstance()->initPool(10); // Initialize pool with 10 bullets for enemy plane
    UpgradeBulletItemPool::getInstance()->initPool(10);
    IncreaseBulletCountItemPool::getInstance()->initPool(10);
    HealthRecoveryItemPool::getInstance()->initPool(10);
    MissileForEnemyPlanePool::getInstance()->initPool(10); // Initialize pool with 10 missiles
    FinisherMissilesPool::getInstance()->initPool(10); // Initialize pool with 10 missiles
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

void Game3Scene::initSpawning(const std::string& jsonFilePath) {
    // Read JSON file
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(jsonFilePath);
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

    const auto& spawnEvents = document["spawnEvents"];
    for (const auto& event : spawnEvents.GetArray()) {
        std::string enemyType = event["enemyType"].GetString();
        float spawnTime = event["spawnTime"].GetFloat();
        float skillTime = event["skillTime"].GetFloat();
        bool spawnSkill = event["spawnSkill"].GetBool();
        std::string direction = event["direction"].GetString();
        std::string position = event["position"].GetString();

        this->scheduleOnce([=, &event](float) {
            if (enemyType == "EnemyPlaneBullet") {
                EnemyPlaneBullet::spawnEnemy(this, skillTime, spawnSkill, direction, position);
                auto enemy = EnemyPlaneBulletPool::getInstance()->getObject();
                if (enemy) {
                    this->addChild(enemy);
                    enemy->spawnEnemy(this, skillTime, spawnSkill, direction, position);
                }
            }
            else if (enemyType == "EnemyPlaneBoom") {
                auto enemy = EnemyPlaneBoomPool::getInstance()->getObject();
                if (enemy) {
                    this->addChild(enemy);
                    enemy->spawnEnemy(this, skillTime, spawnSkill, direction, position);
                }
            }
            }, spawnTime, "spawn_enemy_key_" + std::to_string(spawnTime));
    }
}

void Game3Scene::initBossSpawning(const std::string& jsonFilePath) {
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(jsonFilePath);
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

    if (!document.IsObject() || !document.HasMember("bossSpawnEvents") || !document["bossSpawnEvents"].IsArray()) {
        CCLOG("Invalid JSON format for boss spawn events");
        return;
    }

    const auto& bossSpawnEvents = document["bossSpawnEvents"];
    for (const auto& event : bossSpawnEvents.GetArray()) {
        if (!event.IsObject()) {
            CCLOG("Invalid boss spawn event format");
            continue;
        }

        std::string enemyType = event["enemyType"].GetString();
        float spawnTime = event["spawnTime"].GetFloat();
        float timeToUltimate = event["timeToUltimate"].GetFloat();

        // Lên lịch sinh ra boss
        this->scheduleOnce([=](float) {
            if (enemyType == "EnemyPlaneBoss") {
                enemyBoss = EnemyPlaneBossPool::getInstance()->getObject();
                if (enemyBoss) {
                    enemyBoss->updatePhase();
                    enemyBoss->spawnEnemy(timeToUltimate);
                    initBossHealthBar();
                    if (enemyBoss->getParent() != nullptr) {
                        enemyBoss->removeFromParent();
                    }
                    this->addChild(enemyBoss, Constants::ORDER_LAYER_CHARACTER);
                }
            }
            }, spawnTime, "spawn_boss_key_" + std::to_string(spawnTime));
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
        auto missileEnemy = dynamic_cast<MissileForEnemyPlane*>(nodeB);
        auto item = dynamic_cast<ItemBaseGame3*>(nodeB); 
        auto itemUpgradeBullet = dynamic_cast<UpgradeBulletItemGame3*>(nodeB); 
        auto IncreaseBullet = dynamic_cast<IncreaseBulletCountItemGame3*>(nodeB);
        auto HealthRecovery = dynamic_cast<HealthRecoveryItemGame3*>(nodeB);
        auto FinisherMissilesBoss = dynamic_cast<FinisherMissiles*>(nodeB);

        if (bulletPlayer && enemy) {
            handleBulletEnemyCollision(bulletPlayer, enemy);
        }
        else if (bulletPlayer && missileEnemy) {
            handleBulletMissileCollision(bulletPlayer, missileEnemy);
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
        else if (missileEnemy && cityCollisionArea) {
            handleMissileEnemyCityCollision(missileEnemy);
        }
        else if (FinisherMissilesBoss && cityCollisionArea) {
            handleFinisherMissilesCityCollision(FinisherMissilesBoss);
        }
        else if (item && cityCollisionArea) { 
            item->stopMovement();
            item->returnItemToPoolAfterDelay(7.0f); // Return item to pool after 7 seconds
        }
        else if (player && IncreaseBullet) {
            IncreaseBullet->stopMovement();
            IncreaseBullet->applyPickupEffect();
            Constants::QuantityBulletPlayerGame3 += 50;
        }
        else if (player && itemUpgradeBullet) {
            itemUpgradeBullet->stopMovement();
            itemUpgradeBullet->applyPickupEffect();
            player->increaseBulletCount();
        }
        else if (player && HealthRecovery) {
            HealthRecovery->stopMovement();
            HealthRecovery->applyPickupEffect();
            if (healthBar->getPercent() <= 0) return true;
            // Assuming you have a method to get the current health
            float currentHealth = healthBar->getPercent();
            float newHealth = currentHealth + 10; // Decrease health by 10 (example value)
            updateHealthBar(newHealth);
        }
        else {
            player = dynamic_cast<PlayerGame3*>(nodeB);
            bulletPlayer = dynamic_cast<BulletPlayerGame3*>(nodeB);
            cityCollisionArea = dynamic_cast<CityCollisionArea*>(nodeB);

            enemy = dynamic_cast<EnemyPlaneBase*>(nodeA);
            boomForEnemyPlane = dynamic_cast<BoomForEnemyPlane*>(nodeA);
            bulletForEnemyPlane = dynamic_cast<BulletForEnemyPlane*>(nodeA);
            missileEnemy = dynamic_cast<MissileForEnemyPlane*>(nodeA);
            item = dynamic_cast<ItemBaseGame3*>(nodeA);
            itemUpgradeBullet = dynamic_cast<UpgradeBulletItemGame3*>(nodeA);
            IncreaseBullet = dynamic_cast<IncreaseBulletCountItemGame3*>(nodeA);
            HealthRecovery = dynamic_cast<HealthRecoveryItemGame3*>(nodeA);
            FinisherMissilesBoss = dynamic_cast<FinisherMissiles*>(nodeA);

            if (bulletPlayer && enemy) {
                handleBulletEnemyCollision(bulletPlayer, enemy);
            }
            else if (bulletPlayer && boomForEnemyPlane) {
                handleBulletBoomCollision(bulletPlayer, boomForEnemyPlane);
            }
            else if (bulletPlayer && missileEnemy) {
                handleBulletMissileCollision(bulletPlayer, missileEnemy);
            }
            else if (boomForEnemyPlane && cityCollisionArea) {
                handleBoomCityCollision(boomForEnemyPlane);
            }
            else if (bulletForEnemyPlane && cityCollisionArea) {
                handleBulletForEnemyCityCollision(bulletForEnemyPlane);
            }
            else if (missileEnemy && cityCollisionArea) {
                handleMissileEnemyCityCollision(missileEnemy);
            }
            else if (FinisherMissilesBoss && cityCollisionArea) {
                handleFinisherMissilesCityCollision(FinisherMissilesBoss);
            }
            else if (item && cityCollisionArea) {
                item->stopMovement();
                item->returnItemToPoolAfterDelay(7.0f); // Return item to pool after 7 seconds
            }
            else if (player && itemUpgradeBullet) {
                itemUpgradeBullet->applyPickupEffect();
                player->increaseBulletCount();
            }
            else if (player && IncreaseBullet) {
                IncreaseBullet->applyPickupEffect();
                Constants::QuantityBulletPlayerGame3 += 50;
                updateBulletLabel(nullptr);
            }
            else if (player && HealthRecovery) {
                HealthRecovery->applyPickupEffect();
                if (healthBar->getPercent() <= 0) return true;
                // Assuming you have a method to get the current health
                float currentHealth = healthBar->getPercent();
                float newHealth = currentHealth + 10; // Decrease health by 10 (example value)
                updateHealthBar(newHealth);
            }
        }
    }
    return true;
}

void Game3Scene::handleBulletBoomCollision(BulletPlayerGame3* bullet, BoomForEnemyPlane* boom) {
    bullet->returnPool();
    boom->explode();
}

void Game3Scene::handleBulletMissileCollision(BulletPlayerGame3* bullet, MissileForEnemyPlane* missileEnemy) {
    bullet->returnPool();
    missileEnemy->explode();
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
    else if (enemyBoss = dynamic_cast<EnemyPlaneBoss*>(enemy)) {
        this->handleBossDamage(Constants::BulletDamageGame3);
        bullet->hideModelCharac();
        bullet->explode();
    }
}

void Game3Scene::handleBulletForEnemyCityCollision(BulletForEnemyPlane* bulletForEnemy) {
    bulletForEnemy->explode();
    return;
    if (healthBar->getPercent() <= 0) return;
    // Assuming you have a method to get the current health
    float currentHealth = healthBar->getPercent();
    float newHealth = currentHealth - Constants::DamageBulletForCity; // Decrease health by 10 (example value)
    updateHealthBar(newHealth);
}

void Game3Scene::handleMissileEnemyCityCollision(MissileForEnemyPlane* missileEnemy) {
    missileEnemy->explode();

    if (healthBar->getPercent() <= 0) return;
    // Assuming you have a method to get the current health
    float currentHealth = healthBar->getPercent();
    float newHealth = currentHealth - Constants::DamageBoomForCity; // Decrease health by 10 (example value)
    updateHealthBar(newHealth);
}

void Game3Scene::handleBoomCityCollision(BoomForEnemyPlane* boom) {
    boom->explode();

    if (healthBar->getPercent() <= 0) return;
    float currentHealth = healthBar->getPercent();
    float newHealth = currentHealth - Constants::DamageBoomForCity; // Decrease health by 10 (example value)
    updateHealthBar(newHealth);
}

void Game3Scene::checkHealthBar() {
    if (healthBar->getPercent() <= 0) {
        this->stopAllActions();
        GameController::getInstance()->GameOver(
            []() { Director::getInstance()->end(); }, // Exit action
            []() -> Scene* { return Game3Scene::createScene(); }, // Create scene function
            Constants::pathSoundBossGame3Phase1 // Soundtrack path
        );
    }
}

void Game3Scene::updateHealthBar(float health) {
    healthBar->setPercent(health);
    checkHealthBar();
}

void Game3Scene::handleBossDamage(float damage) {
    if (enemyBoss) {
        enemyBoss->takeDamage(damage);
        if (enemyBoss->getHealth() < 1) {
            if (enemyBoss->getCurrentPhase() == Phase::PHASE_1) {
                // Transition to Phase 2
                initSpawning("path/to/phase2.json");
                initBossSpawning("path/to/phase2.json");
                enemyBoss->updatePhase();
            }
            else if (enemyBoss->getCurrentPhase() == Phase::PHASE_2) {
                // Finish the game and transition to SceneFinishGame
                this->scheduleOnce([](float) {
                    auto scene = SceneFinishGame::createScene();
                    Director::getInstance()->replaceScene(TransitionFade::create(2.0f, scene));
                    }, 2.0f, "finish_game_key");
            }
        }
        this->updateBossHealthBar(nullptr);
    }
}

void Game3Scene::updateBossHealthBar(Ref* sender) {
    if (bossHealthBar) {
        this->bossHealthBar->setPercent((enemyBoss->getHealth() / Constants::HealthEnemyPlaneBoss) * 100);
    }
}

void Game3Scene::showBossHealthBar(Ref* sender) {
    if (bossHealthBar) {
        bossHealthBar->setVisible(true);
    }
}

void Game3Scene::hideBossHealthBar(Ref* sender) {
    if (bossHealthBar) {
        bossHealthBar->setVisible(false);
    }
}

CityCollisionArea* Game3Scene::getCityCollisionArea() const {
    return cityCollisionArea;
}

void Game3Scene::showUltimateSkillBadge(Ref* sender) {
    if (ultimateSkillBadge) {
        ultimateSkillBadge->setVisible(true);
    }
}

void Game3Scene::hideUltimateSkillBadge(Ref* sender) {
    if (ultimateSkillBadge) {
        ultimateSkillBadge->setVisible(false);
    }
}

void Game3Scene::handleFinisherMissilesCityCollision(FinisherMissiles*  FinisherMissilesBoss) {
    // Set health of player and health bar to 0
    FinisherMissilesBoss->explode();
    if (!finisherMissilesHandled) {
        this->healthBar->setPercent(0);

        finisherMissilesHandled = true;
        // Trigger explosion effect in CityCollisionArea
        cityCollisionArea->startExplosions();

        // Schedule GameOver event after a delay
        this->runAction(Sequence::create(
            DelayTime::create(5.0f), // Adjust delay as needed
            CallFunc::create([this]() {
                GameController::getInstance()->GameOver(
                    [this]() {
                        Director::getInstance()->end();
                    },
                    []() -> Scene* {
                        return Game3Scene::createScene();
                    },
                    Constants::pathSoundBossGame3Phase1 // Add the soundtrack path here
                );
                }),
            nullptr
        ));
    }
}