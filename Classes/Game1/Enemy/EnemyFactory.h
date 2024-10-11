#ifndef __ENEMY_FACTORY_H__
#define __ENEMY_FACTORY_H__

#include "cocos2d.h"
#include "Game1/Enemy/EnemyGame1.h"

class EnemyFactoryGame1 {
public:
    static EnemyGame1* spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position);
    static void returnEnemy(EnemyGame1* enemy);
};

#endif // __ENEMY_FACTORY_H__
