// MeleeEnemyGame2.h
#ifndef __MELEE_ENEMY_GAME2_H__
#define __MELEE_ENEMY_GAME2_H__

#include "cocos2d.h"

class MeleeEnemyGame2 : public cocos2d::Sprite
{
public:
    MeleeEnemyGame2();
    virtual ~MeleeEnemyGame2();

    static MeleeEnemyGame2* createMeleeEnemyGame2();

    virtual bool init() override;
    virtual void update(float delta) override;

    void moveToPlayer();
    void attackPlayer();
    void die();
    void updateRotationToPlayer();
private:
    void createIdleAnimation();
    void createWalkAnimation();
    void createAttackAnimation();
    void createDeathAnimation();

    bool onContactBegin(cocos2d::PhysicsContact& contact);

    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving;
    bool _isAttacking;
    bool _isDead;

    cocos2d::RepeatForever* _idleAnimation;
    cocos2d::RepeatForever* _walkAnimation;
    cocos2d::Animate* _attackAnimation;
    cocos2d::Animate* _deathAnimation;
};

#endif // __MELEE_ENEMY_GAME2_H__
