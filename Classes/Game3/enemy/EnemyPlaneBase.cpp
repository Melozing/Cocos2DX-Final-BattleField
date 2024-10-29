#include "EnemyPlaneBase.h"

USING_NS_CC;

bool EnemyPlaneBase::init() {
    if (!Sprite::init()) {
        return false;
    }
    return true;
}

void EnemyPlaneBase::move(const Size& visibleSize, float speed) {
    float startX = -this->getContentSize().width / 2;
    float endX = visibleSize.width + this->getContentSize().width / 2;

    float distance = endX - startX;
    float moveDuration = distance / speed;

    auto moveRight = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));
    auto removeSelf = CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
        });

    auto sequence = Sequence::create(moveRight, removeSelf, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBase::moveRightToLeft(const Size& visibleSize, float speed) {
    float startX = visibleSize.width + this->getContentSize().width / 2;
    float endX = -this->getContentSize().width / 2;

    float distance = startX - endX;
    float moveDuration = distance / speed;

    if (this->modelCharac) {
        this->modelCharac->setFlippedX(true);
    }

    auto moveLeft = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));
    auto removeSelf = CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
        });

    auto sequence = Sequence::create(moveLeft, removeSelf, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBase::moveAndReturn(const Size& visibleSize, float speed) {
    float startXLeft = -this->getContentSize().width / 2;
    float endXRight = visibleSize.width + this->getContentSize().width / 2;

    float distanceLeftToRight = endXRight - startXLeft;
    float moveDurationLeftToRight = distanceLeftToRight / speed;

    auto moveRight = MoveTo::create(moveDurationLeftToRight, Vec2(endXRight, this->getPositionY()));
    auto flipRight = CallFunc::create([this]() {
        if (this->modelCharac) {
            this->modelCharac->setFlippedX(false);
        }
        });

    float startXRight = visibleSize.width + this->getContentSize().width / 2;
    float endXLeft = -this->getContentSize().width / 2;

    float distanceRightToLeft = startXRight - endXLeft;
    float moveDurationRightToLeft = distanceRightToLeft / speed;

    auto moveLeft = MoveTo::create(moveDurationRightToLeft, Vec2(endXLeft, this->getPositionY()));
    auto flipLeft = CallFunc::create([this]() {
        if (this->modelCharac) {
            this->modelCharac->setFlippedX(true);
        }
        });

    auto removeSelf = CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
        });

    auto sequence = Sequence::create(moveRight, flipRight, moveLeft, flipLeft, removeSelf, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBase::spawnEnemyAfterDelay(float delay, Node* parent, std::function<EnemyPlaneBase* ()> createEnemyFunc) {
    auto delayAction = DelayTime::create(delay);
    auto spawnAction = CallFunc::create([parent, createEnemyFunc]() {
        auto enemy = createEnemyFunc();
        if (enemy) {
            parent->addChild(enemy);
        }
        });

    auto sequence = Sequence::create(delayAction, spawnAction, nullptr);
    auto repeatAction = RepeatForever::create(sequence);
    parent->runAction(repeatAction);
}
