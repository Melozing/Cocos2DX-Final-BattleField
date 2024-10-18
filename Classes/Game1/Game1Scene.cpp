#include "Game1/Game1Scene.h"
#include "Enemy/FlyingBulletPool.h"
#include "Enemy/FallingRockPool.h"
#include "Enemy/RandomBoomPool.h"
#include "Enemy/FanBulletPool.h"
#include "Enemy/EnemyFactory.h"
#include "Game1/Items/AmmoItemPool.h"
#include "Game1/Items/HealthItemPool.h"
#include "Game1/Player/HealthPlayerGame1.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "Controller/GameController.h"
#include "Manager/PositionManager.h"
#include "Constants/Constants.h"
#include "ui/UILoadingBar.h"
#include "utils/Music/MusicEvent.h"
#include "utils/Music/AudioUtils.h"
#include "audio/include/AudioEngine.h"
#include <ctime> 
#include "json/document.h"
#include "json/filereadstream.h"
#include <fstream>

USING_NS_CC;
using namespace cocos2d::experimental;

cocos2d::Scene* Game1Scene::createScene() {
    auto scene = Scene::createWithPhysics(); // Create scene with physics
    scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game1Scene::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

bool Game1Scene::init() {
    if (!BaseScene::init()) return false;

    srand(static_cast<unsigned int>(time(nullptr)));

    this->setSceneCreationFunc([]() -> cocos2d::Scene* {
        return Game1Scene::createScene();
        });

    _isGameOver = false;
    PlayerAttributes::getInstance().SetHealth(Constants::PLAYER_HEALTH);
    _playerAttributes = &PlayerAttributes::getInstance();
    _canTakeDamage = true;

    // Initialize shouldPlayMusic
    shouldPlayMusic = true; // or some condition to determine if music should play

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto edgeBody = cocos2d::PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 0);
    edgeBody->setCollisionBitmask(0x03);
    edgeBody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    addChild(edgeNode);

    background = Background::createBackground("assets_game/gameplay/bg_new.jpg", 150.0f);
    this->addChild(background, Constants::ORDER_LAYER_BACKGROUND);

    FlyingBulletPool::getInstance()->initPool(10);
    FallingRockPool::getInstance()->initPool(10);
    RandomBoomPool::getInstance()->initPool(10);
    FanBulletPool::getInstance()->initPool(30);

    _player = PlayerGame1::createPlayer();
    _player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _healthPlayerGame1 = HealthPlayerGame1::createHealth();
    _healthPlayerGame1->initHealthSprites(_playerAttributes->GetHealth());
    this->addChild(_healthPlayerGame1, Constants::ORDER_LAYER_UI);

    auto playerBody = PhysicsBody::createBox(_player->GetSize());
    setPhysicsBodyChar(playerBody, 0x01);
    _player->setPhysicsBody(playerBody);
    addChild(_player, Constants::ORDER_LAYER_CHARACTER);

    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            if (_player) {
                _player->onKeyPressed(keyCode, event);
            }
            break;
        default:
            break;
        }
        };

    eventListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            if (_player) {
                _player->onKeyReleased(keyCode, event);
            }
            break;
        default:
            break;
        }
        };

    auto _spriteLoading = Sprite::create("assets_game/UXUI/Loading/Load_Bar_Fg.png");
    auto _spriteLoadingBorder = Sprite::create("assets_game/UXUI/Loading/Load_Bar_Bg.png");

    _loadingBar = cocos2d::ui::LoadingBar::create("assets_game/UXUI/Loading/Load_Bar_Fg.png");
    _loadingBar->setDirection(cocos2d::ui::LoadingBar::Direction::LEFT);
    _loadingBar->setRotation(-90);
    _loadingBar->setScale(SpriteController::updateSpriteScale(_spriteLoading, 0.25f));
    _loadingBar->setPercent(0);
    _loadingBar->setAnchorPoint(Vec2(0.5, 0.5));
    _loadingBar->setPosition(Vec2(visibleSize.width - _loadingBar->getContentSize().height / 2, visibleSize.height / 2));

    border = Sprite::create("assets_game/UXUI/Loading/Load_Bar_Bg.png");
    auto loadingPos = _loadingBar->getPosition();
    border->setPosition(loadingPos);
    border->setScale(SpriteController::updateSpriteScale(_spriteLoading, 0.25f));
    border->setRotation(-90);
    border->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(border, Constants::ORDER_LAYER_UI);
    this->addChild(_loadingBar, Constants::ORDER_LAYER_UI);

    auto randomPosition = PositionManager::getInstance().getRandomSpawnPosition(visibleSize);
    while (PositionManager::getInstance().isPositionOccupied(randomPosition)) {
        randomPosition = PositionManager::getInstance().getRandomSpawnPosition(visibleSize);
    }
    PositionManager::getInstance().addOccupiedPosition(randomPosition);

    if (!FileUtils::getInstance()->isFileExist(Constants::pathSoundTrackGame1)) {
        CCLOG("Error: Music file does not exist!");
        return false;
    }

    SoundController::getInstance()->preloadMusic(Constants::pathSoundTrackGame1);
    SoundController::getInstance()->playMusic(Constants::pathSoundTrackGame1, true);
    SoundController::getInstance()->setMusicVolume(Constants::pathSoundTrackGame1, 0.0f);

    _musicAnalyzer = MusicAnalyzer::getInstance();

    if (MusicAnalyzer::getInstance()->isMusicPlaying) {
        //_musicAnalyzer->replayMusic();
        //GameController::getInstance()->audioID = AudioUtils::restartMusic(Constants::pathSoundTrackGame1);
        //GameController::getInstance()->musicStarted = true;
    }
    else {
        _musicAnalyzer->analyzeMusic(Constants::pathSoundTrackGame1);
    }


    this->scheduleOnce([this](float) {
        musicDuration = SoundController::getInstance()->getMusicDuration(Constants::pathSoundTrackGame1);
        }, 0.1f, "get_music_duration_key");

    this->schedule([this](float dt) {
        updateLoadingBar(dt);
        }, "loading_bar_update_key");

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game1Scene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();
    this->scheduleCollectibleSpawning();

    //this->schedule([this](float dt) { handleMusicBasedSpawning(dt); }, "music_based_spawning_key");
     // Initialize the enemy spawn map
    enemySpawnMap["FlyingBullet"] = [this](const cocos2d::Size& size) { SpawnFlyingBullet(size, (rand() % 2 == 0)); };
    enemySpawnMap["FallingRock"] = [this](const cocos2d::Size& size) { SpawnFallingRockAndBomb(size); };
    enemySpawnMap["RandomBoom"] = [this](const cocos2d::Size& size) { SpawnRandomBoom(size); };
    enemySpawnMap["FanBullet"] = [this](const cocos2d::Size& size) { SpawnFanBullet(size); };

    // Initialize the spawn schedule from JSON
    initializeSpawnSchedule();

    // Schedule the spawning based on the JSON file
    this->schedule([this](float dt) {
        static float elapsedTime = 0.0f;
        elapsedTime += dt;

        for (auto& event : spawnSchedule) {
            if (!event.spawned && elapsedTime >= event.spawnTime) {
                enemySpawnMap[event.enemyType](Director::getInstance()->getVisibleSize());
                event.spawned = true; // Mark as spawned
            }
        }
        }, "json_based_spawning_key");

    exitAction = []() {
        Director::getInstance()->end();
        };

    createSceneFunc = []() -> cocos2d::Scene* {
        return Game1Scene::createScene();
        };

    return true;
}


