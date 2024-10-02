// Bullet.h
#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite
{
public:
    Bullet();
    virtual ~Bullet();

    static Bullet* createBullet(const cocos2d::Vec2& direction, float speed);

    virtual bool init() override;
    virtual void update(float delta) override;

    void setDirection(const cocos2d::Vec2& direction);
    void setSpeed(float speed);

private:
    cocos2d::Vec2 _direction;
    float _speed;
};

#endif // __BULLET_H__
