#ifndef ENEMYPLANE_BOSS_H
#define ENEMYPLANE_BOSS_H

#include "cocos2d.h"
#include "EnemyPlaneBase.h"

class EnemyPlaneBoss : public EnemyPlaneBase {
public:
    static EnemyPlaneBoss* create();
    void takeDamage(float damage);

    virtual bool init() override;
    void initAnimation();
    void spawnEnemy(float timeToUltimate);
    void reset();
    Size GetSize();
    void moveLeftRight();
    float getHealth();
    void updatePhase();
    void executeUltimateSkill(float timeToUltimate);

private:
    void createPhysicsBody();
    void moveUpAndReturnToPool();
    void startExplosions();
    void executePhaseSkills();
    void dropBooms();
    void graduallyIncreaseHealth();
    void dropUpgradeItem();
    void launchMissiles();
    void launchFinisherMissiles();
    void showWarning();

    enum class Phase {
        StartPHASE,
        PHASE_1,
        PHASE_2,
        PHASE_3
    };

    Phase currentPhase;
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    bool isExploding;
    cocos2d::SpriteBatchNode* _spriteBatchNodeWarning;
    cocos2d::Sprite* _warningSprite;
};

#endif // ENEMYPLANE_BOSS_H
