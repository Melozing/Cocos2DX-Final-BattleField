// EnemyUtils.h
#pragma once

#include "cocos2d.h"

class EnemyUtils
{
public:
    static void moveToPlayer(cocos2d::Sprite* enemy, float speed, bool& isMoving, cocos2d::Action* walkAnimation);
    static bool isWithinRange(cocos2d::Sprite* enemy, cocos2d::Sprite* player, float range);
};
