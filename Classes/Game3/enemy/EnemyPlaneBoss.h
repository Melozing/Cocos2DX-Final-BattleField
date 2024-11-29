#ifndef ENEMYPLANE_BOSS_H
#define ENEMYPLANE_BOSS_H

#include "cocos2d.h"
#include "EnemyPlaneBase.h"

class EnemyPlaneBoss : public EnemyPlaneBase {
public:
    static EnemyPlaneBoss* createEnemyPlaneBoss();
    void takeDamage(float damage);

    virtual bool init() override;
    void initAnimation();
    void spawnEnemy(cocos2d::Node* parent);
    void reset();
    Size GetSize();
    void moveLeftRight();
    float getHealth() const;
private:
    void createPhysicsBody();
    void moveUpAndReturnToPool();
    void startExplosions();
    cocos2d::SpriteBatchNode* spriteBatchNode;
    float health;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    bool isExploding;
};

#endif // ENEMYPLANE_BOSS_H
