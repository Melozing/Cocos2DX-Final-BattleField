#ifndef __FALLING_ROCK_H__
#define __FALLING_ROCK_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "FallingObject.h"

class FallingRock : public FallingObject {
public:
    enum class SpriteType {
        ROCK,
        LANDMINE
    };

    static FallingRock* create();

    virtual bool init() override;
    Size GetSize();
    void spawn(const cocos2d::Vec2& startPosition);
    void reset();
    void initAnimation() override;
private:
    void returnToPool();
    cocos2d::SpriteBatchNode* _spriteBatchNodeRock;
    cocos2d::SpriteBatchNode* _spriteBatchNodeLandmine;
    cocos2d::Sprite* _currentSprite;

    SpriteType _spriteType;
};

#endif // __FALLING_ROCK_H__
