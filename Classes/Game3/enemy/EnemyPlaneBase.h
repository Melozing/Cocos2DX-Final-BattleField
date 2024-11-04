#ifndef __ENEMY_PLANE_BASE_H__
#define __ENEMY_PLANE_BASE_H__

#include "cocos2d.h"

class EnemyPlaneBase : public cocos2d::Sprite {
public:
    virtual bool init() override;

    void moveFromLeftToRight(const cocos2d::Size& visibleSize, float speed);
    void moveFromRightToLeft(const cocos2d::Size& visibleSize, float speed);
    void returnToPool();
    void resetSprite();
protected:
    cocos2d::Sprite* modelCharac;
};

#endif
