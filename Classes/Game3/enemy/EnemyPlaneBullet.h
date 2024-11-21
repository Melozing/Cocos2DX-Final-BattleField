#ifndef __ENEMY_PLANE_BULLET_H__
#define __ENEMY_PLANE_BULLET_H__

#include "EnemyPlaneBase.h"
#include "Controller/SpriteController.h"

class EnemyPlaneBullet : public EnemyPlaneBase {
public:
    static EnemyPlaneBullet* createEnemyBullet();
    void reset();
    virtual bool init() override;
    Size GetSize();
    static void spawnEnemy(cocos2d::Node* parent);
    void initAnimation();
    void explode();
    void createPhysicsBody();
    void spawnBullets();
private:
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::SpriteBatchNode* explosionBatchNode;
    cocos2d::Sprite* warningSign;
    cocos2d::Sprite* explosionSprite;
    void showWarningSign();
    void hideWarningSign();
};


#endif
