// Game2Scene.cpp
#include "Game2/Game2Scene.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Cursor/Cursor.h"
#include "Game2/Enemy/MeleeEnemyGame2.h" // Include the enemy header

USING_NS_CC;

cocos2d::Scene* Game2Scene::createScene() {
    // Create a scene with physics enabled
    auto scene = Scene::createWithPhysics();

    // Optionally, you can set debug draw to see the physics shapes
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // Create the layer and add it to the scene
    auto layer = Game2Scene::create();
    scene->addChild(layer);

    return scene;
}

bool Game2Scene::init()
{
    if (!BaseScene::init())
    {
        CCLOG("Failed to initialize Scene");
        return false;
    }

    auto player = PlayerGame2::createPlayerGame2();
    if (!player)
    {
        CCLOG("Failed to create PlayerGame2");
        return false;
    }
    this->addChild(player);
    CCLOG("PlayerGame2 added to Game2Scene");

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
    if (!_cursor)
    {
        CCLOG("Failed to create Cursor");
        return false;
    }
    this->addChild(_cursor);
    this->schedule([this](float delta) {
        _cursor->updateCursor(delta);
        }, "update_cursor_key");

    // Add an enemy to the scene for testing
    auto enemy = MeleeEnemyGame2::createMeleeEnemyGame2();
    if (!enemy)
    {
        CCLOG("Failed to create MeleeEnemyGame2");
        return false;
    }
    enemy->setPosition(Vec2(200, 200));
    this->addChild(enemy);
    CCLOG("MeleeEnemyGame2 added to Game2Scene");

    CCLOG("Game2Scene initialized successfully");
    return true;
}
