// Bullet.cpp
#include "Bullet/Bullet.h"
#include "Constants/Constants.h"
USING_NS_CC;

Bullet::Bullet()
    : _direction(Vec2::ZERO),
    _speed(0.0f),
    _active(false)
{
}

Bullet::~Bullet()
{
}

Bullet* Bullet::createBullet(const std::string& image, const Vec2& direction, float speed)
{
    CCLOG("createBullet called");

    Bullet* bullet = new (std::nothrow) Bullet();
    if (bullet && bullet->initWithProperties(image, direction, speed))
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

bool Bullet::initWithProperties(const std::string& image, const Vec2& direction, float speed)
{
    if (!Sprite::initWithFile(image))
    {
        CCLOG("Failed to load bullet image: %s", image.c_str());
        return false;
    }

    // Add PhysicsBody to the bullet
    auto physicsBody = PhysicsBody::createCircle(this->getContentSize().width / 2);
    physicsBody->setContactTestBitmask(true);
    this->setPhysicsBody(physicsBody);

    this->setTag(Constants::BulletTag); // Set the tag for the bullet

    setDirection(direction);
    setSpeed(speed);

    this->scheduleUpdate();
    return true;
}

void Bullet::update(float delta)
{
    if (!_active) return;

    Vec2 position = this->getPosition();
    position += _direction * _speed * delta;
    this->setPosition(position);

    // Remove bullet if it goes out of the screen
    auto winSize = Director::getInstance()->getWinSize();
    if (position.x < 0 || position.x > winSize.width || position.y < 0 || position.y > winSize.height)
    {
        this->deactivate();
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

void Bullet::activate()
{
    _active = true;
    this->setVisible(true);
}

void Bullet::deactivate()
{
    _active = false;
    this->setVisible(false);
}

bool Bullet::isActive() const
{
    return _active;
}
