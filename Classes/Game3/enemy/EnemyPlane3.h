// EnemyPlane3.h
#ifndef __ENEMY_PLANE_3_H__
#define __ENEMY_PLANE_3_H__

#include "cocos2d.h"

class EnemyPlane3 : public cocos2d::Sprite {
public:
    static EnemyPlane3* createEnemyPlan3();

    virtual bool init();

    void move(const cocos2d::Size& visibleSize);
    void moveRightToLeft(const cocos2d::Size& visibleSize);
    void moveAndReturn(const cocos2d::Size& visibleSize);
    static void spawnEnemyAfterDelay(float delay, cocos2d::Node* parent);

    CREATE_FUNC(EnemyPlane3);
};

#endif 
