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

    setDirection(direction);
    setSpeed(speed);

    // Set the rotation of the bullet sprite to match the direction
    float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));
    this->setRotation(-angle + 90);

    return true;
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
    this->stopAllActions(); // Stop any ongoing actions
}

bool Bullet::isActive() const
{
    return _active;
}

void Bullet::reset()
{
    _direction = Vec2::ZERO;
    _speed = 0.0f;
    _active = false;
    this->setVisible(false);
    this->stopAllActions(); // Stop any ongoing actions
}

void Bullet::moveIndefinitely()
{
    // Calculate the target position far outside the screen bounds
    Vec2 targetPosition = this->getPosition() + _direction * 10000;

    // Create a move action
    auto moveAction = MoveTo::create(10000 / _speed, targetPosition);

    // Create a callback to check if the bullet is off-screen
    auto checkOffScreen = CallFunc::create([this]() {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();
        Rect visibleRect(origin.x, origin.y, visibleSize.width, visibleSize.height);

        if (!visibleRect.containsPoint(this->getPosition()))
        {
            this->deactivate();
        }
        });

    // Run the actions sequentially with a repeat to keep checking off-screen status
    this->runAction(RepeatForever::create(Sequence::create(moveAction, checkOffScreen, nullptr)));
}
