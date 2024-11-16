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
    virtual cocos2d::Animation* createIdleAnimation() override;
    virtual cocos2d::Animation* createAttackAnimation() override;
    virtual cocos2d::Animation* createDeathAnimation() override;
    virtual void update(float delta) override;
    virtual void createPhysicsBody() override;
private:
    cocos2d::Vec2 _velocity;
    bool _isMoving;

    void attackPlayer();
    cocos2d::RepeatForever* _idleAnimation;
    cocos2d::Animate* _attackAnimation;
    cocos2d::Animate* _deathAnimation;
};
