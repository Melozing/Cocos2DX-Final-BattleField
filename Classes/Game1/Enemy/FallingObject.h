#ifndef __FALLING_OBJECT_H__
#define __FALLING_OBJECT_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"

class FallingObject : public cocos2d::Sprite, public SpriteController {
public:
    virtual bool init() override;
    void spawn(const cocos2d::Vec2& startPosition);
    void reset();
    void returnToPool();
    virtual void initAnimation() = 0;
    Size GetSize();

protected:
    cocos2d::Sprite* _currentSprite;
    float _speed = Constants::FALLINGROCK_ITEMS_SPEED;
    float _spriteScale;
    float _animationDelay = 0.1f;
};

#endif // __FALLING_OBJECT_H__
