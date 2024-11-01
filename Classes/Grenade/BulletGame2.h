// BulletGame2.h
#pragma once

#include "cocos2d.h"

class BulletGame2 : public cocos2d::Sprite
{
public:
    BulletGame2();
    virtual ~BulletGame2();

    static BulletGame2* createBullet(const cocos2d::Vec2& position, const cocos2d::Vec2& direction, float speed, int damage);
    virtual bool init();
    void update(float delta);
    int getDamage() const;
    void setDirection(const cocos2d::Vec2& direction);
    void setSpeed(float speed);
    virtual void onExit() override;
private:
    cocos2d::Vec2 _direction;
    float _speed;
    int _damage;
};
