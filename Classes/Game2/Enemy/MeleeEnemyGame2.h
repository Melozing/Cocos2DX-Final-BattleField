#ifndef __MELEE_ENEMY_GAME2_H__
#define __MELEE_ENEMY_GAME2_H__

#include "cocos2d.h"
#include "Game2/Enemy/EnemyBase.h"

class MeleeEnemyGame2 : public EnemyBase {
public:
    static MeleeEnemyGame2* create();
    virtual bool init() override;
    virtual void reset() override;
};

#endif // __MELEE_ENEMY_GAME2_H__
