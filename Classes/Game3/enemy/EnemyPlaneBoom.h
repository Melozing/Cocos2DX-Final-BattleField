﻿#ifndef __ENEMY_PLANE_BOOM_H__
#define __ENEMY_PLANE_BOOM_H__

#include "EnemyPlaneBase.h"
#include "Controller/SpriteController.h"
#include "BoomForEnemyPlane.h"

class EnemyPlaneBoom : public EnemyPlaneBase {
public:
    static EnemyPlaneBoom* create();
    virtual bool init() override;
    Size GetSize();
    void spawnEnemy(cocos2d::Node* parent, float skillTime, bool spawnWithSkill, const std::string& direction, const std::string& position);
    void initAnimation();
    void createPhysicsBody();
    void spawnBoom(bool spawnFromLeft);
    virtual void returnToPool() override;

private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::SpriteBatchNode* explosionBatchNode;
    cocos2d::Sprite* explosionSprite;
    BoomForEnemyPlane* boom; // Add this line
};

#endif // __ENEMY_PLANE_BOOM_H__
