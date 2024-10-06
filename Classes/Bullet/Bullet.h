// Bullet.h
#pragma once

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite
{
public:
    Bullet();
    ~Bullet();

    static Bullet* createBullet(const std::string& image, const cocos2d::Vec2& direction, float speed);
    bool initWithProperties(const std::string& image, const cocos2d::Vec2& direction, float speed);

    void update(float delta) override;
    void setDirection(const cocos2d::Vec2& direction);
    void setSpeed(float speed);

    void activate();
    void deactivate();
    bool isActive() const;

private:
    cocos2d::Vec2 _direction;
    float _speed;
    bool _active;
};