void Game1Scene::setPhysicsBodyChar(PhysicsBody* physicBody, int num) {
    physicBody->setCollisionBitmask(num);
    physicBody->setContactTestBitmask(true);
    physicBody->setDynamic(false);
}

bool Game1Scene::onContactBegin(PhysicsContact& contact) {
    return true;
    if (_playerAttributes->IsDead()) return true;

    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    if ((bodyA->getCollisionBitmask() == 0x01 && bodyB->getCollisionBitmask() == 0x02) ||
        (bodyA->getCollisionBitmask() == 0x02 && bodyB->getCollisionBitmask() == 0x01)) {
        if (_canTakeDamage) {
            CCLOG("Collision detected with enemy!");
            _playerAttributes->TakeDamage(1);
            _canTakeDamage = false;
            _healthPlayerGame1->updateHealthSprites(_playerAttributes->GetHealth());
            this->scheduleOnce([this](float) { _canTakeDamage = true; }, 1.0f, "damage_delay_key");

            this->checkGameOver();
        }
    }

    if ((bodyA->getCollisionBitmask() == 0x01 && bodyB->getCollisionBitmask() == 0x03) ||
        (bodyA->getCollisionBitmask() == 0x03 && bodyB->getCollisionBitmask() == 0x01)) {
        auto collectible = static_cast<CollectibleItem*>(bodyA->getNode() == _player ? bodyB->getNode() : bodyA->getNode());
        if (collectible) {
            collectible->applyEffect(); // Apply the effect of the collectible item

            // Check the type of collectible item and apply the corresponding effect
            if (auto healthItem = dynamic_cast<HealthItem*>(collectible)) {
                _healthPlayerGame1->updateHealthSprites(_playerAttributes->GetHealth()); // Update health sprites
            }

            collectible->removeFromParentAndCleanup(true);
        }
    }

    return true;
}

