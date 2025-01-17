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
#include "Manager/TutorialLayer.h"
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
    initPools();
    initBoss();
    initBulletSpawning(Constants::JSON_GAME3_ENEMYBULLET_PHASE_1_PATH);
    initBoomSpawning(Constants::JSON_GAME3_ENEMYBOOM_PHASE_1_PATH);
    initBossSpawning(Constants::JSON_GAME3_BOSS_PHASE_1_PATH);
    initHealthBar();
    initBulletBadge();
    initBossHealthBar();
    initSound();
    setupPlayer();
    initUltimateSkillBadge();
    showTutorialIfNeeded();
    setupContactListener();
    setupMobile();

    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_WINDOWS ||
        platform == cocos2d::Application::Platform::OS_MAC) {
        setupCursor();
    }

    // Create the collision area for the city
    cityCollisionArea = CityCollisionArea::createCityCollisionArea();
    this->addChild(cityCollisionArea);

    checkAndUpdateBackground();

    return true;
}

void Game3Scene::setupMobile() {
    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        initControlButtons();
        initTouchListener();
    }
}

void Game3Scene::updateHealth(float newHealth) {
    // Clamp the health value
    if (newHealth < 0) newHealth = 0;
    if (newHealth > maxHealthCircle) newHealth = maxHealthCircle;
    if (enemyBoss->getHealth() < 1 && enemyBoss->isVisible()) return;

    // Update current health
    currentHealthCircle = newHealth;

    // Calculate percentage
    float percentage = (currentHealthCircle / maxHealthCircle) * 100;
    healthBarCircle->setPercentage(percentage);
    checkHealthBar();
}

void Game3Scene::showTutorialIfNeeded() {
    //cocos2d::UserDefault::getInstance()->setBoolForKey(Constants::DONT_SHOW_TUTORIAL_GAME3.c_str(), false);
    bool dontShowTutorial = cocos2d::UserDefault::getInstance()->getBoolForKey(Constants::DONT_SHOW_TUTORIAL_GAME3.c_str(), false);
    if (!dontShowTutorial) {
        if (_cursor)
        {
            _cursor->hideCursor();
        }
        auto imageTutorial = "assets_game/UXUI/Tutorial/Game3/image1.jpg";

        auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
        if (platform == cocos2d::Application::Platform::OS_ANDROID ||
            platform == cocos2d::Application::Platform::OS_IPHONE) {
            imageTutorial = "assets_game/UXUI/Tutorial/Game3/image1_mobile.jpg";
        }

        std::vector<std::string> slideImages = {
            imageTutorial,
            "assets_game/UXUI/Tutorial/Game3/image2.jpg",
            "assets_game/UXUI/Tutorial/Game3/image3.jpg",
            "assets_game/UXUI/Tutorial/Game3/image4.jpg"
        };
        auto tutorialLayer = TutorialLayer::create(slideImages, Constants::DONT_SHOW_TUTORIAL_GAME3);
        this->addChild(tutorialLayer, Constants::ORDER_LAYER_UI); // Add tutorial layer on top
    }
    else
    {
        if (_cursor)
        {
            _cursor->showCursor();
        }
    }
}

void Game3Scene::setMaxHealth(float health) {
    if (health > 0) {
        maxHealthCircle = health;
        currentHealthCircle = health; // Reset current health to max
        updateHealth(health); // Update health bar and label
    }
}

void Game3Scene::preloadAssets() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/gameplay/BackgroundNormal.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/gameplay/BackgroundBreak.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/PlayerGoLeft.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/PlayerGoRight.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/PlayerIdle.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/BulletPlayer3Game.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/EnemyPlaneBoom.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/EnemyPlaneBoss.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/EnemyPlaneBullet.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/rocket.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/warning_rocket.plist");
}

void Game3Scene::setupBackground() {
    // Initialize background state flags
    isBackgroundNormal = false;
    isBackgroundBreak = false;

    // Preload backgrounds
    std::vector<std::string> backgrounds = { "BackgroundNormal", "BackgroundBreak" };
    BackgroundManager::getInstance()->preloadBackgrounds(this, backgrounds, 129, 0.033f);
}

