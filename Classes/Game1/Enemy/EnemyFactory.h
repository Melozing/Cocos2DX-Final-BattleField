#ifndef __ENEMY_FACTORY_H__
#define __ENEMY_FACTORY_H__

#include "cocos2d.h"
#include "Game1/Enemy/Enemy.h"

class EnemyFactory {
public:
    static Enemy* spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position);
};

#endif // __ENEMY_FACTORY_H__