void Game1Scene::checkGameOver() {
    if (_playerAttributes->IsDead()) {
        GameController::getInstance()->GameOver(
            []() {
                Director::getInstance()->end();
            },
            []() -> Scene* {
                return Game1Scene::createScene();
            },
            Constants::pathSoundTrackGame1 // Add the soundtrack path here
        );
        _isGameOver = true;
    }
}

void Game1Scene::updateLoadingBar(float dt) {
    if (_isGameOver) return;

    float currentPercent = _loadingBar->getPercent();
    if (musicDuration > 0) {
        float increment = (dt / musicDuration) * 100.0f;
        currentPercent += increment;

        if (currentPercent >= 100.0f) {
            currentPercent = 100.0f;
            GameController::getInstance()->Victory(
                []() {
                    Director::getInstance()->end();
                },
                []() -> Scene* {
                    return Game1Scene::createScene();
                },
                Constants::pathSoundTrackGame1
            );
            _isGameOver = true;
        }
        _loadingBar->setPercent(currentPercent);
    }
}

void Game1Scene::update(float delta) {
    background->update(delta);
    _musicAnalyzer->update(delta);
}

//void Game1Scene::handlePlayerMovement() {
//    if (_isGameOver) return;
//    if (_movingUp) _player->moveUp();
//    if (_movingDown) _player->moveDown();
//    if (_movingLeft) _player->moveLeft();
//    if (_movingRight) _player->moveRight();
//}

