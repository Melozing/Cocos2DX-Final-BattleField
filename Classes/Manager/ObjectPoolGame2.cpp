#include "Manager/ObjectPoolGame2.h"

template <typename T>
void ObjectPoolGame2<T>::initPool(int size) {
    for (int i = 0; i < size; ++i) {
        T* enemy = T::create();
        if (enemy) {
            enemy->retain();
            pool.push_back(enemy);
        }
    }
}

template <typename T>
T* ObjectPoolGame2<T>::getObject() {
    if (pool.empty()) {
        T* enemy = T::create();
        if (enemy) {
            enemy->retain();
            return enemy;
        }
        return nullptr;
    }
    T* enemy = pool.back();
    pool.pop_back();
    return enemy;
}

template <typename T>
void ObjectPoolGame2<T>::returnObject(T* enemy) {
    if (enemy) {
        enemy->reset();
        pool.push_back(enemy);
    }
}

template <typename T>
void ObjectPoolGame2<T>::resetPool() {
    while (!pool.empty()) {
        T* enemy = pool.back();
        pool.pop_back();
        enemy->release();
    }
}

// Explicit template instantiation
template class ObjectPoolGame2<BunkerGame2>;
template class ObjectPoolGame2<MeleeEnemyGame2>;
template class ObjectPoolGame2<SniperEnemyGame2>;
template class ObjectPoolGame2<SuicideBomberEnemy>;
template class ObjectPoolGame2<BulletGame2>;
template class ObjectPoolGame2<SniperBulletGame2>;
template class ObjectPoolGame2<Grenade>;