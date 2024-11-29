﻿#include "EnemyPlaneBoom.h"
#include "EnemyPlaneBoomPool.h"
#include "Constants/Constants.h"
#include "BoomForEnemyPlanePool.h"

USING_NS_CC;

EnemyPlaneBoom* EnemyPlaneBoom::createEnemyPlaneBoom() {
    EnemyPlaneBoom* enemy = new (std::nothrow) EnemyPlaneBoom();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlaneBoom::init() {
    if (!EnemyPlaneBase::init()) {
        return false;
    }

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/enemy_plane_boom.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");

    initAnimation();

    return true;
}

Size EnemyPlaneBoom::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void EnemyPlaneBoom::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/enemy_plane_boom.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName("Plane_enemy_bom1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("Plane_enemy_bom", 8, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBoom::spawnEnemy(cocos2d::Node* parent, float skillTime) {
    auto enemy = EnemyPlaneBoomPool::getInstance()->getEnemy();
    if (enemy) {
        enemy->setVisible(true);
        enemy->resetSprite();
        parent->addChild(enemy);
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // Define the range to spawn near the top of the screen
        float topBuffer = visibleSize.height / 6; // Smaller range
        float randomY = random(visibleSize.height - topBuffer, visibleSize.height - enemy->getContentSize().height / 2);

        bool spawnFromLeft = random(0, 1) == 0;

        if (spawnFromLeft) {
            enemy->setPosition(Vec2(-enemy->getContentSize().width / 2, randomY));
            enemy->moveFromLeftToRight(visibleSize, Constants::EnemyPlaneBoomGame3Speed);
        }
        else {
            enemy->setPosition(Vec2(visibleSize.width + enemy->getContentSize().width / 2, randomY));
            enemy->moveFromRightToLeft(visibleSize, Constants::EnemyPlaneBoomGame3Speed);
        }

        // Create physics body when spawning
        enemy->createPhysicsBody();

        // Schedule to spawn boom at the specified skill time
        enemy->schedule([enemy, spawnFromLeft](float dt) {
            enemy->spawnBoom(spawnFromLeft);
            }, skillTime, "spawn_boom_key");
    }
}

void EnemyPlaneBoom::spawnBoom(bool spawnFromLeft) {
    auto boom = BoomForEnemyPlanePool::getInstance()->getBoom();
    if (boom) {
        boom->setPosition(this->getPosition());
        if (boom->getParent() == nullptr) {
            this->getParent()->addChild(boom);
        }
        boom->setVisible(true);
        boom->moveDown(spawnFromLeft);
    }
}

void EnemyPlaneBoom::reset() {
    // Reset the state of the EnemyPlaneBoom
    this->stopAllActions();
    this->setVisible(false);
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
    this->setPosition(Vec2::ZERO);
}

void EnemyPlaneBoom::explode() {
    this->removeComponent(this->getPhysicsBody());
    modelCharac->setVisible(false);
    auto explosion = Explosion::create(this->getPosition(), [this]() {
        this->returnToPool();
        });
    this->getParent()->addChild(explosion);
}

void EnemyPlaneBoom::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsBody = PhysicsBody::createBox(this->GetSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    this->addComponent(physicsBody);
}
