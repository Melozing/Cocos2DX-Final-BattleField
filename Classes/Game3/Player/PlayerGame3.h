#ifndef __PLAYER_GAME3_H__
#define __PLAYER_GAME3_H__

#include "cocos2d.h"
#include "Manager/PlayerMovementManager.h"
#include "Controller/SpriteController.h"
#include "Bullet/Bullet.h"
#include "Bullet/BulletPool.h"

class PlayerGame3 : public cocos2d::Sprite, public SpriteController
{
public:
    static PlayerGame3* createPlayerGame3();
    virtual bool init() override;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
    void update(float delta) override;

private:
    void initAnimation();
    void shootBullet();
    void updateTurretRotation();
    bool updateDistanceToMouse(const Vec2& position);
    float calculateDistanceToMouse(const Vec2& position);

    void setupInitialPosition();
    void setupTurret();
    void setupEventListeners();
    void setupManagers();

    cocos2d::Sprite* turretSprite;
    cocos2d::Sprite* modelCharac;
    cocos2d::Vec2 _mousePos;
    PlayerMovement* playerMovement;
    bool isShooting;
    float distanceToMouse;
    bool isMouseDown;
    float shootDelay;
    float timeSinceLastShot;
};

#endif // __PLAYER_GAME3_H__
