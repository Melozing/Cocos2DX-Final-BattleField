#include "Game2/Game2Scene.h"
#include "Manager/PhysicsManager.h"
#include "Constants/Constants.h"
#include "Manager/BackgroundManager.h"

#include "Game2/Enemy/Enemyh/MeleeEnemy.h"
#include "Game2/Enemy/Enemyh/SniperEnemy.h"
#include "Game2/Enemy/Enemyh/InvEnemy.h"
#include "Game2/Enemy/Enemyh/SuicideBomberEnemy.h"
#include "Game2/Enemy/Enemyh/BossEnemy.h"
#include "Controller/GameController.h"
#include "Game2/Items/ItemsSpawn.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

const uint32_t PLAYER_BITMASK = 0x0001;
const uint32_t TILE_BITMASK = 0x0002;

cocos2d::Scene* Game2Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = Game2Scene::create();
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

    _isGameOver = false;
    PlayerAttributes::getInstance().SetHealth(Constants::Player_Health2);
    _playerAttributes = &PlayerAttributes::getInstance();

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
    _cursor = Cursor::create("assets_game/UXUI/Main_Menu/pointer.png");
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f)); // Adjust scale as needed
    this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR);

    this->schedule([this](float delta) {
        spawnEnemies();
        }, 5.0f, "spawn_enemy_key");

    CCLOG("Game2Scene initialized successfully");
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game2Scene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Initialize health bar
    initHealthBar();

    this->scheduleUpdate();

    return true;
}

void Game2Scene::initHealthBar() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    _healthBar = CustomLoadingBar::create("assets_game/UXUI/Loading/health_bar_g3_progress.png", "assets_game/UXUI/Loading/health_bar_g3_border.png", 0.25f);
    _healthBar->setLoadingBarRotation(-90);
    _healthBar->setLoadingBarPosition(Vec2(_healthBar->getLoadingBar()->getContentSize().height + SpriteController::calculateScreenRatio(0.03f) / 2, visibleSize.height / 2));

    // Adjust the border position to be lower than the loading bar
    auto loadingPos = _healthBar->getLoadingBar()->getPosition();
    float loadingBarHeight = SpriteController::calculateScreenRatio(0.01f);
    loadingPos.y -= loadingBarHeight; // Move the border lower
    _healthBar->setBorderPosition(loadingPos);

    _healthBar->setBorderRotation(-90);
    _healthBar->setPercent(100); // Set to full health
    _healthBar->setLoadingBarScale(SpriteController::updateSpriteScale(_healthBar->getLoadingBar(), 0.133f));
    _healthBar->setBorderScale(SpriteController::updateSpriteScale(_healthBar->getBorder(), 0.155f));

    this->addChild(_healthBar, Constants::ORDER_LAYER_UI);
}

void Game2Scene::updateHealthBar() {
    float healthPercent = (static_cast<float>(_playerAttributes->GetHealth()) / Constants::Player_Health2) * 100.0f;
    _healthBar->setPercent(healthPercent);
}

void Game2Scene::checkGameOver() {
    if (_playerAttributes->GetHealth() <= 0) {
        _isGameOver = true;
        auto gameOverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 48);
        gameOverLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
        this->addChild(gameOverLabel);

        GameController::getInstance()->GameOver(
            [this]() {
                Director::getInstance()->end();
            },
            []() -> Scene* {
                return Game2Scene::createScene();
            },
            Constants::pathSoundTrackGame1 // Add the soundtrack path here
        );
    }
}

void Game2Scene::resetGameState() {
    // Unschedule existing tasks
    this->unscheduleAllCallbacks();

    // Reset game state
    _isGameOver = false;
    PlayerAttributes::getInstance().SetHealth(Constants::Player_Health2);
    _playerAttributes = &PlayerAttributes::getInstance();

    this->scheduleUpdate();
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
        const auto visibleSize = Director::getInstance()->getVisibleSize();
        const Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 pos = _player->getPosition();

        pos.x = std::max(origin.x, std::min(pos.x, origin.x + visibleSize.width));
        pos.y = std::max(origin.y, std::min(pos.y, origin.y + visibleSize.height));

        _player->setPosition(pos);
    }
    // Update health bar
    updateHealthBar();

    // Check for game over
    checkGameOver();
}

