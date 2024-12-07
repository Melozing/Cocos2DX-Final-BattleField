#ifndef __GAME2_SCENE_H__
#define __GAME2_SCENE_H__

#include "cocos2d.h"
#include "Cursor/Cursor.h"
#include "Scene/BaseScene.h"
#include "Game2/Player/PlayerGame2.h"
#include "LoadingBar/CustomLoadingBar.h"
class Game2Scene : public BaseScene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    // implement the "static create()" method manually
    CREATE_FUNC(Game2Scene);
    void update(float delta) override;
    void spawnEnemies();
    void setupKeyboardEventListeners();
    void setupCursor();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void checkGameOver();
    void resetGameState();
    void takeDamage(int damage);
    void heal(int amount);
private:
    cocos2d::PhysicsWorld* world;
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }
    bool _isGameOver;

    CustomLoadingBar* _healthBar;
    PlayerAttributes* _playerAttributes;
    cocos2d::TMXTiledMap* _tileMap;
    PlayerGame2* _player;
    Cursor* _cursor;
    Sprite* _followSprite;
    void updateHealthBar();
    void initHealthBar();
    void spawnEnemy(const std::string& enemyType, const cocos2d::Vec2& position);
};

#endif // __GAME2_SCENE_H__
