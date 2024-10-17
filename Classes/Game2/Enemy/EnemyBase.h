// EnemyBase.h
#pragma once

#include "cocos2d.h"
#include "Constants/Constants.h"

class EnemyBase : public cocos2d::Sprite
{
public:
    EnemyBase();
    virtual ~EnemyBase();

    virtual bool init() override;
    virtual void update(float delta) override;
    virtual void die();
    virtual void attack();

    void setHealth(int health);
    int getHealth() const;

    void setSpeed(float speed);
    float getSpeed() const;

    void setDamage(int damage);
    int getDamage() const;

protected:
    int _health;
    float _speed;
    int _damage;
    bool _isDead;
    bool _isAttacking;

    virtual void createIdleAnimation() = 0;
    virtual void createAttackAnimation() = 0;
    virtual void createDeathAnimation() = 0;
    virtual void moveToPlayer() = 0;
    virtual void updateRotationToPlayer();
};
