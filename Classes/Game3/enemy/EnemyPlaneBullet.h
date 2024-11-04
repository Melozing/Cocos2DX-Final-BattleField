#ifndef __ENEMY_PLANE_BULLET_H__
#define __ENEMY_PLANE_BULLET_H__

#include "EnemyPlaneBase.h"
#include "Controller/SpriteController.h"

class EnemyPlaneBullet : public EnemyPlaneBase {
public:
    static EnemyPlaneBullet* createEnemyBullet();
    void reset();
    virtual bool init() override;
    static void spawnEnemy(cocos2d::Node* parent);
    void initAnimation();
private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
};
#endif