void Game3Scene::checkAndUpdateBackground() {
    if (currentHealthCircle >= 50) {
        if (!isBackgroundNormal) {
            BackgroundManager::getInstance()->transitionBackground("BackgroundNormal", 0.15f);
            isBackgroundNormal = true;
            isBackgroundBreak = false;
        }
    }
    else {
        if (!isBackgroundBreak) {
            BackgroundManager::getInstance()->transitionBackground("BackgroundBreak", 0.15f);
            isBackgroundNormal = false;
            isBackgroundBreak = true;
        }
    }
}

void Game3Scene::setupPlayer() {
    playerGame3 = PlayerGame3::createPlayerGame3();
    if (!playerGame3) {
        CCLOG("Failed to create PlayerGame3");
        return;
    }
    playerGame3->setName(Constants::PlayerGame3Name);
    this->addChild(playerGame3);
    setupEventListeners(playerGame3);
    // Add touch event listener

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayerGame3::onTouchBegan, playerGame3);
    touchListener->onTouchMoved = CC_CALLBACK_2(PlayerGame3::onTouchMoved, playerGame3);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, playerGame3);
}

void Game3Scene::initBulletBadge() {
    Constants::QuantityBulletPlayerGame3 = 100;
    auto spriteBadge = Sprite::create("assets_game/UXUI/Panel/Table_03.png");
    bulletBadge = Badge::createBadge("assets_game/UXUI/Panel/Table_03.png", Constants::FONT_GAME, 25);
    bulletBadge->setScale(SpriteController::updateSpriteScale(spriteBadge, 0.09f)); // Adjust the scale as needed
    std::string labelText = std::to_string(Constants::QuantityBulletPlayerGame3);
    bulletBadge->updateLabel(labelText);
    float paddingWidth = SpriteController::calculateScreenRatio(0.1f);
    float paddingHeight = SpriteController::calculateScreenHeightRatio(0.035f);
    float xOffset = -SpriteController::calculateScreenRatio(0.039f);
    bulletBadge->adjustLabelPosition(xOffset, 0);
    bulletBadge->setBadgePosition(Vec2(healthBarCircle->getPosition().x - paddingWidth, healthBarCircle->getPosition().y - paddingHeight));
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

std::string Game3Scene::formatTime(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
    return std::string(buffer);
}


void Game3Scene::initUltimateSkillBadge() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ultimateSkillBadge = Badge::createBadge("assets_game/UXUI/Panel/Ultimate_Boss_Badge.png", Constants::FONT_GAME, 24);
    auto spriteBadge = Sprite::create("assets_game/UXUI/Panel/Ultimate_Boss_Badge.png");
    float paddingWidth = SpriteController::calculateScreenRatio(0.045f);
    ultimateSkillBadge->setScale(SpriteController::updateSpriteScale(spriteBadge, 0.07f)); // Adjust the scale as needed
    ultimateSkillBadge->setBadgePosition(Vec2(visibleSize.width / 2 - bossHealthBar->GetSize().width / 2 - paddingWidth, bulletBadge->getPosition().y));
    float xOffset = SpriteController::calculateScreenRatio(0.025f);
    ultimateSkillBadge->adjustLabelPosition(xOffset, 0);
    std::string timeText = formatTime(static_cast<int>(ultimateSkillTimeRemaining));
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
        std::string timeText = formatTime(static_cast<int>(ultimateSkillTimeRemaining));
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
    std::string bulletText = std::to_string(Constants::QuantityBulletPlayerGame3);
    bulletBadge->updateLabel(bulletText);
}

void Game3Scene::blinkRedBadge(Ref* sender) {
    bulletBadge->blinkRed();
}

