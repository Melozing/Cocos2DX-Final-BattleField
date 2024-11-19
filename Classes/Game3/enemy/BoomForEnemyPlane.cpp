#include "BoomForEnemyPlane.h"
#include "BoomForEnemyPlanePool.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"

USING_NS_CC;

BoomForEnemyPlane* BoomForEnemyPlane::createBoom() {
    BoomForEnemyPlane* boom = new (std::nothrow) BoomForEnemyPlane();
    if (boom && boom->init()) {
        boom->autorelease();
        return boom;
    }
    CC_SAFE_DELETE(boom);
    return nullptr;
}

bool BoomForEnemyPlane::init() {
    if (!Sprite::init()) {
        return false;
    }

    modelCharac = Sprite::create("assets_game/enemies/Boom.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    this->addChild(modelCharac);

    return true;
}

void BoomForEnemyPlane::reset() {
    this->setVisible(true);
    this->setRotation(0);
}
void BoomForEnemyPlane::moveDown(bool spawnFromLeft) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float moveDuration = 4.0f;
    float rotateDuration = 2.0f;
    Vec2 moveBy = Vec2(0, -visibleSize.height);

    // Adjust the movement to be diagonal based on the spawn direction
    float horizontalShift = visibleSize.width / 10; // Reduce the horizontal shift

    float rotationAngle = 15.0f; // Define the rotation angle

    if (!spawnFromLeft) {
        moveBy.x = horizontalShift; // Move to the right
        rotationAngle = -rotationAngle; // Rotate to the left
    }
    else {
        moveBy.x = -horizontalShift; // Move to the left
        rotationAngle = rotationAngle; // Rotate to the right
    }

    auto moveAction = MoveBy::create(moveDuration, moveBy);
    auto rotateAction = RotateBy::create(rotateDuration, rotationAngle);
    auto spawnAction = Spawn::createWithTwoActions(moveAction, rotateAction);
    auto sequence = Sequence::create(spawnAction, CallFunc::create([this]() {
        this->returnToPool();
        }), nullptr);
    this->runAction(sequence);
}



void BoomForEnemyPlane::returnToPool() {
    this->stopAllActions();
    this->removeFromParent();
    this->setVisible(false);
    BoomForEnemyPlanePool::getInstance()->returnBoom(this);
}
