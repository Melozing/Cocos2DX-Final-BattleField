#include "Bullet/Bullet.h"
#include "BulletPool.h"
#include "Constants/Constants.h"
USING_NS_CC;

Bullet* Bullet::create()
{
    Bullet* bullet = new (std::nothrow) Bullet();

    if (bullet && bullet->init())
    {
        bullet->autorelease();
        return bullet;
    }
    else
    {
        delete bullet;
        return nullptr;
    }
}

bool Bullet::init()
{
    if (!Sprite::init()) {
        return false;
    }

    modelCharac = Sprite::create("assets_game/player/1.png");
    this->addChild(modelCharac);

    // Add physics body
    auto physicsBody = PhysicsBody::createBox(this->GetSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    this->addComponent(physicsBody);
    this->setSpeed(Constants::BulletGame3Speed * 0.6f);

    return true;
}

Size Bullet::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void Bullet::setDirection(const Vec2& direction)
{
    _direction = direction;
    _direction.normalize();

    // Set the rotation of the bullet sprite to match the direction
    float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));
    this->setRotation(-angle + 90);
}

void Bullet::setSpeed(float speed)
{
    _speed = speed;
}

void Bullet::reset()
{
    this->setVisible(true);
    this->stopAllActions();
}

void Bullet::spawn(const Vec2& startPosition, float angle)
{
    this->setPosition(startPosition);
    this->setRotation(angle);
    this->startMovement();
}

void Bullet::startMovement()
{
    // Calculate the target position based on direction and speed
    Vec2 targetPosition = this->getPosition() + _direction * _speed * 5; // Adjust the multiplier as needed

    // Create a MoveTo action
    auto moveAction = MoveTo::create(3.0f, targetPosition); // Adjust the duration as needed

    // Create a callback to remove the bullet when the action is done
    auto removeCallback = CallFunc::create([this]() {
        this->returnPool();
        });

    // Run the actions in sequence
    this->runAction(Sequence::create(moveAction, removeCallback, nullptr));
}

void Bullet::removeWhenOutOfScreen()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    Vec2 position = this->getPosition();

    if (position.x < origin.x || position.x > origin.x + visibleSize.width ||
        position.y < origin.y || position.y > origin.y + visibleSize.height)
    {
        this->returnPool();
    }
}

void Bullet::returnPool() {
    this->removeFromParentAndCleanup(false);
    BulletPool::getInstance()->returnBullet(this);
}
