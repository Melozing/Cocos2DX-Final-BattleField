#include "Game1/Game1Scene.h"
#include "Button/PauseButton.h"
#include "Enemy/FlyingBullet.h"
#include "Enemy/FallingRock.h"
#include "Enemy/RandomBoom.h"
#include "Enemy/EnemyFactory.h"
#include "Enemy/EnemyPool.h"
#include "Game1/Items/HealthItem.h"
#include "Game1/Items/AmmoItem.h"
#include "Game1/Player/HealthPlayerGame1.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "Controller/GameController.h"
#include "ui/UILoadingBar.h"
#include <ctime> 

USING_NS_CC;

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

    _isGameOver = false;
    _playerAttributes = &PlayerAttributes::getInstance(); // Use singleton instance
    _canTakeDamage = true;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto edgeBody = cocos2d::PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 0);
    edgeBody->setCollisionBitmask(0x03);
    edgeBody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    addChild(edgeNode);

    EnemyPool::getInstance()->initPool("FlyingBullet", 10);
    EnemyPool::getInstance()->initPool("FallingRock", 5);
    EnemyPool::getInstance()->initPool("RandomBoom", 5);

    background = Background::createBackground("assets_game/gameplay/background.png", 150.0f);
    this->addChild(background);

    _player = PlayerGame1::createPlayer();
    _player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _healthPlayerGame1 = HealthPlayerGame1::createHealth();
    _healthPlayerGame1->initHealthSprites(_playerAttributes->GetHealth());
    this->addChild(_healthPlayerGame1);

    auto playerBody = PhysicsBody::createBox(_player->GetSize());
    setPhysicsBodyChar(playerBody, 0x01);
    _player->setPhysicsBody(playerBody);
    addChild(_player);

    _movingUp = _movingDown = _movingLeft = _movingRight = false;

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W: _movingUp = true; break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S: _movingDown = true; break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A: _movingLeft = true; break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D: _movingRight = true; break;
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            if (_pauseButton && !_pauseButton->isPaused()) { // Check if _pauseButton is not nullptr
                _pauseButton->pauseGame();
            }
            break;
        case EventKeyboard::KeyCode::KEY_ENTER:
            if (_pauseButton && _pauseButton->isPaused()) { // Check if _pauseButton is not nullptr
                _pauseButton->continueGame();
            }
            break;
        default: break;
        }
        };

    listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W: _movingUp = false; break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S: _movingDown = false; break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A: _movingLeft = false; break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D: _movingRight = false; break;
        default: break;
        }
        };

    auto _spriteLoading = Sprite::create("assets_game/UXUI/Loading/Load_Bar_Fg.png");
    auto _spriteLoadingBorder = Sprite::create("assets_game/UXUI/Loading/Load_Bar_Bg.png");

    _loadingBar = cocos2d::ui::LoadingBar::create("assets_game/UXUI/Loading/Load_Bar_Fg.png");
    _loadingBar->setDirection(cocos2d::ui::LoadingBar::Direction::LEFT); // Or RIGHT, depending on your needs
    _loadingBar->setRotation(-90);
    _loadingBar->setScale(SpriteController::updateSpriteScale(_spriteLoading, 0.25f));
    _loadingBar->setPercent(0);
    _loadingBar->setAnchorPoint(Vec2(0.5, 0.5)); // Anchor at the center
    _loadingBar->setPosition(Vec2(visibleSize.width - _loadingBar->getContentSize().height / 2, visibleSize.height / 2)); // Position at the center right of the screen

    // Create and position the border
    border = Sprite::create("assets_game/UXUI/Loading/Load_Bar_Bg.png");
    auto loadingPos = _loadingBar->getPosition();
    border->setPosition(loadingPos);
    border->setScale(SpriteController::updateSpriteScale(_spriteLoading, 0.25f));
    border->setRotation(-90);
    border->setAnchorPoint(Vec2(0.5, 0.5)); // Anchor at the bottom center
    this->addChild(border); // Place border behind the loading bar
    this->addChild(_loadingBar);

    // Schedule the update for the loading bar
    this->schedule([this](float dt) {
        updateLoadingBar(dt);
        }, "loading_bar_update_key");

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game1Scene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->schedule([this](float dt) { handlePlayerMovement(); }, "update_key_schedule");
    this->scheduleUpdate();
    this->scheduleEnemySpawning();
    this->scheduleCollectibleSpawning();

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
            _healthPlayerGame1->updateHealthSprites(PlayerAttributes::getInstance().GetHealth());
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
            }
        );
        _isGameOver = true;
    }
}

void Game1Scene::updateLoadingBar(float dt) {
    if (_isGameOver) return;

    // Update the loading bar percentage
    float currentPercent = _loadingBar->getPercent();
    float increment = (dt / Constants::TIME_TO_WIN) * 100.0f; // Constants::TIME_TO_WIN is the time required to win
    currentPercent += increment;

    if (currentPercent >= 100.0f) {
        currentPercent = 100.0f;
        GameController::getInstance()->Victory();
    }

    _loadingBar->setPercent(currentPercent);
}

void Game1Scene::update(float delta) {
    background->update(delta);
    for (auto enemy : _enemyPool) {
        onEnemyOutOfBounds(enemy);
    }
}

void Game1Scene::handlePlayerMovement() {
    if (_isGameOver) return;
    if (_movingUp) _player->moveUp();
    if (_movingDown) _player->moveDown();
    if (_movingLeft) _player->moveLeft();
    if (_movingRight) _player->moveRight();
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


void Game1Scene::SpawnFallingRockAndBomb(Size size) {
    Vec2 spawnPosition = getRandomSpawnPosition(size);

    if (!isPositionOccupied(spawnPosition)) {
        auto fallingRock = FallingRock::create();
        if (fallingRock) {
            fallingRock->spawn(spawnPosition);
            auto size = fallingRock->GetSize();
            auto fallingRockBody = PhysicsBody::createCircle(size.width / 2);
            setPhysicsBodyChar(fallingRockBody, 0x02);
            fallingRock->setPhysicsBody(fallingRockBody);
            this->addChild(fallingRock);
        }
    }
    else {
        CCLOG("Position occupied for FallingRock at (%f, %f)", spawnPosition.x, spawnPosition.y);
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
    Vec2 spawnPosition = getRandomSpawnPosition(size);

    auto randomBoom = RandomBoom::create();
    if (randomBoom) {
        randomBoom->spawn(spawnPosition);
        this->addChild(randomBoom);
    }
}

void Game1Scene::SpawnCollectibleItem(const Size& size) {
    Vec2 spawnPosition = getRandomSpawnPosition(size);
    if (!isPositionOccupied(spawnPosition)) {
        // Randomly choose between HealthItem and AmmoItem
        CollectibleItem* item = nullptr;
        if (rand() % 2 == 0) {
            item = HealthItem::create();
        }
        else {
            item = AmmoItem::create();
        }

        if (item) {
            item->spawn(spawnPosition);

            // Create and set the physics body as a box with reduced size
            Size reducedSize = Size(item->GetSize().width * 0.65, item->GetSize().height * 0.65); // Reduce size by 10%
            auto itemBody = PhysicsBody::createBox(reducedSize);
            setPhysicsBodyChar(itemBody, 0x03);
            item->setPhysicsBody(itemBody);

            this->addChild(item);
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


