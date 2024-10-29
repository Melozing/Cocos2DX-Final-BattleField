#ifndef __ENEMY_PLANE_BASE_H__
#define __ENEMY_PLANE_BASE_H__

#include "cocos2d.h"

class EnemyPlaneBase : public cocos2d::Sprite {
public:
    virtual bool init() override;

    void move(const cocos2d::Size& visibleSize, float speed);
    void moveRightToLeft(const cocos2d::Size& visibleSize, float speed);
    void moveAndReturn(const cocos2d::Size& visibleSize, float speed);

    static void spawnEnemyAfterDelay(float delay, cocos2d::Node* parent, std::function<EnemyPlaneBase* ()> createEnemyFunc);
    cocos2d::Sprite* modelCharac;
};

#endif
