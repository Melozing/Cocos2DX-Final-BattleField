#ifndef __ENEMY_PLANE_BOOM_H__
#define __ENEMY_PLANE_BOOM_H__

#include "EnemyPlaneBase.h"
#include "Controller/SpriteController.h"

class EnemyPlaneBoom : public EnemyPlaneBase{
public:
    static EnemyPlaneBoom* createEnemyPlaneBoom();

    virtual bool init() override;

    static void spawnEnemyAfterDelay(float delay, cocos2d::Node* parent);

    void initAnimation();

    CREATE_FUNC(EnemyPlaneBoom);

private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
};

#endif