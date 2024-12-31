#include "SniperEnemyGame2.h"
#include "Constants/Constants.h"
#include "Manager/ObjectPoolGame2.h"
#include "utils/PhysicsShapeCache.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "FX/Explodable.h"

USING_NS_CC;

SniperEnemyGame2* SniperEnemyGame2::create()
{
    SniperEnemyGame2* enemy = new (std::nothrow) SniperEnemyGame2();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool SniperEnemyGame2::init()
{
    if (!EnemyBase::init()) {
        return false;
    }

    initAnimation();
    createPhysicsBody();

    currentState = State::Moving;
    stateTime = 0.0f;
    moveDuration = 1.0f; 
    shootDuration = 1.5f; 
    hasShotBullet = false;
    health = maxHealth;
    return true;
}

void SniperEnemyGame2::initAnimation() {
    runSprite = Sprite::createWithSpriteFrameName("SniperEnemyRun1.png");
    runSprite->setScale(SpriteController::updateSpriteScale(runSprite, 0.07f));
    this->addChild(runSprite);

    auto runAnimate = Animate::create(SpriteController::createAnimation("SniperEnemyRun", 6, 0.07f));
    runSprite->runAction(RepeatForever::create(runAnimate));

    shootSprite = Sprite::createWithSpriteFrameName("SniperEnemyShoot1.png");
    shootSprite->setScale(SpriteController::updateSpriteScale(shootSprite, 0.07f));
    this->addChild(shootSprite);

    auto shootAnimate = Animate::create(SpriteController::createAnimation("SniperEnemyShoot", 4, 0.1f));
    shootSprite->runAction(RepeatForever::create(shootAnimate));

    shootSprite->setVisible(false);

    deathSprite = Sprite::createWithSpriteFrameName("SniperEnemyDeath1.png");
    deathSprite->setScale(SpriteController::updateSpriteScale(deathSprite, 0.07f));
    this->addChild(deathSprite);

    auto deathAnimate = Animate::create(SpriteController::createAnimation("SniperEnemyDeath", 5, 0.1f));
    deathSprite->runAction(Sequence::create(deathAnimate, CallFunc::create([this]() {
        this->removeFromParent();
        SniperEnemyGame2Pool::getInstance()->returnObject(this);
        }), nullptr));

    deathSprite->setVisible(false);
}

void SniperEnemyGame2::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/SniperEnemyGame2.plist");

    auto originalSize = runSprite->getTexture()->getContentSize();
    auto scaledSize = runSprite->getContentSize() * runSprite->getScale();

    auto physicsBody = physicsCache->createBody("SniperEnemyGame2", originalSize, scaledSize);
    if (physicsBody) {
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);
        physicsBody->setContactTestBitmask(true);
        physicsBody->setCollisionBitmask(0x02);
        this->setPhysicsBody(physicsBody);
    }
}

void SniperEnemyGame2::reset()
{
    currentState = State::Moving;
    stateTime = 0.0f;
    runSprite->setVisible(true);
    shootSprite->setVisible(false);
    hasShotBullet = false;
    health = maxHealth;
}

void SniperEnemyGame2::setTarget(PlayerGame2* target) {
    this->targetPlayer = target;
    this->createPhysicsBody();
    setInitialDirection();
}

void SniperEnemyGame2::setInitialDirection() {
    if (targetPlayer) {
        Vec2 currentPosition = this->getPosition();
        Vec2 targetPosition = targetPlayer->getPosition();
        Vec2 direction = targetPosition - currentPosition;

        flipSpriteBasedOnDirection(direction);
    }
}

void SniperEnemyGame2::update(float delta) {
    stateTime += delta;

    switch (currentState) {
    case State::Moving:
        if (stateTime >= moveDuration) {
            stateTime = 0.0f;
            currentState = State::Shooting;
            switchToShootAnimation();
            hasShotBullet = false;
        }
        else {
            if (targetPlayer) {
                Vec2 currentPosition = this->getPosition();
                Vec2 targetPosition = targetPlayer->getPosition();
                Vec2 direction = targetPosition - currentPosition;
                direction.normalize();
                float speed = 100.0f;
                Vec2 newPosition = currentPosition + direction * speed * delta;
                this->setPosition(newPosition);

                flipSpriteBasedOnDirection(direction);
            }
        }
        break;
    case State::Shooting:
        if (stateTime >= shootDuration) {
            stateTime = 0.0f;
            currentState = State::Moving;
            switchToRunAnimation();
        }
        else if (stateTime >= shootDuration / 2 && !hasShotBullet) { 
            shootBullet();
            hasShotBullet = true;
        }
        break;
    }
}


void SniperEnemyGame2::flipSpriteBasedOnDirection(const Vec2& direction) {
    if (direction.x < 0) {
        runSprite->setFlippedX(true);
        shootSprite->setFlippedX(true);
    }
    else {
        runSprite->setFlippedX(false);
        shootSprite->setFlippedX(false);
    }
}

void SniperEnemyGame2::switchToShootAnimation()
{
    runSprite->setVisible(false);
    shootSprite->setVisible(true);
}

void SniperEnemyGame2::switchToRunAnimation()
{
    runSprite->setVisible(true);
    shootSprite->setVisible(false);
}

void SniperEnemyGame2::switchToDeathAnimation() {
    runSprite->setVisible(false);
    shootSprite->setVisible(false);
    deathSprite->setVisible(true);

    auto deathAnimate = Animate::create(SpriteController::createAnimation("SniperEnemyDeath", 5, 0.1f));
    auto sequence = Sequence::create(deathAnimate, CallFunc::create([this]() {
        this->removeFromParent();
        this->returnToPool();
        }), nullptr);

    deathSprite->runAction(sequence);
}


void SniperEnemyGame2::shootBullet()
{
    bullet = SniperBulletGame2Pool::getInstance()->getObject();

    bullet->setPosition(this->getPosition());
    bullet->setDirection(targetPlayer->getPosition() - this->getPosition());
    bullet->spawn();

    if (bullet->getParent() != nullptr) {
        bullet->removeFromParent();
    }

    this->getParent()->addChild(bullet);
}

void SniperEnemyGame2::takeDamage(float damage) {
    if (currentState == State::Dead) {
        return;
    }

    health -= damage;
    blinkRed();

    if (health <= 0) {
        currentState = State::Dead;
        switchToDeathAnimation();
    }
}

void SniperEnemyGame2::blinkRed() {
    auto tintToRed = TintTo::create(0.05f, 255, 0, 0);
    auto tintToNormal = TintTo::create(0.05f, 255, 255, 255);
    auto sequence = Sequence::create(tintToRed, tintToNormal, nullptr);
    runSprite->runAction(sequence);
    shootSprite->runAction(sequence->clone());
}