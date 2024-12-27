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

private:
    void initAnimation();
    void createPhysicsBody();
    void flipSpriteBasedOnDirection(const cocos2d::Vec2& direction);
    void switchToShootAnimation();
    void switchToRunAnimation();
    void shootBullet();

    PlayerGame2* targetPlayer;
    cocos2d::Sprite* runSprite;
    cocos2d::Sprite* shootSprite;
    SniperBulletGame2* bullet;

    enum class State {
        Moving,
        Shooting
    };

    State currentState;
    float stateTime;
    float moveDuration;
    float shootDuration;
    bool hasShotBullet;
};

#endif // __SNIPER_ENEMY_GAME2_H__
