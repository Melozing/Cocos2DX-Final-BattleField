#include "Game4/Game4Scene.h"
#include "utils/tilemap.h"

USING_NS_CC;

Scene* Game4Scene::createScene()
{
	return Game4Scene::create();
}

bool Game4Scene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Load the Tiled map
    auto map = TMXTiledMap::create("assets_game/gameplay/map.tmx");
    if (map == nullptr)
    {
        return false;
    }
    this->addChild(map, -1, "map"); // Background should have the lowest z-order

    // Debugging: Check map size and position
    auto mapSize = map->getContentSize();
    printf("Map size: width = %f, height = %f\n", mapSize.width, mapSize.height);

    auto mapPosition = map->getPosition();
    printf("Map position: x = %f, y = %f\n", mapPosition.x, mapPosition.y);

    // Get the object layer and set the player's initial position
    auto objectGroup = map->getObjectGroup("character");
    if (objectGroup == nullptr)
    {
        return false;
    }

    auto playerSpawnPoint = objectGroup->getObject("spawn");
    float x = playerSpawnPoint["x"].asFloat();
    float y = playerSpawnPoint["y"].asFloat();

    // Add the player to the scene
    player = Player::createPlayer();
    player->setPosition(Vec2(x, y));
    player->setScale(0.3f);
    this->addChild(player, 1); // Ensure player is rendered above the map

    // Schedule the update method
    this->scheduleUpdate();

    return true;
}

void Game4Scene::update(float delta)
{
    player->update(delta);
    auto camera = Camera::getDefaultCamera();
    camera->setPosition(player->getPosition());
}

bool Game4Scene::isCollidingWithLayer(Player* player, TMXLayer* layer)
{
    auto playerRect = player->getBoundingBox();
    auto tileSize = layer->getMapTileSize();
    auto mapSize = layer->getLayerSize();

    for (int x = 0; x < mapSize.width; ++x)
    {
        for (int y = 0; y < mapSize.height; ++y)
        {
            auto tileGID = layer->getTileGIDAt(Vec2(x, y));
            if (tileGID != 0)
            {
                auto tileRect = Rect(x * tileSize.width, y * tileSize.height, tileSize.width, tileSize.height);
                if (playerRect.intersectsRect(tileRect))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
