/*#ifndef __SUICIDE_BOMBER_ENEMY_H__
#define __SUICIDE_BOMBER_ENEMY_H__

#include "Game2/Enemy/EnemyBase.h"

class SuicideBomberEnemy : public EnemyBase
{
public:
    SuicideBomberEnemy();
    virtual ~SuicideBomberEnemy();
    virtual bool init() override;
    virtual void update(float delta) override;

    CREATE_FUNC(SuicideBomberEnemy);

protected:
    void createIdleAnimation() override;
    void createAttackAnimation() override;
    void createDeathAnimation() override;
    void moveToPlayer() override;
    void die();
private:
    void explode();
    bool isPlayerInRange();
    float _explosionRadius;
    float _timeInRange;
    float _requiredTimeInRange;
    bool _isExploding;
    bool _isMoving;
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    cocos2d::RepeatForever* _idleAnimation;
    cocos2d::Animate* _deathAnimation;
};

#endif // __SUICIDE_BOMBER_ENEMY_H__
*/