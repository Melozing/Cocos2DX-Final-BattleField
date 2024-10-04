#ifndef __GAME1SCENE_SCENE_H__
#define __GAME1SCENE_SCENE_H__

#include "Background/Background.h"
#include "Game1/Items/CollectibleItem.h"
#include "Game1/Player/PlayerGame1.h"
#include "Enemy/FlyingBullet.h"
#include "Enemy/FallingRock.h"
#include "Enemy/RandomBoom.h"
#include "Enemy/EnemyFactory.h"
#include "Enemy/EnemyPool.h"
#include "Game1/Player/HealthPlayerGame1.h"
#include "Button/PauseButton.h"
#include "Scene/BaseScene.h"
#include "ui/UILoadingBar.h"

class Game1Scene : public BaseScene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(Game1Scene);
    virtual void update(float delta);

private:
    cocos2d::PhysicsWorld* world;
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }

    PlayerAttributes* _playerAttributes;
    bool _canTakeDamage;
    std::vector<FlyingBullet*> _flyingBullets;
    std::vector<FallingRock*> _fallingRocks;
    std::vector<RandomBoom*> _randomBooms;
    std::vector<Vec2> usedPositions;
    std::vector<cocos2d::Node*> _enemyPool;
    PlayerGame1* _player;
    Background* background;
    HealthPlayerGame1* _healthPlayerGame1;
    bool _isGameOver;

    bool _movingUp, _movingDown, _movingLeft, _movingRight;

    PauseButton* _pauseButton;
    cocos2d::ui::LoadingBar* _loadingBar;
    Sprite* border;

    void handlePlayerMovement();
    void spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position);
    void returnEnemyToPool(cocos2d::Node* enemy);
    void onEnemyOutOfBounds(cocos2d::Node* enemy);
    void scheduleEnemySpawning();
    void SpawnFallingRockAndBomb(cocos2d::Size size);
    void SpawnFlyingBullet(cocos2d::Size size, bool directionLeft);
    void SpawnRandomBoom(cocos2d::Size size);
    bool isPositionOccupied(const Vec2& position);
    void trackUsedPosition(const Vec2& position);
    void scheduleCollectibleSpawning();

    void checkCollisions();
    void setPhysicsBodyChar(cocos2d::PhysicsBody* physicBody, int num);
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    void checkGameOver();

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void updateLoadingBar(float dt);

    void SpawnCollectibleItem(const Size& size);
    Vec2 getRandomSpawnPosition(const Size& size);
};

#endif // __GAME1SCENE_SCENE_H__
