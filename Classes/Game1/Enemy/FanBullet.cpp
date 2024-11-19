#include "FanBullet.h"
#include "Controller/SpriteController.h"
#include "FanBulletPool.h"
#include "Constants/Constants.h"
#include "cocos2d.h"

USING_NS_CC;

FanBullet* FanBullet::create() {
    FanBullet* ret = new (std::nothrow) FanBullet();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void FanBullet::reset() {
    this->setVisible(false);
    _direction = Vec2::ZERO; // Reset direction
}


bool FanBullet::init() {
    if (!Sprite::init()) {
        return false;
    }

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/flying_bullet.plist");

    this->scheduleUpdate(); // Schedule the update method
    return true;
}

void FanBullet::initAnimation() {
    std::string spriteFrameName = "flying_bullet1.png";
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/flying_bullet.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName(spriteFrameName);
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.05f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(createAnimation("flying_bullet", 4, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void FanBullet::spawn(const cocos2d::Vec2& startPosition, float angle) {
    this->setPosition(startPosition);
    this->setVisible(true);
    initAnimation();

    // Calculate the movement vector based on the angle
    float radians = CC_DEGREES_TO_RADIANS(angle);
    _direction = Vec2(cos(radians), sin(radians));

    this->setRotation(-angle);

    // Move the bullet continuously in the specified direction
    auto moveForever = RepeatForever::create(MoveBy::create(1.0f, _direction * _speed));
    this->runAction(moveForever);
}

void FanBullet::update(float delta) {
    removeWhenOutOfScreen();
}

void FanBullet::removeWhenOutOfScreen() {
    Vec2 currentPosition = this->getPosition();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Adjust the bounds to account for the offset
    float offset = SpriteController::calculateScreenRatio(Constants::FANBULLET_OFFSET);
    // Check if the bullet is out of the screen bounds
    if (currentPosition.x < origin.x - this->getContentSize().width - offset ||
        currentPosition.x > origin.x + visibleSize.width + this->getContentSize().width + offset ||
        currentPosition.y < origin.y - this->getContentSize().height - offset ||
        currentPosition.y > origin.y + visibleSize.height + this->getContentSize().height + offset) {
        this->stopAllActions();
        this->removeFromParentAndCleanup(false);
        FanBulletPool::getInstance()->returnEnemy(this); // Return to pool
    }
}

cocos2d::Size FanBullet::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

FanBullet::~FanBullet() {
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("assets_game/enemies/flying_bullet.plist");
}