#include "Splash.h"
#include "Enemy/FlyingBullet.h"
#include "Enemy/FallingRock.h"
#include "Enemy/RandomBoom.h"
#include "Enemy/EnemyFactory.h"
#include "Enemy/EnemyPool.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"

USING_NS_CC;

Scene* Splash::createScene() {
    auto scene = Scene::create();
    auto layer = Splash::create();
    scene->addChild(layer);
    return scene;
}

bool Splash::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Initialize enemy pool
    EnemyPool::getInstance()->initPool("FlyingBullet", 10);
    EnemyPool::getInstance()->initPool("FallingRock", 5);
    EnemyPool::getInstance()->initPool("RandomBoom", 5);

    // Create background
    background = Background::createBackground("assets_game/gameplay/background.png", 150.0f);
    this->addChild(background);

    // Create player
    _player = Player::createPlayer();
    _player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    addChild(_player);

    // Handle player movement
    _movingUp = _movingDown = _movingLeft = _movingRight = false;

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

    this->schedule([this](float dt) {
        handlePlayerMovement();
        }, "update_key_schedule");

    this->scheduleUpdate(); // Schedule the update function to be called each frame
    this->scheduleEnemySpawning(); // Ensure enemies are scheduled for spawning

    return true;
}

void Splash::update(float delta) {
    background->update(delta);
    for (auto enemy : _enemyPool) {
        onEnemyOutOfBounds(enemy);
    }
}

void Splash::handlePlayerMovement() {
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

void Splash::spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position) {
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

void Splash::returnEnemyToPool(cocos2d::Node* enemy) {
    enemy->setVisible(false);
    enemy->stopAllActions();
    // Add the enemy back to the pool for reuse
    EnemyPool::getInstance()->returnEnemy(static_cast<Enemy*>(enemy));
}

void Splash::onEnemyOutOfBounds(cocos2d::Node* enemy) {
    auto enemyPos = enemy->getPosition();
    auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    if (enemyPos.x < 0 || enemyPos.x > screenSize.width || enemyPos.y < 0 || enemyPos.y > screenSize.height) {
        returnEnemyToPool(enemy);
    }
}

void Splash::scheduleEnemySpawning() {
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

void Splash::SpawnFallingRockAndBomb(cocos2d::Size size) {
    float restrictedWidth = SpriteController::calculateScreenRatio(Constants::PLAYER_RESTRICTEDWIDTH);
    float centerX = size.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    float minX = centerX - halfRestrictedWidth;
    float maxX = centerX + halfRestrictedWidth;

    int spawnOption = rand() % 3;
    Vec2 spawnPosition;
    switch (spawnOption) {
    case 0:
        spawnPosition = Vec2(centerX, size.height + 50);
        break;
    case 1:
        spawnPosition = Vec2(maxX - 10, size.height + 50);
        break;
    case 2:
        spawnPosition = Vec2(minX + 10, size.height + 50);
        break;
    }

    auto fallingRock = FallingRock::create();
    if (fallingRock) {
        fallingRock->spawn(spawnPosition);
        this->addChild(fallingRock);
    }
}

void Splash::SpawnFlyingBullet(cocos2d::Size size, bool directionLeft) {
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

        flyingBullet->runAction(sequence);
        this->addChild(flyingBullet);

    }
}


void Splash::SpawnRandomBoom(cocos2d::Size size) {
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
