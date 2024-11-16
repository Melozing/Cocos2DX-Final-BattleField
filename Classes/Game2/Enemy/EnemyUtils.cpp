#include "Game2/Enemy/EnemyUtils.h"
#include "Game2/Player/PlayerGame2.h"
#include "Constants/Constants.h"

USING_NS_CC;

void EnemyUtils::moveToPlayer(Sprite* enemy, float speed, bool& isMoving, Action* walkAnimation)
{
    auto player = dynamic_cast<PlayerGame2*>(enemy->getParent()->getChildByName("PlayerGame2"));
    if (player)
    {
        Vec2 playerPos = player->getPosition();
        Vec2 pos = enemy->getPosition();
        Vec2 dirToPlayer = playerPos - pos;
        dirToPlayer.normalize();
        Vec2 velocity = dirToPlayer * speed;

        pos += velocity * Director::getInstance()->getDeltaTime();
        enemy->setPosition(pos);

        // Update rotation to face the player
        float angle = CC_RADIANS_TO_DEGREES(-dirToPlayer.getAngle());
        enemy->setRotation(angle + 90);

        if (!isMoving)
        {
            enemy->stopAllActions();
            enemy->runAction(walkAnimation);
            isMoving = true;
        }

        //CCLOG("Enemy moving to player: (%f, %f)", pos.x, pos.y);
    }
    else
    {
        CCLOG("Player not found");
    }
}

bool EnemyUtils::isWithinRange(Sprite* enemy, Sprite* player, float range)
{
    if (player)
    {
        Vec2 playerPos = player->getPosition();
        Vec2 pos = enemy->getPosition();
        float distance = pos.distance(playerPos);
        return distance <= range;
    }
    return false;
}
