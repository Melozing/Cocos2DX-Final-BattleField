#ifndef __SNIPER_ENEMY_GAME2_H__
#define __SNIPER_ENEMY_GAME2_H__

#include "cocos2d.h"
#include "Game2/Enemy/EnemyBase.h"
#include "Game2/Player/PlayerGame2.h"
#include "SniperBulletGame2.h"

class SniperEnemyGame2 : public EnemyBase {
public:
    static SniperEnemyGame2* create();
    virtual bool init() override;
    virtual void reset() override;
    void setTarget(PlayerGame2* target);
    void setInitialDirection();
    void update(float delta) override;
    void takeDamage(float damage);

private:
    void initAnimation();
    void createPhysicsBody();
    void flipSpriteBasedOnDirection(const cocos2d::Vec2& direction);
    void switchToShootAnimation();
    void switchToRunAnimation();
    void switchToDeathAnimation();
    void shootBullet();
    void blinkRed();

    PlayerGame2* targetPlayer;
    cocos2d::Sprite* runSprite;
    cocos2d::Sprite* shootSprite;
    cocos2d::Sprite* deathSprite;
    SniperBulletGame2* bullet;

    enum class State {
        Moving,
        Shooting,
        Dead
    };

    State currentState;
    float stateTime;
    float moveDuration;
    float shootDuration;
    bool hasShotBullet;
    float health;
    const float maxHealth = 3.0f;
};

#endif // __SNIPER_ENEMY_GAME2_H__
