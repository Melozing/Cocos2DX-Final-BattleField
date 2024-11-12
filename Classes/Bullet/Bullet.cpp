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

    this->scheduleUpdate();
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
    _active = true; // Set active to true
    this->setVisible(true);
    this->stopAllActions();
}

void Bullet::update(float delta)
{
    if (!_active) return;

    // Move the bullet
    Vec2 currentPosition = this->getPosition();
    Vec2 movement = _direction * _speed * delta;
    this->setPosition(currentPosition + movement);

    // Check if the bullet is off-screen
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    Rect visibleRect(origin.x, origin.y, visibleSize.width, visibleSize.height);

    if (!visibleRect.containsPoint(this->getPosition()))
    {
        BulletPool::getInstance()->returnBullet(this);
        this->removeFromParentAndCleanup(false);
    }
}
