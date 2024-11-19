#ifndef __ENEMY_PLANE_BOOM_H__
#define __ENEMY_PLANE_BOOM_H__

#include "EnemyPlaneBase.h"
#include "Controller/SpriteController.h"
#include "BoomForEnemyPlane.h"

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
    void spawnBoom(bool spawnFromLeft);
private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::SpriteBatchNode* explosionBatchNode;
    cocos2d::Sprite* explosionSprite;
    BoomForEnemyPlane* boom; // Add this line
};

#endif // __ENEMY_PLANE_BOOM_H__
