#include "Game2/Game2Scene.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Cursor/Cursor.h"
#include "Game2/Enemy/MeleeEnemyGame2.h"
#include "Game2/Enemy/SniperEnemyGame2.h"
#include "utils/tilemap.h"
#include "Manager/PhysicsManager.h"
#include "Constants/Constants.h"

USING_NS_CC;

// Tạo scene với physics
cocos2d::Scene* Game2Scene::createScene() {
    auto scene = Scene::createWithPhysics();
    CCASSERT(scene != nullptr, "Failed to create Scene with Physics");
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = Game2Scene::create();
    scene->addChild(layer); // Thêm layer vào scene

    return scene;
}

// Khởi tạo scene
bool Game2Scene::init() {
    if (!BaseScene::init()) {
        CCLOG("Failed to initialize Scene");
        return false; // Kiểm tra khởi tạo thành công
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Tạo tilemap từ tệp tmx
    _tileMap = TMXTiledMap::create("assets_game/gameplay/mapgame2.tmx");
    CCASSERT(_tileMap != nullptr, "Failed to create TMXTiledMap");
    this->addChild(_tileMap); // Thêm tilemap vào scene

    auto objectGroup = _tileMap->getObjectGroup("object");
    CCASSERT(objectGroup != nullptr, "'object' object group not found");

    // Tạo nhân vật từ thông tin trong tilemap
    _player = PlayerGame2::createPlayerGame2();
    if (!_player) {
        CCLOG("Failed to create PlayerGame2");
        return false;
    }
    auto spawnPoint = objectGroup->getObject("playerSpawnPoint");
    float x = spawnPoint["x"].asFloat();
    float y = spawnPoint["y"].asFloat();
    _player->setPosition(Vec2(x, y)); // Đặt vị trí cho nhân vật
    _player->setName("PlayerGame2");
    this->addChild(_player); // Thêm nhân vật vào scene

    // Thiết lập sự kiện bàn phím để di chuyển nhân vật
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        _player->onKeyPressed(keyCode, event);
        };

    eventListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        _player->onKeyReleased(keyCode, event);
        };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this); // Gán listener cho scene

    // Ẩn con trỏ chuột
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);

    // Tạo con trỏ
    _cursor = Cursor::create("assets_game/player/tam.png");
    if (!_cursor) {
        CCLOG("Failed to create Cursor");
        return false;
    }
    this->addChild(_cursor); // Thêm con trỏ vào scene

    // Cập nhật con trỏ
    this->schedule([this](float delta) {
        _cursor->updateCursor(delta);
        }, "update_cursor_key");

    // Khởi tạo xuất hiện kẻ địch
    this->schedule([this](float delta) {
        auto enemy = SniperEnemyGame2::createSniperEnemyGame2();
        if (enemy) {
            const auto visibleSize = Director::getInstance()->getVisibleSize();
            const Vec2 origin = Director::getInstance()->getVisibleOrigin();
            const float x = origin.x + visibleSize.width / 2;
            const float y = origin.y + visibleSize.height - enemy->getContentSize().height / 2;
            enemy->setPosition(Vec2(x, y));
            this->addChild(enemy); // Thêm kẻ địch vào scene
            enemy->scheduleUpdate(); // Bắt đầu cập nhật kẻ địch
            CCLOG("SniperEnemyGame2 spawned at (%f, %f)", x, y);
        }
        }, 5.0f, "spawn_enemy_key");

    CCLOG("Game2Scene initialized successfully");
    scaleTilemap(_tileMap); // Điều chỉnh tỉ lệ tilemap
    this->scheduleUpdate(); // Bắt đầu cập nhật mỗi frame

    return true;
}

// Cập nhật mỗi frame
void Game2Scene::update(float delta) {
    Vec2 playerPos = _player->getPosition(); // Lấy vị trí của nhân vật

    // Tính toán vị trí camera mới
    Vec2 cameraPos = Camera::getDefaultCamera()->getPosition();
    cameraPos.x = playerPos.x; // Gán vị trí camera theo nhân vật
    cameraPos.y = playerPos.y;

    auto winSize = Director::getInstance()->getVisibleSize(); // Kích thước cửa sổ
    float mapWidth = _tileMap->getMapSize().width * _tileMap->getTileSize().width; // Chiều rộng bản đồ
    float mapHeight = _tileMap->getMapSize().height * _tileMap->getTileSize().height; // Chiều cao bản đồ

    // Đảm bảo camera không ra ngoài ranh giới bản đồ
    if (cameraPos.x < winSize.width / 2) cameraPos.x = winSize.width / 2;
    if (cameraPos.y < winSize.height / 2) cameraPos.y = winSize.height / 2;
    if (cameraPos.x > mapWidth - winSize.width / 2) cameraPos.x = mapWidth - winSize.width / 2;
    if (cameraPos.y > mapHeight - winSize.height / 2) cameraPos.y = mapHeight - winSize.height / 2;

    Camera::getDefaultCamera()->setPosition(cameraPos); // Đặt lại vị trí camera
}

// Điều chỉnh tỷ lệ cho tilemap
void Game2Scene::scaleTilemap(cocos2d::TMXTiledMap* tilemap) {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto mapSize = tilemap->getMapSize();
    auto tileSize = tilemap->getTileSize();

    float scaleX = visibleSize.width / (mapSize.width * tileSize.width); // Tính tỉ lệ theo chiều rộng
    float scaleY = visibleSize.height / (mapSize.height * tileSize.height); // Tính tỉ lệ theo chiều cao

    float scale = std::max(scaleX, scaleY); // Chọn tỉ lệ lớn hơn

    tilemap->setScale(scale * Constants::scaleFactor); // Áp dụng tỉ lệ cho tilemap
}