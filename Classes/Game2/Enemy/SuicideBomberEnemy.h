#ifndef __SUICIDE_BOMBER_ENEMY_H__
#define __SUICIDE_BOMBER_ENEMY_H__

#include "cocos2d.h"
#include "Game2/Enemy/EnemyBase.h"
#include "Game2/Player/Cannon.h"

class SuicideBomberEnemy : public EnemyBase {
public:
    static SuicideBomberEnemy* create();
    virtual bool init() override;
    void reset() override;
    void setTarget(Cannon* target);
    void setInitialDirection();
    void update(float delta) override;

private:
    void initAnimation();
    void createPhysicsBody();
    void flipSpriteBasedOnDirection(const cocos2d::Vec2& direction);
    cocos2d::Size GetSize();

    Cannon* targetCannon;
    cocos2d::SpriteBatchNode* spriteBatchNode;
    cocos2d::Sprite* modelCharac;
};

#endif // __SUICIDE_BOMBER_ENEMY_H__
