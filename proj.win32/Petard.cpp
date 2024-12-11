#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "Controller/SpriteController.h"
#include "Game2/Player/Petard.h"
#include "Game2/Player/Bullet_Petard.h"
#include "utils/PhysicsShapeCache.h"
#include "FX/Explodable.h"
#include "cocos2d.h"

USING_NS_CC;

Petard* Petard::create()
{
    Petard* enemy = new (std::nothrow) Petard();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool Petard::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    modelCharac = Sprite::create("assets_game/player/petard.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    if (modelCharac->getParent() == nullptr) {
        this->addChild(modelCharac, Constants::ORDER_LAYER_CHARACTER);
    }
    return true;
}

void Petard::initOptions()
{

    // Schedule the shootBullet function to be called every 3 seconds
    this->schedule(CC_SCHEDULE_SELECTOR(Petard::shootBullet), 3.0f);
}

void Petard::addEvents()
{
    // Add event listeners for the petard
}

void Petard::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/player/petard.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName("petard.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("petard", 8, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void Petard::setupInitialPosition()
{
    // Get the visible size of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Calculate the initial position to be in the center and slightly to the right
    float initialPosX = visibleSize.width / 2 + 150; // Center and move right by 50 units
    float initialPosY = visibleSize.height / 2 + 150; // Center vertically

    // Set the initial position of the petard
    this->setPosition(Vec2(initialPosX, initialPosY));
    this->setAnchorPoint(Vec2(0.5, 0.7));
}

void Petard::shootBullet(float dt)
{
    auto bullet = Bullet_Petard::createBullet();
    if (bullet)
    {
        // Set the bullet's initial position to be the same as the petard's position
        bullet->setPosition(this->getPosition());

        // Add the bullet to the same parent as the petard
        this->getParent()->addChild(bullet);

        // Move the bullet to a random position
        bullet->moveToRandomPosition();

        CCLOG("Bullet shot by Petard");
    }
}

Size Petard::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void Petard::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    this->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height/2));

    modelCharac = Sprite::create("assets_game/player/petard.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    if (modelCharac->getParent() == nullptr) {
        this->addChild(modelCharac, Constants::ORDER_LAYER_CHARACTER);
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/Petard.plist", "petard", originalSize, scaledSize);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

// Enable debug drawing for physics bodies
void enableDebugDraw(Node* scene) {
    auto physicsWorld = scene->getScene()->getPhysicsWorld();
    physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
}