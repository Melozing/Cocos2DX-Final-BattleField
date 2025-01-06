#ifndef __GAME3SCENE_SCENE_H__
#define __GAME3SCENE_SCENE_H__

#include "cocos2d.h"
#include "Game3/Player/PlayerGame3.h"
#include "Game3/Player/CityCollisionArea.h"
#include "Scene/BaseScene.h"
#include "Game3/enemy/EnemyPlaneBase.h"
#include "Game3/enemy/BoomForEnemyPlane.h"
#include "Game3/enemy/MissileForEnemyPlane.h"
#include "Game3/enemy/BulletForEnemyPlane.h"
#include "Game3/enemy/FinisherMissiles.h"
#include "Game3/enemy/EnemyPlaneBoss.h" 
#include "LoadingBar/CustomLoadingBar.h"
#include <Game2/Cursor/Cursor.h>
#include "UIGame/Badge.h"

class Game3Scene : public BaseScene {
public:
    // Create the scene
    static cocos2d::Scene* createScene();

    // Initialize the scene
    virtual bool init() override;

    // Macro to create the Game3Scene object
    CREATE_FUNC(Game3Scene);

    void updateBulletLabel(cocos2d::Ref* sender);
    CityCollisionArea* getCityCollisionArea() const;

    // Method to handle the boss's ultimate skill
    void handleBossUltimateSkill(Ref* sender);

    // Methods to show and hide the UltimateSkillBadge
    void showUltimateSkillBadge(Ref* sender);
    void hideUltimateSkillBadge(Ref* sender);

    // Method to spawn or update the boss
    void spawnOrUpdateBoss(float timeToUltimate);

private:
    ~Game3Scene();
    // Physics world
    cocos2d::PhysicsWorld* world;
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }
    void transitionToNextScene(Ref* sender);
    void checkAndUpdateBackground();

    // Boss 
    EnemyPlaneBoss* enemyBoss = nullptr;
    void initBoss();

    // Cursor
    Cursor* _cursor;

    // City collision area
    CityCollisionArea* cityCollisionArea;

    // Health bar
    CustomLoadingBar* healthBar;
    PlayerGame3* playerGame3;

    // Elapsed time
    float _elapsedTime;

    // Setup methods
    void setupBackground();
    void setupPlayer();
    void initBulletSpawning(const std::string& jsonFilePath);
    void initBoomSpawning(const std::string& jsonFilePath);
    void initBossSpawning(const std::string& jsonFilePath);
    void setupCursor();
    void setupEventListeners(PlayerGame3* player);
    void initPools();
    void setupContactListener();

    // Contact listener
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    // Collision handling methods
    void handleBulletEnemyCollision(BulletPlayerGame3* bullet, EnemyPlaneBase* enemy);
    void handleBulletBoomCollision(BulletPlayerGame3* bullet, BoomForEnemyPlane* boom);
    void handleBulletMissileCollision(BulletPlayerGame3* bullet, MissileForEnemyPlane* missileEnemy);
    void handleBoomCityCollision(BoomForEnemyPlane* boom);
    void handleBulletForEnemyCityCollision(BulletForEnemyPlane* bulletForEnemy);
    void handleMissileEnemyCityCollision(MissileForEnemyPlane* missileEnemy);
    void handleFinisherMissilesCityCollision(FinisherMissiles* FinisherMissilesBoss);

    // Health bar methods
    void initHealthBar();
    void checkHealthBar();
    void updateHealthBar(float health);

    //Boss bar methods 
    CustomLoadingBar* bossHealthBar;
    void initBossHealthBar();
    void updateBossHealthBar(Ref* sender);
    void handleBossDamage(float damage);
    void showBossHealthBar(Ref* sender);
    void hideBossHealthBar(Ref* sender);

    //Sound 
    void initSound();
    void preloadAssets();

    //bulletBadge
    Badge* bulletBadge;
    void initBulletBadge();
    void blinkRedBadge(Ref* sender);

    // Badge for displaying the countdown
    Badge* ultimateSkillBadge;
    void initUltimateSkillBadge();
    void updateUltimateSkillCountdown(float dt);
    float ultimateSkillTimeRemaining;
    bool finisherMissilesHandled = false;

    // Background
    bool isBackgroundNormal;
    bool isBackgroundBreak;

    // Format Text
    std::string Game3Scene::formatTime(int totalSeconds);
};

#endif // __GAME3SCENE_SCENE_H__
