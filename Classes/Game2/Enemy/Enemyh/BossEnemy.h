/*#ifndef __BOSS_ENEMY_H__
#define __BOSS_ENEMY_H__

#include "Game2/Enemy/EnemyBase.h"

class BossEnemy : public EnemyBase
{
public:
    BossEnemy();
    virtual ~BossEnemy();

    virtual bool init() override;
    virtual void update(float delta) override;
    void updateHealthUI();
    CREATE_FUNC(BossEnemy);

protected:
    void createIdleAnimation() override;
    void createAttackAnimation() override;
    void createDeathAnimation() override;
    void moveToPlayer() override;
    void shoot();
    void reload();
    void spawnEnemies(int count);
    void specialSkill1();
    void specialSkill2();
    void die();
private:
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    float _shootInterval;
    float _timeSinceLastShot;
    float _reloadTime;
    bool _isReloading;
    bool _hasUsedSpecialSkill2;
    bool _isMoving;
    int _maxHealth;
    Sprite* _reloadSprite;
    cocos2d::RepeatForever* _idleAnimation;
	cocos2d::Animate* _shootAnimation;
	cocos2d::Animate* _deathAnimation; 
};

#endif // __BOSS_ENEMY_H__
*/