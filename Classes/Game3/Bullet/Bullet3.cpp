// Bullet.cpp
#include "Game3/Bullet/Bullet3.h"
#include "Constants/Constants.h"
USING_NS_CC;

Bullet3::Bullet3()
    : _direction(Vec2::ZERO),
    _speed(0.0f)
{
}

Bullet3::~Bullet3()
{
}

Bullet3* Bullet3::createBullet(const Vec2& direction, float speed)
{
    Bullet3* bullet = new (std::nothrow) Bullet3();
    if (bullet && bullet->init())
    {
        bullet->autorelease();
        bullet->setDirection(direction);
        bullet->setSpeed(speed);
        return bullet;
    }
    else
    {
        delete bullet;
        return nullptr;
    }
}

bool Bullet3::init()
{
    if (!Sprite::initWithFile("assets_game/bullet/1.png"))
    {
        CCLOG("Failed to load bullet image");
        return false;
    }

    // Add PhysicsBody to the bullet
    auto physicsBody = PhysicsBody::createCircle(this->getContentSize().width / 2);
    physicsBody->setContactTestBitmask(true);
    this->setPhysicsBody(physicsBody);

    this->setTag(Constants::BulletTag); // Set the tag for the bullet

    this->scheduleUpdate();
    return true;
}


void Bullet3::update(float delta)
{
    Vec2 position = this->getPosition();
    position += _direction * _speed * delta;
    this->setPosition(position);

    // Remove bullet if it goes out of the screen
    auto winSize = Director::getInstance()->getWinSize();
    if (position.x < 0 || position.x > winSize.width || position.y < 0 || position.y > winSize.height)
    {
        this->removeFromParentAndCleanup(true);
    }
}

void Bullet3::setDirection(const Vec2& direction)
{
    _direction = direction;
    _direction.normalize();
}

void Bullet3::setSpeed(float speed)
{
    _speed = speed;
}
