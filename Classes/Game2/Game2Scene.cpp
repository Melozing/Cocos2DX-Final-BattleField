#include "Game2/Game2Scene.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Cursor/Cursor.h"
#include "Game2/Enemy/MeleeEnemyGame2.h"
#include "Game2/Enemy/SniperEnemyGame2.h"
#include "utils/tilemap.h"
#include "Manager/PhysicsManager.h"
#include "Constants/Constants.h"
USING_NS_CC;
using namespace MyGameNamespace;

cocos2d::Scene* Game2Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = Game2Scene::create();
    scene->addChild(layer);

    return scene;
}

bool Game2Scene::init() {
    if (!BaseScene::init()) {
        CCLOG("Failed to initialize Scene");
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto tilemap = new Tilemap();
    tilemap->initTilemap(Vec2::ZERO);
    this->addChild(tilemap);

    auto map = TMXTiledMap::create("assets_game/gameplay/mapgame2.tmx");
    this->addChild(map);

    scaleTilemap(map);

    auto mapSize = map->getMapSize();
    auto tileSize = map->getTileSize();
    float scaleFactor = map->getScale();
    auto mapWidth = mapSize.width * tileSize.width * Constants::scaleFactor;
    auto mapHeight = mapSize.height * tileSize.height * Constants::scaleFactor;

    auto objectGroup = map->getObjectGroup("object");
    if (objectGroup) {
        auto objects = objectGroup->getObjects();
        for (auto& object : objects) {
            ValueMap& dict = object.asValueMap();
            float x = dict["x"].asFloat();
            float y = dict["y"].asFloat();
            auto node = Node::create();
            node->setPosition(Vec2(x, y));
            PhysicsManager::getInstance()->setPhysicsBody(node, 0x01, false);
            this->addChild(node);
        }
    }

    auto player = PlayerGame2::createPlayerGame2();
    if (!player) {
        CCLOG("Failed to create PlayerGame2");
        return false;
    }
    player->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    player->setName("PlayerGame2");
    this->addChild(player);
    CCLOG("PlayerGame2 added to Game2Scene");

    auto camera = Camera::create();
    camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(camera);

    this->schedule([this, player, camera, visibleSize, mapWidth, mapHeight](float delta) {
        Vec2 playerPos = player->getPosition();
        float x = std::max(playerPos.x, visibleSize.width / 2);
        float y = std::max(playerPos.y, visibleSize.height / 2);
        x = std::min(x, mapWidth - visibleSize.width / 2);
        y = std::min(y, mapHeight - visibleSize.height / 2);
        camera->setPosition(x, y);
        }, "update_camera_key");

    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [player](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            player->onKeyPressed(keyCode, event);
            break;
        default:
            break;
        }
        };

    eventListener->onKeyReleased = [player](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            player->onKeyReleased(keyCode, event);
            break;
        default:
            break;
        }
        };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    Director::getInstance()->getOpenGLView()->setCursorVisible(false);

    _cursor = Cursor::create("assets_game/player/tam.png");
    if (!_cursor) {
        CCLOG("Failed to create Cursor");
        return false;
    }
    this->addChild(_cursor);
    this->schedule([this](float delta) {
        _cursor->updateCursor(delta);
        }, "update_cursor_key");

    this->schedule([this](float delta) {
        auto enemy = SniperEnemyGame2::createSniperEnemyGame2();
        if (enemy) {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            float x = origin.x + visibleSize.width / 2;
            float y = origin.y + visibleSize.height - enemy->getContentSize().height / 2;
            enemy->setPosition(Vec2(x, y));
            this->addChild(enemy);
            enemy->scheduleUpdate();
            CCLOG("SniperEnemyGame2 spawned at (%f, %f)", x, y);
        }
        }, 5.0f, "spawn_enemy_key");

    CCLOG("Game2Scene initialized successfully");
    return true;
}

void Game2Scene::scaleTilemap(cocos2d::TMXTiledMap* tilemap) {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto mapSize = tilemap->getMapSize();
    auto tileSize = tilemap->getTileSize();

    float scaleX = visibleSize.width / (mapSize.width * tileSize.width);
    float scaleY = visibleSize.height / (mapSize.height * tileSize.height);

    float scale = std::max(scaleX, scaleY);

    tilemap->setScale(scale * Constants::scaleFactor);
}