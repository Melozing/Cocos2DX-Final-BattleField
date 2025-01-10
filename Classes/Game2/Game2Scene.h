#ifndef __GAME2_SCENE_H__
#define __GAME2_SCENE_H__

#include "cocos2d.h"
#include "Cursor/Cursor.h"
#include "Scene/BaseScene.h"
#include "Game2/Enemy/SuicideBomberEnemy.h"
#include "Game2/Enemy/SniperEnemyGame2.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Player/BulletGame2.h"
#include "Game2/Player/Cannon.h"
#include "Game2/Player/GroundCannonGame2.h" 
#include "LoadingBar/CustomLoadingBar.h"

class Game2Scene : public BaseScene
{
public:
    // Creates and returns a new scene object
    static cocos2d::Scene* createScene();

    // Initializes the scene
    virtual bool init() override;

    // Updates the scene every frame
    void update(float delta) override;

    // Sets up keyboard event listeners
    void setupKeyboardEventListeners();

    // Sets up the cursor
    void setupCursor();

    // Checks if the game is over
    void checkGameOver();

private:
    // Physics world for the scene
    cocos2d::PhysicsWorld* world;

    // Sets the physics world
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }

    // Handles the beginning of a physics contact event
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void handleSuicideBomberEnemyCollision(SuicideBomberEnemy* enemy, Cannon* cannon);
    void handleSniperEnemyAndBulletPlayerCollision(SniperEnemyGame2* enemy, BulletGame2* bullet);
    // Flag to check if the game is over
    bool _isGameOver;

    // Player attributes
    PlayerAttributes* _playerAttributes;

    // Player object
    PlayerGame2* _player;

    // Cannon object
    Cannon* _cannon;

    // Cursor object
    Cursor* _cursor;

    // Ground sprite object
    GroundCannonGame2* groundSprite;

    // Preloads assets for the scene
    void preloadAssets();

    // Sets up the background of the scene
    void setupBackground();

    // Sets up the player in the scene
    void setupPlayer();

    // Sets up the contact listener for physics events
    void setupContactListener();

    // Initializes the health bar
    void initHealthBar();

    // Initializes object pools
    void initPools();

    // Creates the ground for the scene
    void createGround();

    // Sets up bunkers in the scene
    void setupBunkers();

    // Sets up the cannon in the scene
    void setupCannon();
};

#endif // __GAME2_SCENE_H__