Game3Scene::~Game3Scene() {
    // Remove observer when the scene is destroyed
    __NotificationCenter::getInstance()->removeObserver(this, "HideCursorNotification");
    __NotificationCenter::getInstance()->removeObserver(this, "ShowCursorNotification");
    __NotificationCenter::getInstance()->removeObserver(this, "HANDLE_ULTIMATE_SKILL_BADGE");
    __NotificationCenter::getInstance()->removeObserver(this, "HIDE_ULTIMATE_SKILL_BADGE");
    __NotificationCenter::getInstance()->removeObserver(this, "BulletCountChanged");
    __NotificationCenter::getInstance()->removeObserver(this, "SHOW_ULTIMATE_SKILL_BADGE");
    __NotificationCenter::getInstance()->removeObserver(this, "SHOW_BOSS_HEALTH_BAR");
    __NotificationCenter::getInstance()->removeObserver(this, "HIDE_BOSS_HEALTH_BAR");
    __NotificationCenter::getInstance()->removeObserver(this, "UPDATE_BOSS_HEALTH_BAR");
    __NotificationCenter::getInstance()->removeObserver(this, "BlinkRedBadge");
    __NotificationCenter::getInstance()->removeObserver(this, "FINISH_AND_CHANGE_SCENE");
}

void Game3Scene::initHealthBar() {
    auto sprite = Sprite::create("assets_game/UXUI/Loading/Red_circle.png");
    healthBarCircle = ProgressTimer::create(sprite);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    float paddingHeight = SpriteController::calculateScreenRatio(0.04f); // Adjust the padding as needed
    float paddingWeight = SpriteController::calculateScreenRatio(0.05f); // Adjust the padding as needed
    float xPos = origin.x + visibleSize.width - paddingWeight;
    float yPos = origin.y / 2 + paddingHeight;

    // Initialize the ProgressTimer with the sprite
    healthBarCircle->setType(ProgressTimer::Type::RADIAL);
    healthBarCircle->setScale(SpriteController::updateSpriteScale(sprite,0.145f));
    healthBarCircle->setMidpoint(Vec2(0.5f, 0.5f));
    healthBarCircle->setBarChangeRate(Vec2(0, 1));  // Radial change from bottom
    healthBarCircle->setPercentage(100);            // Start at 100% health
    healthBarCircle->setPosition(Vec2(xPos, yPos));   // Default position
    healthBarCircle->setRotation(180);   // Default position

    auto backHealthBar = Sprite::create("assets_game/UXUI/Loading/health_player_game3_back.png");
    auto frontHealthBar = Sprite::create("assets_game/UXUI/Loading/health_player_game3_front.png");
    backHealthBar->setPosition(Vec2(xPos, yPos));
    float paddingHeightFront = SpriteController::calculateScreenRatio(0.008f);

    frontHealthBar->setPosition(Vec2(xPos, yPos - paddingHeightFront));
    backHealthBar->setScale(SpriteController::updateSpriteScale(backHealthBar, 0.155f));
    frontHealthBar->setScale(SpriteController::updateSpriteScale(backHealthBar, 0.155f));

    this->addChild(backHealthBar, Constants::ORDER_LAYER_UI);
    this->addChild(healthBarCircle, Constants::ORDER_LAYER_UI);
    this->addChild(frontHealthBar, Constants::ORDER_LAYER_UI);

}

void Game3Scene::initBossHealthBar() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    bossHealthBar = CustomLoadingBar::create("assets_game/UXUI/Loading/BossbarGame3.png", "assets_game/UXUI/Loading/BossbarGame3_Border.png", 0.25f);
    //bossHealthBar->setLoadingBarRotation(-90);
    bossHealthBar->setLoadingBarPosition(Vec2(visibleSize.width / 2, bulletBadge->getPosition().y));

    // Adjust the border position to be lower than the loading bar
    auto loadingPos = bossHealthBar->getLoadingBar()->getPosition();
    float loadingBarHeight = SpriteController::calculateScreenHeightRatio(0.0035f);
    float loadingBarWeight = SpriteController::calculateScreenRatio(0.0011f);
    loadingPos.y -= loadingBarHeight; // Move the border lower
    loadingPos.x -= loadingBarWeight; // Move the border lower
    bossHealthBar->setBorderPosition(loadingPos);

    //bossHealthBar->setBorderRotation(-90);
    bossHealthBar->setPercent(0);
    bossHealthBar->setLoadingBarScale(SpriteController::updateSpriteScale(bossHealthBar->getLoadingBar(), 0.4325f));
    bossHealthBar->setBorderScale(SpriteController::updateSpriteScale(bossHealthBar->getBorder(), 0.5f));
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
    
    SoundController::getInstance()->playMusic(Constants::pathSoundTrackGame3, false);
}

