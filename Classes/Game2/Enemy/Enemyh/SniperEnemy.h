//#pragma once
//
//#include "Game2/Enemy/EnemyBase.h"
//
//class SniperEnemy : public EnemyBase
//{
//public:
//    SniperEnemy();
//    virtual ~SniperEnemy();
//    virtual bool init() override;
//    //CREATE_FUNC(SniperEnemy);
//
//protected:
//    void createIdleAnimation() override;
//    void createAttackAnimation() override;
//    void createDeathAnimation() override;
//    void update(float delta) override;
//    void die();
//private:
//    void shootBullet();
//    void updateRotationToPlayer();
//    bool onContactBegin(cocos2d::PhysicsContact& contact);
//    cocos2d::Vec2 _velocity;
//    float _speed;
//    bool _isShooting;
//    bool _isDead;
//
//    cocos2d::RepeatForever* _idleAnimation;
//    cocos2d::Animate* _shootAnimation;
//    cocos2d::Animate* _deathAnimation;
//
//    float _shootCooldown;
//    float _shootRange;
//};
