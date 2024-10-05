#include "Game3/Game3Scene.h"
#include "Game3/Player/PlayerGame3.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "Controller/GameController.h"
#include "Background/Background3.h"
#include "Game2/Cursor/Cursor.h"


USING_NS_CC;

cocos2d::Scene* Game3Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = Game3Scene::create();
    scene->addChild(layer);

    return scene;
}//Create new Scene3

bool Game3Scene::init() {
    if (!BaseScene::init()) {
        return false;
    }

    
    auto background3 = Background3::createBackground("assets_game/gameplay/Game3.2.png");
    if (!background3) {
        return false; 
    }

    this->addChild(background3);


    auto player = PlayerGame3::createPlayerGame3();
    if (!player)
    {
        CCLOG("Failed to create PlayerGame3");
        return false;
    }
    this->addChild(player);
   


   /* auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_W:
            player->moveUp();
            break;
        case EventKeyboard::KeyCode::KEY_S:
            player->moveDown();
            break;
        case EventKeyboard::KeyCode::KEY_A:
            player->moveLeft();
            break;
        case EventKeyboard::KeyCode::KEY_D:
            player->moveRight();
            break;
        default:
            break;
        }
        };

    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_S:
            player->stopVerticalMovement();
            break;
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_D:
            player->stopHorizontalMovement();
            break;
        default:
            break;
        }
        };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    Director::getInstance()->getOpenGLView()->setCursorVisible(false);

    _cursor = Cursor::create("assets_game/player/tam.png");
    if (!_cursor)
    {
        CCLOG("Failed to create Cursor");
        return false;
    }
    this->addChild(_cursor);
    this->schedule([this](float delta) {
        _cursor->updateCursor(delta);
        }, "update_cursor_key");
   
    return true;*/

}
