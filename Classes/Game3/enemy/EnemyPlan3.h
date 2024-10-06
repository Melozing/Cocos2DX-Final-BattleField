// EnemyPlan3.h
#ifndef __ENEMY_PLAN_3_H__
#define __ENEMY_PLAN_3_H__

#include "cocos2d.h"

class EnemyPlan3 : public cocos2d::Sprite {
public:
    static EnemyPlan3* createEnemyPlan3();

    virtual bool init();
    void move();  

    CREATE_FUNC(EnemyPlan3);
};

#endif 
