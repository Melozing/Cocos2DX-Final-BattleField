#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "Background/Background.h"
#include "Player/Player.h"
#include "Enemy/FlyingBullet.h"
#include "Enemy/FallingRock.h"
#include "Enemy/RandomBoom.h"
#include "Enemy/EnemyFactory.h"
#include "Enemy/EnemyPool.h"

class Splash : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Splash);

    virtual void update(float delta);

private:
    std::vector<FlyingBullet*> _flyingBullets;
    std::vector<FallingRock*> _fallingRocks;
    std::vector<RandomBoom*> _randomBooms;

    // Handles player movement based on input
    void handlePlayerMovement();
    // Spawns enemies of different types
    void spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position);
    // Returns an enemy to the pool and makes it invisible
    void returnEnemyToPool(cocos2d::Node* enemy);
    // Handles enemies going out of bounds
    void onEnemyOutOfBounds(cocos2d::Node* enemy);
    // Schedules the spawning of different types of enemies
    void scheduleEnemySpawning();
    // Spawns FallingRock and RandomBoom enemies
    void SpawnFallingRockAndBomb(cocos2d::Size size);
    void removeFallingRock(FallingRock* fallingRock);
    // Spawns FlyingBullet enemies
    void SpawnFlyingBullet(cocos2d::Size size, bool directionLeft);
    // Spawns RandomBoom enemies
    void SpawnRandomBoom(cocos2d::Size size);

    // Player instance
    Player* _player;
    // Background instance
    Background* background;
    // Flag variables for player movement
    bool _movingUp, _movingDown, _movingLeft, _movingRight;
    // Enemy pool for reusing enemies
    std::vector<cocos2d::Node*> _enemyPool;
    // Sprite batch node to optimize rendering
    cocos2d::SpriteBatchNode* _spriteBatchNode;
};

#endif // __SPLASH_SCENE_H__
