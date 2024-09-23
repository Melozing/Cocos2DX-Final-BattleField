// SplashScene.cpp
#include "Game2/Game2Scene.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Cursor/Cursor.h"

USING_NS_CC;

Scene* Game2Scene::createScene()
{
    return Game2Scene::create();
}

bool Game2Scene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto player = PlayerGame2::createPlayerGame2();
    this->addChild(player);

    auto eventListener = EventListenerKeyboard::create();

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
    this->addChild(_cursor);
    this->schedule([this](float delta) {
        _cursor->updateCursor(delta);
        }, "update_cursor_key");

    return true;
}
