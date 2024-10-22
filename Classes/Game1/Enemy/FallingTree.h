#ifndef __FALLING_TREE_H__
#define __FALLING_TREE_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"

class FallingTree : public cocos2d::Sprite, public SpriteController {
public:
    static FallingTree* create();

    virtual bool init() override;
    void spawn(const cocos2d::Vec2& startPosition);
    virtual void update(float delta) override;
    void reset();
    Size GetSize();
    
    virtual ~FallingTree();
private:
    cocos2d::Sprite* _currentSprite;
    float _speed = Constants::FALLINGROCK_ITEMS_SPEED;
    float _spriteScale;
    void removeWhenOutOfScreen();
};

#endif // __FALLING_TREE_H__
