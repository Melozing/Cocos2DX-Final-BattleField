#include "Game2/Game2Scene.h"
#include "Manager/PhysicsManager.h"
#include "Manager/ObjectPoolGame2.h"
#include "Constants/Constants.h"
#include "Manager/BackgroundManager.h"
#include "Game2/Player/Cannon.h"
#include "Game2/Enemy/BunkerGame2.h"
#include "Controller/GameController.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

cocos2d::Scene* Game2Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game2Scene::create();
    scene->addChild(layer);
    return scene;
}

bool Game2Scene::init() {
    if (!BaseScene::init()) {
        return false;
    }

    this->setSceneCreationFunc([]() -> cocos2d::Scene* {
        return Game2Scene::createScene();
        });
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
    BackgroundManager::getInstance()->setBackground(this, "assets_game/gameplay/game2/bg_game_2.png", Constants::ORDER_LAYER_BACKGROUND);

    preloadAssets();
    setupContactListener();
    setupBackground();
    createGround();
    setupCannon();
    setupPlayer();
    setupCursor();
    setupKeyboardEventListeners();
    initHealthBar();
    initPools();
    setupBunkers();

    this->scheduleUpdate();
    return true;
}

void Game2Scene::setupContactListener() {
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Game2Scene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

}

void Game2Scene::preloadAssets() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/BulletPlayer3Game.plist");

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/SniperEnemyDeath.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/SniperEnemyRun.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/SniperEnemyShoot.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/SuicideBomberEnemy.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/effects/explosion.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/PlayerGame2Hands.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/PlayerGame2Body.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/CannonGame2_.plist");
};

void Game2Scene::initHealthBar() {
    // Implement health bar initialization
}

void Game2Scene::initPools() {
    BunkerGame2Pool::getInstance()->resetPool();
    MeleeEnemyGame2Pool::getInstance()->resetPool();
    SniperEnemyGame2Pool::getInstance()->resetPool();
    SuicideBomberEnemyPool::getInstance()->resetPool();
    BulletGame2Pool::getInstance()->resetPool();
    SniperBulletGame2Pool::getInstance()->resetPool();

    BunkerGame2Pool::getInstance()->initPool(10);
    MeleeEnemyGame2Pool::getInstance()->initPool(10);
    SniperEnemyGame2Pool::getInstance()->initPool(10);
    SuicideBomberEnemyPool::getInstance()->initPool(10);
    BulletGame2Pool::getInstance()->initPool(10);
    SuicideBomberEnemyPool::getInstance()->initPool(10);
    SniperBulletGame2Pool::getInstance()->initPool(10);
}

void Game2Scene::setupBackground() {
    BackgroundManager::getInstance()->setBackground(this, "assets_game/gameplay/game2/bg_game_2.png", Constants::ORDER_LAYER_BACKGROUND);
}

bool Game2Scene::onContactBegin(cocos2d::PhysicsContact& contact) {
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    auto nodeA = bodyA->getNode();
    auto nodeB = bodyB->getNode();

    if (nodeA && nodeB) {
        auto suicideBomberEnemyA = dynamic_cast<SuicideBomberEnemy*>(nodeA);
        auto sniperEnemyA = dynamic_cast<SniperEnemyGame2*>(nodeA);
        auto playerNodeA = dynamic_cast<PlayerGame2*>(nodeA);
        auto playerBulletA = dynamic_cast<BulletGame2*>(nodeA);
        auto cannonA = dynamic_cast<Cannon*>(nodeA);
        auto groundCannonA = dynamic_cast<GroundCannonGame2*>(nodeA);

        auto suicideBomberEnemyB = dynamic_cast<SuicideBomberEnemy*>(nodeB);
        auto sniperEnemyB = dynamic_cast<SniperEnemyGame2*>(nodeB);
        auto playerNodeB = dynamic_cast<PlayerGame2*>(nodeB);
        auto playerBulletB = dynamic_cast<BulletGame2*>(nodeB);
        auto cannonB = dynamic_cast<Cannon*>(nodeB);
        auto groundCannonB = dynamic_cast<GroundCannonGame2*>(nodeB);

        if ((suicideBomberEnemyA && groundCannonB) || (suicideBomberEnemyB && groundCannonA)) {
            handleSuicideBomberEnemyCollision(suicideBomberEnemyA ? suicideBomberEnemyA : suicideBomberEnemyB, cannonA ? cannonA : cannonB);
            return true;
        }
        if ((sniperEnemyA && playerBulletB) || (sniperEnemyB && playerBulletA)) {
            handleSniperEnemyAndBulletPlayerCollision(sniperEnemyA ? sniperEnemyA : sniperEnemyB, playerBulletA ? playerBulletA : playerBulletB);
            return true;
        }
        if ((sniperEnemyA && groundCannonB) || (sniperEnemyB && groundCannonA)) {
            auto sniperEnemy = sniperEnemyA ? sniperEnemyA : sniperEnemyB;
            sniperEnemy->setPosition(sniperEnemy->getPosition() - contact.getContactData()->normal * 5);
            return true;
        }
        if ((playerNodeA && groundCannonB) || (playerNodeB && groundCannonA)) {
            auto playerNode = playerNodeA ? playerNodeA : playerNodeB;
            playerNode->setPosition(playerNode->getPosition() - contact.getContactData()->normal * 3);
            return true;
        }
    }

    return false;
}


