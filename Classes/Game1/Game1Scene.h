#ifndef __GAME1SCENE_SCENE_H__
#define __GAME1SCENE_SCENE_H__

#include "Background/Background.h"
#include "Game1/Items/CollectibleItem.h"
#include "Game1/Player/PlayerGame1.h"
#include "Enemy/FlyingBullet.h"
#include "Enemy/FallingRock.h"
#include "Enemy/RandomBoom.h"
#include "Enemy/FlyingBulletPool.h"
#include "Enemy/FallingRockPool.h"
#include "Enemy/RandomBoomPool.h"
#include "Game1/Player/HealthPlayerGame1.h"
#include "Scene/BaseScene.h"
#include "ui/UILoadingBar.h"
#include "utils/Music/MusicAnalyzer.h"
#include "Controller/SoundController.h"
#include "utils/Music/MusicEvent.h" 
#include <functional>

class Game1Scene : public BaseScene {
public:
    // Public methods
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(Game1Scene);
    virtual void update(float delta);

private:
    // Physics world
    cocos2d::PhysicsWorld* world;
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }

    // Player attributes
    PlayerAttributes* _playerAttributes;
    PlayerGame1* _player;
    HealthPlayerGame1* _healthPlayerGame1;
    bool _canTakeDamage;
    bool _isGameOver;

    // Background
    Background* background;

    // Enemy management
    std::vector<FlyingBullet*> _flyingBullets;
    std::vector<FallingRock*> _fallingRocks;
    std::vector<RandomBoom*> _randomBooms;
    std::vector<CollectibleItem*> _collectibleItems;
    std::vector<Vec2> usedPositions;
    std::vector<cocos2d::Node*> _enemyPool;

    // Music and sound
    MusicAnalyzer* _musicAnalyzer;
    SoundController* _soundController;
    float musicDuration;
    bool shouldPlayMusic;

    // UI elements
    cocos2d::ui::LoadingBar* _loadingBar;
    Sprite* border;

    // Player movement
    //void setupMovementListener(EventListenerKeyboard* listener);

    // Enemy spawning
    void SpawnFallingRockAndBomb(cocos2d::Size size);
    void SpawnFlyingBullet(cocos2d::Size size, bool directionLeft);
    void SpawnRandomBoom(cocos2d::Size size);
    bool isPositionOccupied(const Vec2& position);
    void trackUsedPosition(const Vec2& position);

    // Collectible spawning
    void scheduleCollectibleSpawning();
    void SpawnCollectibleItem(const Size& size);
    Vec2 getRandomSpawnPosition(const Size& size);

    // Collision handling
    void checkCollisions();
    void setPhysicsBodyChar(cocos2d::PhysicsBody* physicBody, int num);
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    // Game over handling
    void checkGameOver();

    //// Input handling
    //void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    //void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // UI updates
    void updateLoadingBar(float dt);

    // Music-based spawning
    void handleMusicBasedSpawning(float dt);
    void spawnBasedOnMusicEvent(MusicEvent event);

    // New methods for exitAction and createSceneFunc
    std::function<void()> exitAction;
    std::function<cocos2d::Scene* ()> createSceneFunc;

};

#endif // __GAME1SCENE_SCENE_H__
