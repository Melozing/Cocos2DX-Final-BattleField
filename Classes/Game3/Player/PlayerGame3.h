#ifndef __PLAYER_GAME3_H__
#define __PLAYER_GAME3_H__

#include "cocos2d.h"

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

private:
    void initAnimation();

    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving;
    cocos2d::Animation* createAnimation(const std::string& name, int frameCount, float delay);
    cocos2d::Sprite* modelCharac;
    cocos2d::Animate* idleAnimate;
    cocos2d::Animate* moveAnimate;
};

#endif // __PLAYER_GAME3_H__