void Game2Scene::handleSuicideBomberEnemyCollision(SuicideBomberEnemy* enemy, Cannon* cannon) {
    enemy->explode();
    enemy->returnToPool();
}

void Game2Scene::handleSniperEnemyAndBulletPlayerCollision(SniperEnemyGame2* enemy, BulletGame2* bullet) {
    enemy->takeDamage(-1);
    bullet->returnPool();
}

void Game2Scene::createGround() {
    groundSprite = GroundCannonGame2::createGround();
    if (groundSprite) {
        const auto visibleSize = Director::getInstance()->getVisibleSize();
        groundSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(groundSprite, Constants::ORDER_LAYER_BACKGROUND);
    }
}

void Game2Scene::setupCannon() {
    _cannon = Cannon::create();
    if (!_cannon) {
        CCLOG("Failed to create Cannon");
        return;
    }
    this->addChild(_cannon);
    _cannon->setPosition(Vec2(groundSprite->getPositionX(), groundSprite->getPositionY() - groundSprite->getContentSize().height / 2 + _cannon->getContentSize().height / 2)); // Position cannon on the ground
}

void Game2Scene::setupBunkers() {
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float padding = SpriteController::calculateScreenRatio(0.07f);

    Vec2 positions[4] = {
        Vec2(origin.x + padding, origin.y + padding),
        Vec2(origin.x + visibleSize.width - padding, origin.y + padding),
        Vec2(origin.x + padding, origin.y + visibleSize.height - padding),
        Vec2(origin.x + visibleSize.width - padding, origin.y + visibleSize.height - padding)
    };

    for (const auto& pos : positions) {
        auto bunker = BunkerGame2Pool::getInstance()->getObject();
        bunker->setPosition(pos);
        bunker->setTargetCannon(_cannon);
        bunker->setPlayer(_player);
        this->addChild(bunker);
    }
}

void Game2Scene::setupPlayer() {
    PlayerAttributes::getInstance().SetHealth(Constants::Player_Health2);
    _playerAttributes = &PlayerAttributes::getInstance();
    _player = PlayerGame2::createPlayerGame2();
    if (!_player) {
        CCLOG("Failed to create PlayerGame2");
        return;
    }
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    _player->setName("PlayerGame2");
    this->addChild(_player);
}

void Game2Scene::checkGameOver() {
    /* if (_playerAttributes->GetHealth() <= 0) {
         _isGameOver = true;
         auto gameOverLabel = Label::createWithTTF("Game Over", "", 48);
         gameOverLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
         this->addChild(gameOverLabel);

         GameController::getInstance()->GameOver(
             [this]() {
                 Director::getInstance()->end();
             },
             []() -> Scene* {
                 return Game2Scene::createScene();
             },
             Constants::pathSoundTrackGame1
         );
     }*/
}

void Game2Scene::setupCursor() {
    _cursor = Cursor::create("assets_game/textures/Cursor/Cursor.png");
    _cursor->setScale(SpriteController::updateSpriteScale(_cursor, 0.03f));
    this->addChild(_cursor, Constants::ORDER_LAYER_CURSOR);
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
    checkGameOver();
}