// EnemyBase.h
#pragma once

#include "cocos2d.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"

class EnemyBase : public cocos2d::Sprite, public SpriteController
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

    void takeDamage(int damage);
protected:
    int _health;
    float _speed;
    int _damage;
    bool _isDead;
    bool _isAttacking;

    cocos2d::PhysicsBody* _physicsBody;

    virtual void createIdleAnimation() = 0;
    virtual void createAttackAnimation() = 0;
    virtual void createDeathAnimation() = 0;
    virtual void moveToPlayer() = 0;
    virtual void updateRotationToPlayer();
};

