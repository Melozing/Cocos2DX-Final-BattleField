// EnemyBase.h
#pragma once

#include "cocos2d.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"
#include "Game2/Enemy/EnemyUtils.h"

class EnemyBase : public cocos2d::Sprite, public SpriteController {
public:
    EnemyBase();
    virtual ~EnemyBase();
    virtual bool init() override;
    virtual void update(float delta) override;
    void die();
    void attack();
    void moveToPlayer();
    void setHealth(int health);
    int getHealth() const;
    void setSpeed(float speed);
    float getSpeed() const;
    void setDamage(int damage);
    int getDamage() const;
    void setAttackRange(float range);
    float getAttackRange() const;
    void updateRotationToPlayer();
    void takeDamage(int damage);
    Size GetSize();
    void createPhysicsBody();

proteted :
    int _health;
    float _speed;
    int _damage;
    float _attackRange;
    bool _isDead;
    bool _isAttacking;
    bool _isMoving;

    cocos2d::Animation* createDeathAnimation();
    cocos2d::Animation* createAttackAnimation();
    cocos2d::Animation* createIdleAnimation();
};


