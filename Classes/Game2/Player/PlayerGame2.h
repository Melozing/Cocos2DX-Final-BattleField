// PlayerGame2.h
#pragma once

#include "cocos2d.h"
#include "Bullet/BulletManager.h"
#include "Controller/SpriteController.h"
#include "Manager/PlayerMovementManager.h"

class PlayerGame2 : public cocos2d::Sprite, public SpriteController
{
public:
    PlayerGame2();
    virtual ~PlayerGame2();

    static PlayerGame2* createPlayerGame2();
    virtual bool init();
    void initAnimation();
    void startMovementAnimation();
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void update(float delta);
    void RotateToMouse();
    void shootBullet(const cocos2d::Vec2& direction);
    void throwGrenade(const cocos2d::Vec2& direction, float duration);
    void die();

private:
    cocos2d::Vec2 _mousePos;
    bool _isMouseDown;
    float _mousePressDuration;
    bool _isThrowingGrenade;
    cocos2d::Sprite* modelCharac;
    BulletManager* bulletManager;
    PlayerMovement* playerMovement;
};
