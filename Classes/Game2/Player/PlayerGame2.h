#pragma once
#include "cocos2d.h"

class PlayerGame2 : public cocos2d::Sprite
{
public:
    PlayerGame2();
    virtual ~PlayerGame2();

    static PlayerGame2* createPlayerGame2();
    virtual bool init() override;
    void update(float delta) override;
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void stopVerticalMovement();
    void stopHorizontalMovement();
    void shootBullet(const cocos2d::Vec2& direction);
private:
    void createWalkAnimation();
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    void RotateToMouse();

    cocos2d::Vec2 _mousePos;
    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving;
    cocos2d::RepeatForever* _walkAnimation;
};
