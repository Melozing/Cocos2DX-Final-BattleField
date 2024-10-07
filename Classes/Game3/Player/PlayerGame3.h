// PlayerGame3.h
#ifndef __PLAYER_GAME3_H__
#define __PLAYER_GAME3_H__

#include "cocos2d.h"
#include "Bullet/Bullet.h"
#include "Bullet/BulletManager.h"
#include "Controller/SpriteController.h"

class PlayerGame3 : public cocos2d::Sprite, public SpriteController
{
public:
    PlayerGame3();
    virtual ~PlayerGame3();

    static PlayerGame3* createPlayerGame3();
    virtual bool init();
    void initAnimation();
    void moveLeft();
    void moveRight();
    void stopMovement();
    void update(float delta);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void movePlayer(const cocos2d::Vec2& direction);
    void updateTurret();
    void updateTurretRotation(const cocos2d::Vec2& targetPosition);
    void setTurretRotation(float angle);
    void shootBullet();
    void onMouseDown(cocos2d::Event* event);

private:
    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving;
    cocos2d::Sprite* modelCharac;
    cocos2d::Sprite* turretSprite;
    cocos2d::Vec2 _mousePos;
    BulletManager* bulletManager;
};

#endif // __PLAYER_GAME3_H__
