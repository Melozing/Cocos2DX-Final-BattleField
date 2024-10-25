#ifndef __PLAYER_GAME3_H__
#define __PLAYER_GAME3_H__

#include "cocos2d.h"
#include "Bullet/BulletManager.h"
#include "Manager/PlayerMovementManager.h"
#include "Controller/SpriteController.h"

class PlayerGame3 : public cocos2d::Sprite, public SpriteController
{
public:
    PlayerGame3();
    virtual ~PlayerGame3();

    static PlayerGame3* createPlayerGame3();
    virtual bool init() override;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void update(float delta) override;

private:
    void initAnimation();
    void shootBullet();
    void updateTurretRotation();
    void setTurretRotation(float angle);

    cocos2d::Sprite* turretSprite;
    cocos2d::Sprite* modelCharac;
    cocos2d::Vec2 _mousePos;
    BulletManager* bulletManager;
    PlayerMovement* playerMovement;
    bool isShooting;
};

#endif // __PLAYER_GAME3_H__