// Function to initialize the spawn schedule from JSON
void Game1Scene::initializeSpawnSchedule() {
    // Open the JSON file
    std::ifstream ifs("json/spawn_enemies_game1.json");
    if (!ifs.is_open()) {
        CCLOG("Error: Could not open JSON file!");
        return;
    }

    // Read the file into a string
    std::string jsonContent((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    // Parse the JSON content
    rapidjson::Document document;
    if (document.Parse(jsonContent.c_str()).HasParseError()) {
        CCLOG("Error: JSON parse error!");
        return;
    }

    // Check if the document is an array
    if (!document.IsArray()) {
        CCLOG("Invalid JSON format: Expected an array");
        return;
    }

    // Iterate through the array and populate the spawn schedule
    for (const auto& item : document.GetArray()) {
        if (item.HasMember("spawnTime") && item.HasMember("enemyType") &&
            item["spawnTime"].IsFloat() && item["enemyType"].IsString()) {
            float spawnTime = item["spawnTime"].GetFloat();
            std::string enemyType = item["enemyType"].GetString();
            spawnSchedule.emplace_back(spawnTime, enemyType, false); // Add false to indicate not spawned yet
        }
        else {
            CCLOG("Invalid JSON format: Missing required fields or incorrect types");
        }
    }
}


void Game1Scene::handleMusicBasedSpawning(float dt) {
    auto events = _musicAnalyzer->getMusicEvents(dt);
    for (const auto& event : events) {
        spawnBasedOnMusicEvent(event);
    }
}

void Game1Scene::spawnBasedOnMusicEvent(MusicEvent event) {
    static float lastSpawnTimeBullet = 0.0f; // Make lastSpawnTimeBullet static to retain its value between calls
    static float lastSpawnTimeRandomBoom = 0.0f; // Make lastSpawnTimeRandomBoom static to retain its value between calls
    static float lastSpawnTimeRockAndBoom = 0.0f; // Make lastSpawnTime static to retain its value between calls
    float spawnCooldownBullet = 2.0f; // Cooldown time in seconds for bullets
    float spawnCooldownRandomBoom = 2.0f; // Cooldown time in seconds for random booms
    float spawnCooldownRockAndBoom = 1.9f; // General cooldown time in seconds
    auto currentTime = Director::getInstance()->getTotalFrames() / 60.0f; // Assuming 60 FPS

    auto visibleSize = Director::getInstance()->getVisibleSize();

    switch (event.getType()) {
    case MusicEventType::BEAT:
        //CCLOG("Spawning FlyingBullet for BEAT event");
        if (currentTime - lastSpawnTimeBullet < spawnCooldownBullet) {
            return; // Skip spawning if cooldown has not passed
        }
        lastSpawnTimeBullet = currentTime;
        SpawnFlyingBullet(visibleSize, (rand() % 2 == 0));
        SpawnFanBullet(visibleSize);
        break;
    case MusicEventType::KICK:
        //CCLOG("Spawning FallingRock for KICK event");
        if (currentTime - lastSpawnTimeRockAndBoom < spawnCooldownRockAndBoom) {
            return; // Skip spawning if cooldown has not passed
        }
        lastSpawnTimeRockAndBoom = currentTime;
        SpawnFallingRockAndBomb(visibleSize);
        break;

    case MusicEventType::SNARE:
         //CCLOG("Spawning RandomBoom for SNARE event");
         if (currentTime - lastSpawnTimeRandomBoom < spawnCooldownRandomBoom) {
                return; // Skip spawning if cooldown has not passed
         }
         lastSpawnTimeRandomBoom = currentTime;
         SpawnRandomBoom(visibleSize);
         break;
    default:
        break;
    }
}

Vec2 Game1Scene::getRandomSpawnPosition(const Size& size) {
    float restrictedWidth = SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH);
    float centerX = size.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    float minX = centerX - halfRestrictedWidth;
    float maxX = centerX + halfRestrictedWidth;

    int spawnOption = rand() % 3;
    Vec2 spawnPosition;
    switch (spawnOption) {
    case 0:
        spawnPosition = Vec2(centerX, size.height + SpriteController::calculateScreenRatio(Constants::FALLINGROCK_START_Y));
        break;
    case 1:
        spawnPosition = Vec2(maxX - SpriteController::calculateScreenRatio(Constants::FALLINGROCK_PADDING), size.height + SpriteController::calculateScreenRatio(Constants::FALLINGROCK_START_Y));
        break;
    case 2:
        spawnPosition = Vec2(minX + SpriteController::calculateScreenRatio(Constants::FALLINGROCK_PADDING), size.height + SpriteController::calculateScreenRatio(Constants::FALLINGROCK_START_Y));
        break;
    }
    return spawnPosition;
}

void Game1Scene::SpawnFanBullet(cocos2d::Size size) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    int numBullets = 5;
    float angleSpread = 95.0f;
    float startAngle = -angleSpread / 2;
    float angleIncrement = angleSpread / (numBullets - 1);

    // Randomly choose an edge: 0 for top, 1 for left, 2 for right
    int edge;
    do {
        edge = rand() % 3;
    } while (std::find(previousSpawnEdges.begin(), previousSpawnEdges.end(), edge) != previousSpawnEdges.end());

    previousSpawnEdges.push_back(edge);
    if (previousSpawnEdges.size() > 2) {
        previousSpawnEdges.erase(previousSpawnEdges.begin());
    }
    Vec2 spawnPosition;
    float baseAngle;
    float offset = SpriteController::calculateScreenRatio(Constants::FANBULLET_OFFSET); // Adjust this value to move the spawn position outside the screen
    switch (edge) {
    case 0: // Top edge
        spawnPosition = Vec2(visibleSize.width / 2, visibleSize.height + offset);
        baseAngle = -90.0f;
        break;
    case 1: // Left edge
        spawnPosition = Vec2(-offset, visibleSize.height / 2);
        baseAngle = 0.0f;
        break;
    case 2: // Right edge
        spawnPosition = Vec2(visibleSize.width + offset, visibleSize.height / 2);
        baseAngle = 180.0f;
        break;
    }

    for (int i = 0; i < numBullets; ++i) {
        float angle = baseAngle + startAngle + i * angleIncrement;
        FanBullet* fanBullet = FanBulletPool::getInstance()->getEnemy();
        if (fanBullet) {
            // Spawn the FanBullet
            fanBullet->spawn(spawnPosition, angle);
            // Add the FanBullet to the scene
            this->addChild(fanBullet, Constants::ORDER_LAYER_CHARACTER);
        }
    }
}

