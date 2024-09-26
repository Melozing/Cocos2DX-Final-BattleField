#include "Game1/Game1Scene.h"
#include "Enemy/FlyingBullet.h"
#include "Enemy/FallingRock.h"
#include "Enemy/RandomBoom.h"
#include "Enemy/EnemyFactory.h"
#include "Enemy/EnemyPool.h"
#include "Game1/Player/HealthPlayerGame1.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "Controller/GameController.h" // Ensure GameController is included

USING_NS_CC;

Scene* Game1Scene::createScene() {
    auto scene = Scene::createWithPhysics(); // Create scene with physics
    scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game1Scene::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

bool Game1Scene::init() {
    if (!Scene::init()) {
        return false;
    }

    _playerAttributes = new PlayerAttributes(3); // Initialize player attributes
    _canTakeDamage = true; // Allow damage initially

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Create edge physics body
    auto edgeBody = cocos2d::PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 0);
    edgeBody->setCollisionBitmask(0x03);
    edgeBody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    addChild(edgeNode);

    // Initialize enemy pool
    EnemyPool::getInstance()->initPool("FlyingBullet", 10);
    EnemyPool::getInstance()->initPool("FallingRock", 5);
    EnemyPool::getInstance()->initPool("RandomBoom", 5);

    // Create background
    background = Background::createBackground("assets_game/gameplay/background.png", 150.0f);
    this->addChild(background);

    // Create player
    _player = PlayerGame1::createPlayer();
    _player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    _healthPlayerGame1 = HealthPlayerGame1::createHealth();
    _healthPlayerGame1->initHealthSprite`s(_playerAttributes->GetHealth());
    this->addChild(_healthPlayerGame1);

    auto playerBody = PhysicsBody::createBox(_player->GetSize());
    setPhysicsBodyChar(playerBody, 0x01);
    _player->setPhysicsBody(playerBody);
    addChild(_player);

    // Handle player movement
    _movingUp = _movingDown = _movingLeft = _movingRight = false;

    // Keyboard listener for player movement
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W:
            _movingUp = true;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S:
            _movingDown = true;
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A:
            _movingLeft = true;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D:
            _movingRight = true;
            break;
        default:
            break;
        }
        };

    listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W:
            _movingUp = false;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S:
            _movingDown = false;
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A:
            _movingLeft = false;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D:
            _movingRight = false;
            break;
        default:
            break;
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game1Scene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Schedule player movement handling
    this->schedule([this](float dt) {
        handlePlayerMovement();
        }, "update_key_schedule");

    this->scheduleUpdate(); // Schedule the update function to be called each frame
    this->scheduleEnemySpawning(); // Ensure enemies are scheduled for spawning

    return true;
}

void Game1Scene::setPhysicsBodyChar(PhysicsBody* physicBody, int num) {
    physicBody->setCollisionBitmask(num);
    physicBody->setContactTestBitmask(true);
    physicBody->setDynamic(false);
}

bool Game1Scene::onContactBegin(PhysicsContact& contact) {
    if (_playerAttributes->IsDead()) return true;
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // Check for player vs enemy collisions
    if ((bodyA->getCollisionBitmask() == 0x01 && bodyB->getCollisionBitmask() == 0x02) ||
        (bodyA->getCollisionBitmask() == 0x02 && bodyB->getCollisionBitmask() == 0x01)) {

        if (_canTakeDamage) {
            CCLOG("Collision detected!");
            _playerAttributes->TakeDamage(1); // Reduce player health
            _canTakeDamage = false;

            // Update health sprites based on current health
            _healthPlayerGame1->updateHealthSprites(_playerAttributes->GetHealth());

            // Schedule to allow damage again after 1 second
            this->scheduleOnce([this](float) {
                _canTakeDamage = true;
                }, 1.0f, "damage_delay_key");

            // Check if the player is dead
            if (_playerAttributes->IsDead()) {
                CCLOG("Player is dead!");
                CCLOG("Calling Game Over...");
                GameController::getInstance()->GameOver(_playerAttributes); // Transition to game over screen
            }
        }
    }

    return true;
}

void Game1Scene::update(float delta) {
    background->update(delta);
    for (auto enemy : _enemyPool) {
        onEnemyOutOfBounds(enemy);
    }
}

void Game1Scene::handlePlayerMovement() {
    if (_movingUp) {
        _player->moveUp();
    }
    if (_movingDown) {
        _player->moveDown();
    }
    if (_movingLeft) {
        _player->moveLeft();
    }
    if (_movingRight) {
        _player->moveRight();
    }
}

void Game1Scene::spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position) {
    Enemy* enemy = EnemyPool::getInstance()->getEnemy();
    if (enemy) {
        enemy->setPosition(position);
        this->addChild(enemy);
        enemy->setVisible(true);
    }
    else {
        enemy = EnemyFactory::spawnEnemy(enemyType, position);
        if (enemy) {
            enemy->setPosition(position);
            this->addChild(enemy);
        }
    }
}

void Game1Scene::returnEnemyToPool(cocos2d::Node* enemy) {
    enemy->setVisible(false);
    enemy->stopAllActions();
    // Add the enemy back to the pool for reuse
    EnemyPool::getInstance()->returnEnemy(static_cast<Enemy*>(enemy));
}

void Game1Scene::onEnemyOutOfBounds(cocos2d::Node* enemy) {
    auto enemyPos = enemy->getPosition();
    auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    if (enemyPos.x < 0 || enemyPos.x > screenSize.width || enemyPos.y < 0 || enemyPos.y > screenSize.height) {
        returnEnemyToPool(enemy);
    }
}

void Game1Scene::scheduleEnemySpawning() {
    this->schedule([this](float dt) {
        if (_player) {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            SpawnFlyingBullet(visibleSize, (rand() % 2 == 0));
        }
        }, 7.5f, "flying_bullet_spawn_key");

    this->schedule([this](float dt) {
        if (_player) {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            SpawnFallingRockAndBomb(visibleSize);
        }
        }, 3.0f, "falling_rock_spawn_key");

    this->schedule([this](float dt) {
        if (_player) {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            SpawnRandomBoom(visibleSize);
        }
        }, 4.0f, "random_boom_spawn_key");
}

void Game1Scene::SpawnFallingRockAndBomb(cocos2d::Size size) {
    float restrictedWidth = SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH);
    float centerX = size.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    float minX = centerX - halfRestrictedWidth;
    float maxX = centerX + halfRestrictedWidth;

    int spawnOption = rand() % 3;
    Vec2 spawnPosition;
    CCLOG("%f", SpriteController::calculateScreenRatio(Constants::FALLINGROCK_PADDING));
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

    auto fallingRock = FallingRock::create();
    if (fallingRock) {
        fallingRock->spawn(spawnPosition);
        auto size = fallingRock->GetSize();
        auto fallingRockBody = PhysicsBody::createCircle(size.width / 2); // Adjust for radius

        setPhysicsBodyChar(fallingRockBody, 0x02);
        fallingRock->setPhysicsBody(fallingRockBody);
        this->addChild(fallingRock);
    }
}

void Game1Scene::SpawnFlyingBullet(cocos2d::Size size, bool directionLeft) {
    Vec2 spawnPosition = directionLeft ? Vec2(-50, _player->getPosition().y) : Vec2(size.width + 50, _player->getPosition().y);

    auto flyingBullet = FlyingBullet::create();
    if (flyingBullet) {
        flyingBullet->initAnimation(directionLeft);
        flyingBullet->setPosition(spawnPosition);

        float targetX = directionLeft ? size.width + 50 : -50;
        auto moveAction = MoveTo::create(Constants::FLYING_BULLET_SPEED, Vec2(targetX, _player->getPosition().y));

        auto sequence = Sequence::create(moveAction, CallFunc::create([flyingBullet]() {
            flyingBullet->removeFromParent();
            }), nullptr);

        auto flyingBulletBody = PhysicsBody::createBox(flyingBullet->GetSize());
        setPhysicsBodyChar(flyingBulletBody, 0x02);
        flyingBullet->setPhysicsBody(flyingBulletBody);

        flyingBullet->runAction(sequence);
        this->addChild(flyingBullet);
    }
}

void Game1Scene::SpawnRandomBoom(cocos2d::Size size) {
    // Define the width and height of the restricted movement area
    float restrictedWidth = 100.0f; // The width of the restricted movement area
    float restrictedHeight = size.height - 100.0f; // The height of the restricted movement area
    float centerX = size.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    float minX = centerX - halfRestrictedWidth;
    float maxX = centerX + halfRestrictedWidth;

    // Randomly decide the spawn location
    int spawnOption = rand() % 3; // 0 for center, 1 for right edge, 2 for left edge

    Vec2 spawnPosition;
    switch (spawnOption) {
    case 0: // Spawn in the center
        spawnPosition = Vec2(centerX, size.height + 50); // Center of the restricted area
        break;
    case 1: // Spawn near the right edge
        spawnPosition = Vec2(maxX - 10, size.height + 50); // Right edge
        break;
    case 2: // Spawn near the left edge
        spawnPosition = Vec2(minX + 10, size.height + 50); // Left edge
        break;
    }

    auto randomBoom = RandomBoom::create();
    if (randomBoom) {
        randomBoom->spawn(spawnPosition);
        this->addChild(randomBoom);
    }
}
