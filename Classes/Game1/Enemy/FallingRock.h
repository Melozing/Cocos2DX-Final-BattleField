#ifndef __FALLING_ROCK_H__
#define __FALLING_ROCK_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Controller/SpriteController.h"

class FallingRock : public Enemy, public SpriteController {
public:
    enum class SpriteType {
        ROCK,
        LANDMINE
    };

    static FallingRock* create();

    virtual bool init() override;
    virtual void spawn(const cocos2d::Vec2& startPosition) override;
    virtual void update(float delta) override;

    float getSpeed() const { return _speed; }
    void initAnimation();
    void removeWhenOutOfScreen();

    virtual ~FallingRock();

private:
    cocos2d::SpriteBatchNode* _spriteBatchNodeRock;
    cocos2d::SpriteBatchNode* _spriteBatchNodeLandmine;
    cocos2d::Sprite* _currentSprite;

    SpriteType _spriteType; // Enum to hold the type of sprite (ROCK or LANDMINE)
    float _speed = 75.0f;
    float _spriteScale = 1.0f;
    float _animationDelay = 0.1f;
    bool _hasLandedOnMine = false;  // Flag for landing on mine
};

#endif // __FALLING_ROCK_H__
