#ifndef __ENEMY_PLANE_BOOM_H__
#define __ENEMY_PLANE_BOOM_H__

#include "EnemyPlaneBase.h"
#include "Controller/SpriteController.h"

class EnemyPlaneBoom : public EnemyPlaneBase {
public:
    static EnemyPlaneBoom* createEnemyPlaneBoom();
    void reset();
    virtual bool init() override;
    Size GetSize();
    static void spawnEnemy(cocos2d::Node* parent);
    void initAnimation();
    void explode();
    void createPhysicsBody();
private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::SpriteBatchNode* explosionBatchNode;
    cocos2d::Sprite* explosionSprite;
};

#endif
