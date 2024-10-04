// SniperEnemyGame2.h
#ifndef __SNIPER_ENEMY_GAME2_H__
#define __SNIPER_ENEMY_GAME2_H__

#include "cocos2d.h"

class SniperEnemyGame2 : public cocos2d::Sprite
{
public:
    SniperEnemyGame2();
    virtual ~SniperEnemyGame2();

    static SniperEnemyGame2* createSniperEnemyGame2();
    virtual bool init() override;

    void update(float delta) override;
    void shootBullet();
    void die();
	void updateRotationToPlayer();
    bool onContactBegin(cocos2d::PhysicsContact& contact);

private:
    void createIdleAnimation();
    void createShootAnimation();
    void createDeathAnimation();

    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isShooting;
    bool _isDead;

    cocos2d::RepeatForever* _idleAnimation;
    cocos2d::Animate* _shootAnimation;
    cocos2d::Animate* _deathAnimation;

    float _shootCooldown;
};

#endif // __SNIPER_ENEMY_GAME2_H__
