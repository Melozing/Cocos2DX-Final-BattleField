#ifndef __PlayerGame2_H__
#define __PlayerGame2_H__

#include "cocos2d.h"

class PlayerGame2 : public cocos2d::Sprite
{
public:
    PlayerGame2();
    virtual ~PlayerGame2();

    static PlayerGame2* createPlayerGame2();
    virtual bool init();

    void onMouseMove(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void stopVerticalMovement();
    void stopHorizontalMovement();

    void update(float delta);

private:
    cocos2d::Vec2 _mousePos;
    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving; // Add this line
    cocos2d::Animate* _animate; // Add this line
};

#endif // __PlayerGame2_H__
