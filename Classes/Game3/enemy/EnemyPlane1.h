// EnemyPlane1.h
#ifndef __ENEMY_PLANE_1_H__
#define __ENEMY_PLANE_1_H__

#include "cocos2d.h"
#include "Controller/SpriteController.h"

class EnemyPlane1 : public cocos2d::Sprite, public SpriteController
{
public:
    EnemyPlane1();
    virtual ~EnemyPlane1();
    static EnemyPlane1* createEnemyPlane1();
    virtual bool init() override;
    void initAnimation();

private:
    cocos2d::Sprite* modelCharac;
};

#endif 
