#pragma once

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class Bullet : public cocos2d::Sprite, public SpriteController
{
public:
    static Bullet* create();
    bool init() override;

    void setDirection(const cocos2d::Vec2& direction);
    void setSpeed(float speed);
    Size GetSize();
    void reset();
    void spawn(const cocos2d::Vec2& startPosition, float angle);
    void startMovement();
    void removeWhenOutOfScreen();
    void returnPool();

private:
    cocos2d::Vec2 _direction;
    float _speed;
    cocos2d::Sprite* modelCharac;
};
