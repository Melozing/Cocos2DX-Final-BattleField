//#pragma once
//
//#include "Game2/Enemy/EnemyBase.h"
//
//class InvEnemy : public EnemyBase
//{
//public:
//    InvEnemy();
//    virtual ~InvEnemy();
//    virtual bool init() override;
//   
//    CREATE_FUNC(InvEnemy);
//
//protected:
//	void createIdleAnimation() override;
//	void createAttackAnimation() override;
//	void createDeathAnimation() override;
//	void update(float delta) override;
//	void die();
//private:
//    bool onContactBegin(cocos2d::PhysicsContact& contact);
//    void shoot();
//    void goInvisible();
//    void moveToPlayer();
//    void moveRandomly();
//    void showEyeSprite();
//    void hideEyeSprite(); 
//    Sprite* _eyeSprite;
//    bool _isInvisible;
//    bool _isMoving;
//    float _shootInterval;
//    float _timeSinceLastShot;
//    cocos2d::RepeatForever* _idleAnimation;
//    cocos2d::Animate* _shootAnimation;
//    cocos2d::Animate* _deathAnimation;
//};
//
