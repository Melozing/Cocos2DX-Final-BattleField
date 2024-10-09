// PlayerMovement.h
#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "cocos2d.h"

class PlayerMovement : public cocos2d::Sprite
{
public:
    PlayerMovement();
    virtual ~PlayerMovement();

    virtual bool init() override;
    virtual void update(float delta) override;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    virtual void moveUp();
    virtual void moveDown();
    virtual void moveLeft();
    virtual void moveRight();
    virtual void stopVerticalMovement();
    virtual void stopHorizontalMovement();

    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving;
};

#endif // PLAYERMOVEMENT_H
