#ifndef __FALLING_ROCK_H__
#define __FALLING_ROCK_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class FallingRock : public cocos2d::Sprite, public SpriteController {
public:
    enum class SpriteType {
        ROCK,
        LANDMINE
    };

    static FallingRock* create();

    virtual bool init() override;
    void spawn(const cocos2d::Vec2& startPosition);
    virtual void update(float delta) override;
    void reset();

    float getSpeed() const { return _speed; }
    void initAnimation();
    void removeWhenOutOfScreen();
    Size GetSize();

    virtual ~FallingRock();
private:
    cocos2d::SpriteBatchNode* _spriteBatchNodeRock;
    cocos2d::SpriteBatchNode* _spriteBatchNodeLandmine;
    cocos2d::Sprite* _currentSprite;

    SpriteType _spriteType; // Enum to hold the type of sprite (ROCK or LANDMINE)
    float _speed = 255.0f;
    float _spriteScale;
    float _animationDelay = 0.1f;
    bool _hasLandedOnMine = false;  // Flag for landing on mine
};

#endif // __FALLING_ROCK_H__