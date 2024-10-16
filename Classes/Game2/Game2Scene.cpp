#include "Game2/Game2Scene.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Cursor/Cursor.h"
#include "Game2/Enemy/MeleeEnemyGame2.h"
#include "Game2/Enemy/SniperEnemyGame2.h"
#include "utils/tilemap.h"
#include "Manager/PhysicsManager.h"
#include "Constants/Constants.h"
USING_NS_CC;

cocos2d::Scene* Game2Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    CCASSERT(scene != nullptr, "Failed to create Scene with Physics");
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

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _tileMap = TMXTiledMap::create("assets_game/gameplay/mapgame2.tmx");
    CCASSERT(_tileMap != nullptr, "Failed to create TMXTiledMap");
    this->addChild(_tileMap);

    auto objectGroup = _tileMap->getObjectGroup("object");
    CCASSERT(objectGroup != nullptr, "'object' object group not found");

    _player = PlayerGame2::createPlayerGame2(); // Sử dụng biến thành viên _player
    if (!_player) {
        CCLOG("Failed to create PlayerGame2");
        return false;
    }
    auto spawnPoint = objectGroup->getObject("playerSpawnPoint");
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();
    _player->setPosition(Vec2(x, y));
    _player->setName("PlayerGame2");
    this->addChild(_player);

    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _player->onKeyPressed(keyCode, event);
            break;
        default:
            break;
        }
        };

    eventListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _player->onKeyReleased(keyCode, event);
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
            const auto visibleSize = Director::getInstance()->getVisibleSize();
            const Vec2 origin = Director::getInstance()->getVisibleOrigin();
            const float x = origin.x + visibleSize.width / 2;
            const float y = origin.y + visibleSize.height - enemy->getContentSize().height / 2;
            enemy->setPosition(Vec2(x, y));
            this->addChild(enemy);
            enemy->scheduleUpdate();
            CCLOG("SniperEnemyGame2 spawned at (%f, %f)", x, y);
        }
        }, 5.0f, "spawn_enemy_key");

    CCLOG("Game2Scene initialized successfully");

    scaleTilemap(_tileMap);

    this->scheduleUpdate(); // Bắt đầu gọi hàm update mỗi frame

    return true;
}

void Game2Scene::update(float delta) {
    // Get the player's position
    Vec2 playerPos = _player->getPosition();

    // Calculate the new camera position
    Vec2 cameraPos = Camera::getDefaultCamera()->getPosition();
    cameraPos.x = playerPos.x;
    cameraPos.y = playerPos.y;

    // Get the visible size of the window
    auto winSize = Director::getInstance()->getVisibleSize();

    // Get the map size
    float mapWidth = _tileMap->getMapSize().width * _tileMap->getTileSize().width;
    float mapHeight = _tileMap->getMapSize().height * _tileMap->getTileSize().height;

    // Ensure the camera stays within the map boundaries
    if (cameraPos.x < winSize.width / 2) {
        cameraPos.x = winSize.width / 2;
    }
    if (cameraPos.y < winSize.height / 2) {
        cameraPos.y = winSize.height / 2;
    }
    if (cameraPos.x > mapWidth - winSize.width / 2) {
        cameraPos.x = mapWidth - winSize.width / 2;
    }
    if (cameraPos.y > mapHeight - winSize.height / 2) {
        cameraPos.y = mapHeight - winSize.height / 2;
    }

    // Set the camera position
    Camera::getDefaultCamera()->setPosition(cameraPos);
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
