#ifndef __ENEMY_PLANE_BOOM_H__
#define __ENEMY_PLANE_BOOM_H__

#include "EnemyPlaneBase.h"
#include "Controller/SpriteController.h"

class EnemyPlaneBoom : public EnemyPlaneBase {
public:
    static EnemyPlaneBoom* createEnemyPlaneBoom();
    void reset();
    virtual bool init() override;
    static void spawnEnemy(cocos2d::Node* parent);
    void initAnimation();
private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
};

#endif
