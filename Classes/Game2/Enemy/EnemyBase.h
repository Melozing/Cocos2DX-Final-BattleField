#ifndef __ENEMY_BASE_H__
#define __ENEMY_BASE_H__

#include "cocos2d.h"

class EnemyBase : public cocos2d::Sprite {
public:
    virtual bool init() override;
    virtual void update(float delta) override;
    void updateRotationToPlayer();
    void moveToPlayer();
    virtual void reset();
    void explode();
    void dropRandomItem();
    void returnToPool();
    void resetSprite();

protected:
    cocos2d::Sprite* modelCharac; 
};

#endif // __ENEMY_BASE_H__

