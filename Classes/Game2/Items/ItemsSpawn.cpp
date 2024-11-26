#include "ItemsSpawn.h"

USING_NS_CC;

ItemsSpawn* ItemsSpawn::create(ItemType type, const Vec2& position) {
    ItemsSpawn* item = new (std::nothrow) ItemsSpawn();
    if (item && item->init(type, position)) {
        item->autorelease();
		item->setScale(0.02f);
        return item;
    }
    delete item;
    return nullptr;
}

bool ItemsSpawn::init(ItemType type, const Vec2& position) {
    if (!Sprite::init()) {
        return false;
    }

    _type = type;
    _initialPosition = position;
    _time = 0.0f;

    std::string spritePath;
    switch (type) {
    case ItemType::AMMO:
        spritePath = "assets_game/items/hop_dan.png";
        break;
    /*case ItemType::HEALTH:
        spritePath = "assets_game/items/health.png";
        break;
    case ItemType::GRENADE:
        spritePath = "assets_game/items/grenade.png";
        break;*/
    }

    this->setTexture(spritePath);
    this->setPosition(position);

    this->scheduleUpdate();

    return true;
}

void ItemsSpawn::update(float delta) {
    _time += delta;
    float height = 10.0f * std::sin(_time * 2.0f);
    this->setPositionY(_initialPosition.y + height);
    this->setRotation(this->getRotation() + 90.0f * delta);
}
