// Game3Scene.h
#ifndef __GAME3SCENE_SCENE_H__
#define __GAME3SCENE_SCENE_H__

#include "cocos2d.h"
#include "Game3/Player/PlayerGame3.h"
#include "Scene/BaseScene.h"
#include <Game2/Cursor/Cursor.h>

class Game3Scene : public BaseScene {
public:
    static cocos2d::Scene* createScene();
    float _elapsedTime;
    virtual bool init() override;
    CREATE_FUNC(Game3Scene);
    void setupLoadingBar();

private:
    cocos2d::PhysicsWorld* world;
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }

    Cursor* _cursor;

    void setupBackground();
    void setupPlayer();
    void setupEnemies();
    void setupCursor();
    void setupEventListeners(PlayerGame3* player);
	

};

#endif // __GAME3SCENE_SCENE_H__
