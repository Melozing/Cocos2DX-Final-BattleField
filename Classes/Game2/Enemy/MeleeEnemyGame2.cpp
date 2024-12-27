#include "MeleeEnemyGame2.h"

USING_NS_CC;

MeleeEnemyGame2* MeleeEnemyGame2::create() {
    MeleeEnemyGame2* enemy = new (std::nothrow) MeleeEnemyGame2();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    delete enemy;
    return nullptr;
}

bool MeleeEnemyGame2::init() {
    if (!EnemyBase::init()) {
        return false;
    }
    return true;
}

void MeleeEnemyGame2::reset() {
    EnemyBase::reset();
    // Additional reset logic for MeleeEnemyGame2 if needed
}