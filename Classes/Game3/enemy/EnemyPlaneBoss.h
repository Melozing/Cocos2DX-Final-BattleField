#ifndef ENEMYPLANE_BOSS_H
#define ENEMYPLANE_BOSS_H

#include "cocos2d.h"
#include "EnemyPlaneBase.h"

class EnemyPlaneBoss : public EnemyPlaneBase {
public:
    static EnemyPlaneBoss* createEnemyPlaneBoss();
    void  takeDamage(float damage);

    virtual bool init() override;
    void initAnimation();
    static void spawnEnemy(cocos2d::Node* parent);
    void reset();
    Size GetSize();
private:
    void createPhysicsBody();
    cocos2d::SpriteBatchNode* spriteBatchNode;
    float health;
};

#endif // ENEMYPLANE_BOSS_H
