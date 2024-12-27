#pragma once

#include <vector>
#include "Game2/Enemy/BunkerGame2.h"
#include "Game2/Enemy/MeleeEnemyGame2.h"
#include "Game2/Enemy/SniperEnemyGame2.h"
#include "Game2/Enemy/SuicideBomberEnemy.h"
#include "Game2/Enemy/SniperBulletGame2.h"
#include "Game2/Player/BulletGame2.h"
#include "Game2/Player/Grenade.h"

template <typename T>
class ObjectPoolGame2
{
public:
    static ObjectPoolGame2* getInstance() {
        static ObjectPoolGame2 instance;
        return &instance;
    }

    void initPool(int size);
    T* getObject();
    void returnObject(T* enemy);
    void resetPool();

private:
    std::vector<T*> pool;
};

// Usage
using BunkerGame2Pool = ObjectPoolGame2<BunkerGame2>;
using MeleeEnemyGame2Pool = ObjectPoolGame2<MeleeEnemyGame2>;
using SniperEnemyGame2Pool = ObjectPoolGame2<SniperEnemyGame2>;
using SuicideBomberEnemyPool = ObjectPoolGame2<SuicideBomberEnemy>;
using BulletGame2Pool = ObjectPoolGame2<BulletGame2>;
using SniperBulletGame2Pool = ObjectPoolGame2<SniperBulletGame2>;
using GrenadeEnemyPool = ObjectPoolGame2<Grenade>;