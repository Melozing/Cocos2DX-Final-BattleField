#ifndef __PLAYER_GAME3_H__
#define __PLAYER_GAME3_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class PlayerGame3 : public cocos2d::Sprite
{
public:
    PlayerGame3();
    virtual ~PlayerGame3();
    static PlayerGame3* createPlayerGame3();
    virtual bool init();
    void update(float delta) override;

    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void moveLeft();
    void moveRight();
    void stopMovement();

    void startMovementAnimation();
    void startIdleAnimation();
    void movePlayer(const Vec2& direction);
    void updateTurret();
    void setTurretRotation(float angle);
    // Handle mouse movement events
    void updateTurretRotation(const cocos2d::Vec2& targetPosition);
    // Update turret rotation based on mouse position
private:
    void initAnimation();

    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving;
    cocos2d::Sprite* turretSprite;      // Turret sprite
    cocos2d::Vec2 mousePosition;        // Current mouse position for turret rotation
    cocos2d::Animation* createAnimation(const std::string& name, int frameCount, float delay);
    cocos2d::Sprite* modelCharac;
    cocos2d::Animate* idleAnimate;
    cocos2d::Animate* moveAnimate;
    cocos2d::Vec2 _mousePos;
};

#endif // __PLAYER_GAME3_H__
