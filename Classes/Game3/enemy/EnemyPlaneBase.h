#ifndef __ENEMY_PLANE_BASE_H__
#define __ENEMY_PLANE_BASE_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "FX/Explodable.h"

class EnemyPlaneBase : public SpriteController, public Explosion {
public:
    virtual bool init() override;

    void moveFromLeftToRight(const cocos2d::Size& visibleSize, float speed);
    void moveFromRightToLeft(const cocos2d::Size& visibleSize, float speed);
    void returnToPool();
    void resetSprite();
    void explode();
    void dropRandomItem();
    void update(float delta);
protected:
    cocos2d::Sprite* modelCharac;
};

#endif
