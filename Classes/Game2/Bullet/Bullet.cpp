// Bullet.cpp
#include "Game2/Bullet/Bullet.h"

USING_NS_CC;

Bullet::Bullet()
    : _direction(Vec2::ZERO),
    _speed(0.0f)
{
}

Bullet::~Bullet()
{
}

Bullet* Bullet::createBullet(const Vec2& direction, float speed)
{
    Bullet* bullet = new (std::nothrow) Bullet();
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

bool Bullet::init()
{
    if (!Sprite::initWithFile("assets_game/player/shot.png"))
    {
        CCLOG("Failed to load bullet image");
        return false;
    }

    this->scheduleUpdate();
    return true;
}

void Bullet::update(float delta)
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

void Bullet::setDirection(const Vec2& direction)
{
    _direction = direction;
    _direction.normalize();
}

void Bullet::setSpeed(float speed)
{
    _speed = speed;
}