void Game3Scene::initBulletSpawning(const std::string& jsonFilePath) {
     // Read JSON file
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFilePath);
    if (fileContent.empty()) {
        CCLOG("Failed to read JSON file: %s", jsonFilePath.c_str());
        return;
    }

    // Parse the JSON content
    rapidjson::Document document;
    document.Parse(fileContent.c_str());

    if (!document.IsObject() || !document.HasMember("spawnEvents") || !document["spawnEvents"].IsArray()) {
        CCLOG("Invalid JSON format: %s", jsonFilePath.c_str());
        return;
    }

    const rapidjson::Value& spawnEvents = document["spawnEvents"];
    for (rapidjson::SizeType i = 0; i < spawnEvents.Size(); ++i) {
        const rapidjson::Value& enemyData = spawnEvents[i];
        std::string enemyType = enemyData["enemyType"].GetString();
        float spawnTime = enemyData["spawnTime"].GetFloat();
        float skillTime = enemyData["skillTime"].GetFloat();
        bool spawnSkill = enemyData["spawnSkill"].GetBool();
        std::string direction = enemyData["direction"].GetString();
        std::string position = enemyData["position"].GetString();

        this->scheduleOnce([=](float) {
            if (enemyType == "EnemyPlaneBullet") {
                auto enemy = EnemyPlaneBulletPool::getInstance()->getObject();
                if (enemy) {
                    this->addChild(enemy);
                    enemy->spawnEnemy(this, skillTime, spawnSkill, direction, position);
                }
            }
            }, spawnTime, "spawn_enemy_bullet_key_" + std::to_string(spawnTime));
    }
}

void Game3Scene::initBoomSpawning(const std::string& jsonFilePath) {
    // Read JSON file
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFilePath);
    if (fileContent.empty()) {
        CCLOG("Failed to read JSON file: %s", jsonFilePath.c_str());
        return;
    }

    // Parse the JSON content
    rapidjson::Document document;
    document.Parse(fileContent.c_str());

    if (!document.IsObject() || !document.HasMember("spawnEvents") || !document["spawnEvents"].IsArray()) {
        CCLOG("Invalid JSON format: %s", jsonFilePath.c_str());
        return;
    }

    const rapidjson::Value& spawnEvents = document["spawnEvents"];
    for (rapidjson::SizeType i = 0; i < spawnEvents.Size(); ++i) {
        const rapidjson::Value& enemyData = spawnEvents[i];
        std::string enemyType = enemyData["enemyType"].GetString();
        float spawnTime = enemyData["spawnTime"].GetFloat();
        float skillTime = enemyData["skillTime"].GetFloat();
        bool spawnSkill = enemyData["spawnSkill"].GetBool();
        std::string direction = enemyData["direction"].GetString();
        std::string position = enemyData["position"].GetString();

        this->scheduleOnce([=](float) {
            if (enemyType == "EnemyPlaneBoom") {
                auto enemy = EnemyPlaneBoomPool::getInstance()->getObject();
                if (enemy) {
                    this->addChild(enemy);
                    enemy->spawnEnemy(this, skillTime, spawnSkill, direction, position);
                }
            }
            }, spawnTime, "spawn_enemy_boom_key_" + std::to_string(spawnTime));
    }
}

void Game3Scene::initBossSpawning(const std::string& jsonFilePath) {
    // Read JSON file
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFilePath);
    if (fileContent.empty()) {
        CCLOG("Failed to read JSON file: %s", jsonFilePath.c_str());
        return;
    }

    // Parse the JSON content
    rapidjson::Document document;
    document.Parse(fileContent.c_str());

    if (!document.IsObject() || !document.HasMember("bossSpawnEvents") || !document["bossSpawnEvents"].IsArray()) {
        CCLOG("Invalid JSON format: %s", jsonFilePath.c_str());
        return;
    }

    const rapidjson::Value& spawnEvents = document["bossSpawnEvents"];
    for (rapidjson::SizeType i = 0; i < spawnEvents.Size(); ++i) {
        const rapidjson::Value& enemyData = spawnEvents[i];

        std::string enemyType = enemyData["enemyType"].GetString();
        float spawnTime = enemyData["spawnTime"].GetFloat();
        float timeToUltimate = enemyData["timeToUltimate"].GetFloat();

        this->scheduleOnce([=](float) {
            if (enemyType == "EnemyPlaneBoss") {
                this->spawnOrUpdateBoss(timeToUltimate);
            }
            }, spawnTime, "spawn_boss_key_" + std::to_string(spawnTime));
    }
}

