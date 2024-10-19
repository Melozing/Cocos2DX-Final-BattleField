<<<<<<< HEAD
﻿// SniperEnemy.h
#pragma once

#include "Game2/Enemy/EnemyBase.h"

class SniperEnemy : public EnemyBase
{
public:
    SniperEnemy();
    virtual ~SniperEnemy();
    virtual bool init() override;
    CREATE_FUNC(SniperEnemy);

protected:
    void createIdleAnimation() override;
    void createAttackAnimation() override;
    void createDeathAnimation() override;
    void update(float delta) override;
    void die();
    void attack();
    void moveToPlayer();
private:

    void shootBullet();
    void reload();
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isShooting;
    bool _isDead;
    bool _isReloading;
    bool _isMoving;
    int _ammoCount;

    cocos2d::RepeatForever* _idleAnimation;
    cocos2d::Animate* _shootAnimation;
    cocos2d::Animate* _deathAnimation;
    cocos2d::Animate* _reloadAnimation;

    float _shootCooldown;
    float _shootRange;
};
=======
>>>>>>> parent of 84e12b3 (up game2)
