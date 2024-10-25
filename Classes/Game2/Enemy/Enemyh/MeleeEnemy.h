// MeleeEnemy.h
#pragma once

#include "Game2/Enemy/EnemyBase.h"

class MeleeEnemy : public EnemyBase
{
public:
    MeleeEnemy();
    virtual ~MeleeEnemy();
    virtual bool init() override;
    CREATE_FUNC(MeleeEnemy);

protected:
    virtual void createIdleAnimation() override;
    virtual void createAttackAnimation() override;
    virtual void createDeathAnimation() override;
	void update(float delta) override;
    void die();
private:
    cocos2d::Vec2 _velocity;
    bool _isMoving;

    void moveToPlayer();
    void attackPlayer();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    cocos2d::RepeatForever* _idleAnimation;
    cocos2d::RepeatForever* _walkAnimation;
    cocos2d::Animate* _attackAnimation;
    cocos2d::Animate* _deathAnimation;
};
