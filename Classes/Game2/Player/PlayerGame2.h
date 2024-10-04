// PlayerGame2.h
#pragma once
#include "cocos2d.h"
#include "Controller/SpriteController.h"

class PlayerGame2 : public cocos2d::Sprite, public SpriteController
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
	void die();
private:
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void RotateToMouse();
    void initAnimation();
    void startMovementAnimation(); // Add this line

    cocos2d::Vec2 _mousePos;
    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving;
    Sprite* modelCharac;
};
