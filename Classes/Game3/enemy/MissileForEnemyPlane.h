#ifndef __MISSILEFORENEMYPLANE_H__
#define __MISSILEFORENEMYPLANE_H__

#include "cocos2d.h"

class MissileForEnemyPlane : public cocos2d::Sprite {
public:
    static MissileForEnemyPlane* create();
    void reset();
    virtual bool init() override;
    void moveDown(bool spawnFromLeft);
    void returnToPool();
    void explode();
    void createPhysicsBody();
    void initAnimation();

private:
    cocos2d::Size GetSize();
    bool isOutOfScreen;
    cocos2d::Sprite* modelCharac;
    cocos2d::SpriteBatchNode* spriteBatchNode;
};

#endif // __MISSILEFORENEMYPLANE_H__