void Game3Scene::spawnOrUpdateBoss(float timeToUltimate) {
    CCLOG("CALLED spawnOrUpdateBoss");
        enemyBoss->updatePhase();
        enemyBoss->spawnEnemy(timeToUltimate);
}

void Game3Scene::initBoss() {
    enemyBoss = EnemyPlaneBoss::create();
    this->addChild(enemyBoss);
    enemyBoss->setVisible(false);
    __NotificationCenter::getInstance()->addObserver(
        this,
        callfuncO_selector(Game3Scene::transitionToNextScene),
        "FINISH_AND_CHANGE_SCENE",
        nullptr
    );

}

void Game3Scene::setupCursor() {
    _cursor = Cursor::create("assets_game/player/bullseye_white.png");
    if (!_cursor) {
        CCLOG("Failed to create Cursor");
        return;
    }
    _cursor->showCursor();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    _cursor->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _cursor->setName("Cursor");
    this->addChild(_cursor, Constants::ORDER_LAYER_UI - 10);
}

void Game3Scene::showCursor(Ref* sender) {
    if (_cursor) {
        _cursor->showCursor();
    }
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

void Game3Scene::initTouchListener() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Game3Scene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Game3Scene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool Game3Scene::onTouchBegan(Touch* touch, Event* event) {
    playerGame3->onMouseDown(nullptr); // Simulate mouse down event
    return true;
}

void Game3Scene::onTouchEnded(Touch* touch, Event* event) {
     playerGame3->onMouseUp(nullptr); // Simulate mouse up event
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
            Constants::QuantityBulletPlayerGame3 += 70;
            updateBulletLabel(nullptr);
        }
        else if (player && itemUpgradeBullet) {
            itemUpgradeBullet->stopMovement();
            itemUpgradeBullet->applyPickupEffect();
            player->increaseBulletCount();
        }
        else if (player && HealthRecovery) {
            HealthRecovery->stopMovement();
            HealthRecovery->applyPickupEffect();
            if (currentHealthCircle <= 0) return true;
            // Assuming you have a method to get the current health
            float currentHealth = currentHealthCircle;
            float newHealth = currentHealth + 10; // Decrease health by 10 (example value)
            updateHealth(newHealth);
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
                IncreaseBullet->stopMovement();
                IncreaseBullet->applyPickupEffect();
                Constants::QuantityBulletPlayerGame3 += 100;
                updateBulletLabel(nullptr);
            }
            else if (player && HealthRecovery) {
                HealthRecovery->applyPickupEffect();
                if (currentHealthCircle <= 0) return true;
                // Assuming you have a method to get the current health
                float currentHealth = currentHealthCircle;
                float newHealth = currentHealth + 10; // Decrease health by 10 (example value)
                updateHealth(newHealth);
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
    else if (enemyBoss == dynamic_cast<EnemyPlaneBoss*>(enemy)) {
        this->handleBossDamage(Constants::BulletDamageGame3);
        bullet->hideModelCharac();
        bullet->explode();
    }
}

void Game3Scene::handleBulletForEnemyCityCollision(BulletForEnemyPlane* bulletForEnemy) {
    bulletForEnemy->explode();
    if (currentHealthCircle <= 0) return;
    // Assuming you have a method to get the current health
    float currentHealth = currentHealthCircle;
    float newHealth = currentHealth - Constants::DamageBulletForCity; // Decrease health by 10 (example value)
    updateHealth(newHealth);
}

void Game3Scene::handleMissileEnemyCityCollision(MissileForEnemyPlane* missileEnemy) {
    missileEnemy->explode();
    if (currentHealthCircle <= 0) return;
    // Assuming you have a method to get the current health
    float currentHealth = currentHealthCircle;
    float newHealth = currentHealth - Constants::DamageBoomForCity; // Decrease health by 10 (example value)
    updateHealth(newHealth);
}

void Game3Scene::handleBoomCityCollision(BoomForEnemyPlane* boom) {
    boom->explode();

    if (currentHealthCircle <= 0) return;
    float currentHealth = currentHealthCircle;
    float newHealth = currentHealth - Constants::DamageBoomForCity; // Decrease health by 10 (example value)
    updateHealth(newHealth);
}

void Game3Scene::checkHealthBar() {
    if (currentHealthCircle <= 0) {
        this->stopAllActions();
        GameController::getInstance()->GameOver(
            []() { Director::getInstance()->end(); }, // Exit action
            []() -> Scene* { return Game3Scene::createScene(); }, // Create scene function
            Constants::currentSoundTrackPath // Soundtrack path
        );
    }
}

void Game3Scene::handleBossDamage(float damage) {
    if (enemyBoss) {
        enemyBoss->takeDamage(damage);
        if (enemyBoss->getHealth() < 1) {
            this->unschedule("UltimateSkillCountdown");

            if (enemyBoss->getCurrentPhase() == Phase::PHASE_1) {
                // Transition to Phase 2
                SoundController::getInstance()->playMusic(Constants::pathSoundTrackGame3Phase2, false);
                initBulletSpawning(Constants::JSON_GAME3_ENEMYBULLET_PHASE_2_PATH);
                initBoomSpawning(Constants::JSON_GAME3_ENEMYBOOM_PHASE_2_PATH);
                initBossSpawning(Constants::JSON_GAME3_BOSS_PHASE_2_PATH);
            }
            else
            {
                playerGame3->setMovementAndShootingDisabled(true);
            }
        }
        
        this->updateBossHealthBar(nullptr);
    }
}

void Game3Scene::transitionToNextScene(Ref* sender) {
    UserDefault::getInstance()->setBoolForKey(Constants::UD_VICTORY.c_str(), true);
    auto nextScene = SceneFinishGame::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, nextScene));
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
        currentHealthCircle = 0;

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
                    Constants::currentSoundTrackPath // Add the soundtrack path here
                );
                }),
            nullptr
        ));
    }
}

