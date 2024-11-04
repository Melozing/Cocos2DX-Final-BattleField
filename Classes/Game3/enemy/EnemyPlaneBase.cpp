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
    return true;
}

void EnemyPlaneBase::moveFromLeftToRight(const Size& visibleSize, float speed) {

    float startX = -this->getContentSize().width / 2;
    float endX = visibleSize.width + this->getContentSize().width / 2;

    float distance = endX - startX;
    float moveDuration = distance / speed;

    auto moveRight = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));
    auto returnToPool = CallFunc::create([this]() {
        this->returnToPool();
        });

    auto sequence = Sequence::create(moveRight, returnToPool, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBase::moveFromRightToLeft(const Size& visibleSize, float speed) {
    float startX = visibleSize.width + this->getContentSize().width / 2;
    float endX = -this->getContentSize().width / 2;

    float distance = startX - endX;
    float moveDuration = distance / speed;

    if (this->modelCharac) {
        this->modelCharac->setFlippedX(true);
    }

    auto moveLeft = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));
    auto returnToPool = CallFunc::create([this]() {
        this->returnToPool();
        });

    auto sequence = Sequence::create(moveLeft, returnToPool, nullptr);
    this->runAction(sequence);
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