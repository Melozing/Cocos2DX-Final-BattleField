#ifndef __FALLING_TREE_H__
#define __FALLING_TREE_H__

#include "cocos2d.h"
#include "Game1/Enemy/Enemy.h"

class FallingTree : public Enemy {
public:
    virtual bool init() override; 
    virtual void spawn(const cocos2d::Vec2& startPosition) override; 
    virtual void update(float delta) override; 
};

#endif // __FALLING_TREE_H__