void Game3Scene::initControlButtons() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
	float paddingTop = SpriteController::calculateScreenRatio(0.1f);
	float paddingWidth = SpriteController::calculateScreenRatio(0.1f);
	float spacingWidth = SpriteController::calculateScreenRatio(0.18f);

    // Create left button
	auto spriteButton = Sprite::create("assets_game/UXUI/Panel/Next_Round_BTN.png");
    leftButton = cocos2d::ui::Button::create("assets_game/UXUI/Panel/Back_Round_BTN.png", "assets_game/UXUI/Panel/Back_Round_BTN_Active.png");
    leftButton->setScale(SpriteController::updateSpriteScale(spriteButton , 0.3));
    leftButton->setPosition(Vec2(origin.x / 2 + paddingWidth,
        origin.y / 2 + paddingTop));
    leftButton->setOpacity(120);
    leftButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            playerGame3->startMovingLeft();
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            playerGame3->stopMoving();
            break;
        default:
            break;
        }
        });
    this->addChild(leftButton, Constants::ORDER_LAYER_UI - 5);

    // Create right button
    rightButton = cocos2d::ui::Button::create("assets_game/UXUI/Panel/Next_Round_BTN.png", "assets_game/UXUI/Panel/Next_Round_BTN_Active.png");
    rightButton->setScale(SpriteController::updateSpriteScale(spriteButton, 0.3));
    rightButton->setPosition(Vec2(leftButton->getPositionX() + spacingWidth,
        leftButton->getPositionY()));
    rightButton->setOpacity(120);
    rightButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            playerGame3->startMovingRight();
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            playerGame3->stopMoving();
            break;
        default:
            break;
        }
        });
    this->addChild(rightButton, Constants::ORDER_LAYER_UI - 5);
}