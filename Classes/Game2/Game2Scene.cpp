// Game2Scene.cpp
#include "Game2/Game2Scene.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Cursor/Cursor.h"
#include "Game2/Enemy/MeleeEnemyGame2.h"
#include "Game2/Enemy/SniperEnemyGame2.h" // Include the sniper enemy header

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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Create and position the player at the center of the screen
    auto player = PlayerGame2::createPlayerGame2();
    if (!player)
    {
        CCLOG("Failed to create PlayerGame2");
        return false;
    }
    player->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    player->setName("PlayerGame2"); // Set the name for the player
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

    // Schedule enemy spawning every 3 seconds
    this->schedule([this](float delta) {
        auto enemy = SniperEnemyGame2::createSniperEnemyGame2();
        if (enemy) {
            // Set the enemy position at a fixed location (e.g., top center of the screen)
            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            float x = origin.x + visibleSize.width / 2;
            float y = origin.y + visibleSize.height - enemy->getContentSize().height / 2;
            enemy->setPosition(Vec2(x, y));
            this->addChild(enemy);
            enemy->scheduleUpdate();
            CCLOG("SniperEnemyGame2 spawned at (%f, %f)", x, y);
        }
        }, 3.0f, "spawn_enemy_key");

    CCLOG("Game2Scene initialized successfully");
    return true;
}
