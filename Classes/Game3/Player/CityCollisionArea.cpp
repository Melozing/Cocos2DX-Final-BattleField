#include "CityCollisionArea.h"
#include "Controller/SpriteController.h"
#include "FX/Explodable.h"
#include "cocos2d.h"

USING_NS_CC;

CityCollisionArea* CityCollisionArea::createCityCollisionArea() {
    CityCollisionArea* area = new (std::nothrow) CityCollisionArea();
    if (area && area->init()) {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

bool CityCollisionArea::init() {
    if (!Node::init()) {
        return false;
    }
    // Set the content size manually if there is no sprite
    this->setAnchorPoint(Vec2(0.5f,0.5f));
    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width , Director::getInstance()->getVisibleSize().height / 12.5));

    // Create the physics body for the collision area
    createPhysicsBody();

    return true;
}

void CityCollisionArea::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, visibleSize.height / 12.5));

    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(0x01); // Set category bitmask if needed
    physicsBody->setCollisionBitmask(0x01); // Set collision bitmask if needed
    physicsBody->setContactTestBitmask(0x01); // Set contact test bitmask if needed
    physicsBody->setTag(1); // Set tag if needed
    auto shape = physicsBody->getFirstShape();
    if (shape) {
        shape->setSensor(true);
    }

    this->setPhysicsBody(physicsBody);

    // Position the collision area at the bottom of the screen
    this->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + (visibleSize.height / 12.5) / 2));
}

void CityCollisionArea::startExplosions() {
    float delayBetweenExplosions = 0.1f; // Adjust the delay between explosions as needed

    this->schedule([this](float dt) {
        // Generate random positions within the CityCollisionArea
        float randomX = random(0.0f, this->getContentSize().width);
        float randomY = random(0.0f, this->getContentSize().height);

        // Create explosion
        auto explosion = Explosion::create(Vec2(randomX, randomY), nullptr); // Adjust scale as needed
        this->addChild(explosion);
        }, delayBetweenExplosions, "explosion_schedule_key");
}
