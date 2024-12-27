#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "Controller/SpriteController.h"
#include "Game2/Player/Cannon.h"
#include "Game2/Player/Bullet_Petard.h"
#include "utils/PhysicsShapeCache.h"
#include "FX/Explodable.h"
#include "cocos2d.h"

USING_NS_CC;

Cannon* Cannon::create()
{
    Cannon* enemy = new (std::nothrow) Cannon();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool Cannon::init()
{
    if (!Sprite::init())
    {
        return false;
    }
     //createPetardHealth
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->health = Constants::PetardHealth;
    //add health bar
    this->addHealthBar();
    this->setupInitialPosition();
    modelCharac = Sprite::createWithSpriteFrameName("CannonGame2_13.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.15f));
    if (modelCharac->getParent() == nullptr) {
        this->addChild(modelCharac, Constants::ORDER_LAYER_CHARACTER);
    }
    this->createPhysicsBody();
    return true;
}

void Cannon::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/player/CannonGame2_.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("CannonGame2_", 13, 0.15f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void Cannon::setupInitialPosition()
{
    // Get the visible size of the screen
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Calculate the initial position to be in the center and slightly to the right
    float initialPosX = visibleSize.width / 2 - SpriteController::calculateScreenRatio(0.015f); // Center and move right by 50 units
    float initialPosY = visibleSize.height / 2 + SpriteController::calculateScreenRatio(0.08f); // Center vertically

    // Set the initial position of the petard
    this->setPosition(Vec2(initialPosX, initialPosY));
}

Size Cannon::GetSize() {
    return SpriteController::GetContentSizeSprite(modelCharac);
}

void Cannon::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/Petard.plist", "Petard", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "Petard", originalSize, 0.5f);

    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

void Cannon::addHealthBar() {
    auto healthBarBorder = Sprite::create("assets_game/UXUI/Loading/health_bar_g3_border.png");
    if (healthBarBorder) {
        healthBarBorder->setRotation(270); 
        healthBarBorder->setScale(0.5f); 

        float posX = this->getContentSize().width / 2 - 700;
        float posY = this->getContentSize().height + healthBarBorder->getContentSize().height / 2;
        healthBarBorder->setPosition(Vec2(posX, posY));

        this->addChild(healthBarBorder);

        auto healthBarProgress = Sprite::create("assets_game/UXUI/Loading/health_bar_g3_progress.png");
        if (healthBarProgress) {
            healthBarProgress->setRotation(270); 
            healthBarProgress->setScale(0.5f); 

            healthBarProgress->setPosition(Vec2(posX, posY+12));
            this->addChild(healthBarProgress);
        }
        else {
            CCLOG("Failed to add health bar progress to Petard");
        }
    }
    else {
        CCLOG("Failed to add health bar border to Petard");
    }
}




