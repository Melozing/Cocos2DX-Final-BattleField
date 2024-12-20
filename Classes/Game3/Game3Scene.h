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
#include "Game3/enemy/EnemyPlaneBoss.h" // Include EnemyPlaneBoss
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
    void updateBulletLabel();
    CityCollisionArea* getCityCollisionArea() const;

    // Method to handle the boss's ultimate skill
    void handleBossUltimateSkill(float timeToUltimate);

    // Methods to show and hide the UltimateSkillBadge
    void showUltimateSkillBadge(Ref* sender);
    void hideUltimateSkillBadge(Ref* sender);

private:
    ~Game3Scene();
    // Physics world
    cocos2d::PhysicsWorld* world;
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }

    // Boss 
    EnemyPlaneBoss* enemyBoos;

    // Cursor
    Cursor* _cursor;

    // City collision area
    CityCollisionArea* cityCollisionArea;

    // Health bar
    CustomLoadingBar* healthBar;

    // Elapsed time
    float _elapsedTime;

    // Setup methods
    void setupBackground();
    void setupPlayer();
    void initSpawning();
    void initBossSpawning();
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

    // Health bar methods
    void initHealthBar();
    void checkHealthBar();
    void updateHealthBar(float health);

    //Boss bar methods 
    CustomLoadingBar* bossHealthBar;
    void initBossHealthBar();
    void updateBossHealthBar(float healthPercent);
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
};

#endif // __GAME3SCENE_SCENE_H__
