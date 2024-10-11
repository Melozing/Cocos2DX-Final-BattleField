#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

class EnemyGame1 : public cocos2d::Sprite {
public:
    virtual bool init() override;
    virtual void update(float delta) = 0;
    virtual void spawn(const cocos2d::Vec2& startPosition) = 0; 
};

#endif // __ENEMY_H__