void Game1Scene::SpawnFallingRockAndBomb(Size size) {
    Vec2 spawnPosition = getRandomSpawnPosition(size);

    if (!isPositionOccupied(spawnPosition)) {
        auto fallingRock = FallingRockPool::getInstance()->getEnemy();
        if (fallingRock) {
            fallingRock->reset();
            fallingRock->spawn(spawnPosition);
            auto size = fallingRock->GetSize();
            auto fallingRockBody = PhysicsBody::createCircle(size.width / 2);
            setPhysicsBodyChar(fallingRockBody, 0x02);
            fallingRock->setPhysicsBody(fallingRockBody);
            this->addChild(fallingRock, Constants::ORDER_LAYER_CHARACTER - 1);
        }
    }
}

void Game1Scene::SpawnFlyingBullet(cocos2d::Size size, bool directionLeft) {
    Vec2 spawnPosition = directionLeft ? Vec2(-50, _player->getPosition().y) : Vec2(size.width + 50, _player->getPosition().y);

    auto flyingBullet = FlyingBulletPool::getInstance()->getEnemy();
    if (flyingBullet) {
        flyingBullet->reset();
        flyingBullet->initAnimation(directionLeft);
        flyingBullet->setPosition(spawnPosition);

        float targetX = directionLeft ? size.width + 50 : -50;
        auto moveAction = MoveTo::create(Constants::FLYING_BULLET_SPEED, Vec2(targetX, _player->getPosition().y));

        auto sequence = Sequence::create(moveAction, CallFunc::create([flyingBullet, this]() {
            flyingBullet->removeFromParent();
            FlyingBulletPool::getInstance()->returnEnemy(flyingBullet);
            }), nullptr);

        Size reducedSize = Size(flyingBullet->GetSize().width * 0.65, flyingBullet->GetSize().height * 0.65); // Reduce size by 10%
        auto flyingBulletBody = PhysicsBody::createBox(reducedSize);
        setPhysicsBodyChar(flyingBulletBody, 0x02);
        flyingBullet->setPhysicsBody(flyingBulletBody);

        flyingBullet->runAction(sequence);
        this->addChild(flyingBullet, Constants::ORDER_LAYER_CHARACTER - 1);
    }
}

void Game1Scene::SpawnRandomBoom(cocos2d::Size size) {
    Vec2 spawnPosition = getRandomSpawnPosition(size);

    auto randomBoom = RandomBoomPool::getInstance()->getEnemy();
    if (randomBoom) {
        randomBoom->spawn(spawnPosition);
        this->addChild(randomBoom, Constants::ORDER_LAYER_CHARACTER - 1);
    }
}

void Game1Scene::SpawnCollectibleItem(const Size& size) {
    Vec2 spawnPosition = getRandomSpawnPosition(size);
    if (!isPositionOccupied(spawnPosition)) {
        // Randomly choose between HealthItem and AmmoItem
        CollectibleItem* item = nullptr;
        if (rand() % 2 == 0) {
            item = HealthItemPool::getInstance()->getItem();
        }
        else {
            item = AmmoItemPool::getInstance()->getItem();
        }
        if (item) {
            item->spawn(spawnPosition);

            // Retrieve the scaled size of the item
            Size itemSize = item->getScaledSize();

            // Create and set the physics body using the scaled size
            auto itemBody = PhysicsBody::createBox(itemSize);
            setPhysicsBodyChar(itemBody, 0x03);
            item->setPhysicsBody(itemBody);

            this->addChild(item, Constants::ORDER_LAYER_CHARACTER - 1);
        }
        else {
            CCLOG("Failed to create CollectibleItem");
        }
    }
}

void Game1Scene::trackUsedPosition(const Vec2& position) {
    usedPositions.push_back(position);
}

// Schedule the spawning of collectible items
void Game1Scene::scheduleCollectibleSpawning() {
    this->schedule([this](float dt) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        this->SpawnCollectibleItem(visibleSize); // Call the function to spawn items
        }, 5.0f, "collectible_item_spawn_key"); // Adjust the interval as needed
}

bool Game1Scene::isPositionOccupied(const Vec2& position) {
    for (auto child : this->getChildren()) {
        // Check for CollectibleItem
        if (auto collectible = dynamic_cast<CollectibleItem*>(child)) {
            if (collectible->getPosition().distance(position) < collectible->GetSize().width) {
                return true; // Position is occupied
            }
        }
        // Check for FallingRock
        else if (auto fallingRock = dynamic_cast<FallingRock*>(child)) {
            if (fallingRock->getPosition().distance(position) < fallingRock->GetSize().width) {
                return true; // Position is occupied
            }
        }
    }
    return false; // Position is free
}
