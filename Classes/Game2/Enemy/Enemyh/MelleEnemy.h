// MeleeEnemy.h
#pragma once

#include "Game2/Enemy/EnemyBase.h"

class MeleeEnemy : public EnemyBase
{
public:
    MeleeEnemy();
    virtual ~MeleeEnemy();

    virtual bool init() override;
    CREATE_FUNC(MeleeEnemy);

protected:
    virtual void createIdleAnimation() override;
    virtual void createAttackAnimation() override;
    virtual void createDeathAnimation() override;
    virtual void moveToPlayer() override;
};

