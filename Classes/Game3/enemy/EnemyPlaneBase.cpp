#include "EnemyPlaneBase.h"
#include "EnemyPlaneBoom.h"
#include "EnemyPlaneBoomPool.h"
#include "EnemyPlaneBullet.h"
#include "EnemyPlaneBulletPool.h"

USING_NS_CC;

bool EnemyPlaneBase::init() {
    if (!Sprite::init()) {
        return false;
    }
    this->scheduleUpdate(); // Schedule the update function
    return true;
}

void EnemyPlaneBase::moveFromLeftToRight(const Size& visibleSize, float speed) {
    float startX = -this->getContentSize().width / 2;
    float endX = visibleSize.width + SpriteController::calculateScreenRatio(0.15f);

    float distance = endX - startX;
    float moveDuration = distance / speed;

    auto moveRight = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));
    auto sequence = Sequence::create(moveRight, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBase::moveFromRightToLeft(const Size& visibleSize, float speed) {
    float startX = visibleSize.width + this->getContentSize().width / 2;
    float endX = -this->getContentSize().width / 2 - SpriteController::calculateScreenRatio(0.2f);

    float distance = startX - endX;
    float moveDuration = distance / speed;

    if (this->modelCharac) {
        this->modelCharac->setFlippedX(true);
    }

    auto moveLeft = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));
    auto sequence = Sequence::create(moveLeft, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBase::update(float delta) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto position = this->getPosition();

    if (position.x < -this->getContentSize().width / 2 || position.x > visibleSize.width + this->getContentSize().width / 2) {
        this->returnToPool();
    }
}

void EnemyPlaneBase::returnToPool() {
    this->stopAllActions();
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);

    if (dynamic_cast<EnemyPlaneBullet*>(this)) {
        EnemyPlaneBulletPool::getInstance()->returnEnemy(static_cast<EnemyPlaneBullet*>(this));
    }
    else if (dynamic_cast<EnemyPlaneBoom*>(this)) {
        EnemyPlaneBoomPool::getInstance()->returnEnemy(static_cast<EnemyPlaneBoom*>(this));
    }
}

void EnemyPlaneBase::resetSprite() {
    if (this->modelCharac) {
        this->modelCharac->setFlippedX(false);
    }
    this->stopAllActions();
    this->setVisible(true);
}
