#ifndef __GAME3SCENE_SCENE_H__
#define __GAME3SCENE_SCENE_H__

#include "cocos2d.h"
#include "Game3/Player/PlayerGame3.h"
#include "Background/Background3.h"
#include "Scene/BaseScene.h"
#include <Game2/Cursor/Cursor.h>


class Game3Scene : public BaseScene {
public:
	static cocos2d::Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(Game3Scene);
	

private:
	cocos2d::PhysicsWorld* world;
	void setPhysicWorld(cocos2d::PhysicsWorld* m_world) { world = m_world; }

	PlayerGame3* _player;
	Background3* background3;
	Cursor* _cursor;


	void handlePlayerMovement();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

};


#endif // __GAME3SCENE_SCENE_H__