void Game2Scene::spawnEnemies() {
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 spawnPoints[] = {
        Vec2(585, 330),
        Vec2(1520, 460),
        Vec2(1020, 900)
    };
    for (const auto& point : spawnPoints) {
        float x = point.x * (visibleSize.width / 1920.0f);
        float y = point.y * (visibleSize.height / 1080.0f);
        spawnEnemy("MeleeEnemy", Vec2(x + origin.x, y + origin.y));
    }
}

void Game2Scene::spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position) {
    EnemyBase* enemy = nullptr;

    if (enemyType == "MeleeEnemy") {
        enemy = MeleeEnemy::create();
    }
    /*else if (enemyType == "SniperEnemy") {
        enemy = SniperEnemy::create();
    }
    else if (enemyType == "InvEnemy") {
        enemy = InvEnemy::create();
    }
    else if (enemyType == "SuicideBomberEnemy") {
        enemy = SuicideBomberEnemy::create();
    }
    else if (enemyType == "BossEnemy") {
        enemy = BossEnemy::create();
    }*/

    if (enemy) {
        enemy->setName("Enemy");
        enemy->setPosition(position);
        this->addChild(enemy);
        enemy->scheduleUpdate();
    }
}

bool Game2Scene::onContactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (!nodeA || !nodeB) {
        return false;
    }

    // Handle collision between player and grenade
    if ((nodeA->getName() == "PlayerGame2" && nodeB->getName() == "Grenade") ||
        (nodeB->getName() == "PlayerGame2" && nodeA->getName() == "Grenade")) {
        if (nodeA->getName() == "Grenade") {
            nodeA->removeFromParent();
        }
        else {
            nodeB->removeFromParent();
        }
        _player->die();
    }

    // Handle collision between enemy and grenade
    if ((nodeA->getName() == "Enemy" && nodeB->getName() == "Grenade") ||
        (nodeB->getName() == "Enemy" && nodeA->getName() == "Grenade")) {
        if (nodeA->getName() == "Grenade") {
            nodeA->removeFromParent();
        }
        else {
            nodeB->removeFromParent();
        }
        if (nodeA->getName() == "Enemy") {
            auto enemy = dynamic_cast<MeleeEnemy*>(nodeA);
            if (enemy) {
                enemy->die();
            }
        }
        else {
            auto enemy = dynamic_cast<MeleeEnemy*>(nodeB);
            if (enemy) {
                enemy->die();
            }
        }
    }

    if ((nodeA->getName() == "Bullet" && nodeB->getName() == "Enemy") ||
        (nodeB->getName() == "Bullet" && nodeA->getName() == "Enemy")) {
        auto bullet = (nodeA->getName() == "Bullet") ? nodeA : nodeB;
        auto enemy = (nodeA->getName() == "Enemy") ? nodeA : nodeB;

        // Apply damage to the enemy
        auto enemyBase = dynamic_cast<EnemyBase*>(enemy);
        auto bulletGame2 = dynamic_cast<BulletGame2*>(bullet);
        if (enemyBase && bulletGame2) {
            enemyBase->takeDamage(bulletGame2->getDamage());
        }

        // Remove the bullet
        bullet->removeFromParent();
    }

    return true;
}

void Game2Scene::takeDamage(int damage) {
    _playerAttributes->SetHealth(_playerAttributes->GetHealth() - damage);
    updateHealthBar();
    checkGameOver();
}

void Game2Scene::heal(int amount) {
    _playerAttributes->SetHealth(std::min(_playerAttributes->GetHealth() + amount, Constants::Player_Health2));
    updateHealthBar();
}
