// EnemyPlane1.cpp

#include "EnemyPlane1.h"

USING_NS_CC;

EnemyPlane1::EnemyPlane1()
    : modelCharac(nullptr)
{
}

EnemyPlane1::~EnemyPlane1()
{
}

EnemyPlane1* EnemyPlane1::createEnemyPlane1()
{
    EnemyPlane1* enemy = new (std::nothrow) EnemyPlane1();
    if (enemy && enemy->init())
    {
        enemy->autorelease();
        enemy->initAnimation();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlane1::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // Get the screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Set position to the center of the screen
    this->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->setScale(3.0f); // Example scale

    return true;
}

void EnemyPlane1::initAnimation()
{
    // Load sprite frames from .plist file
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/Plane_enemy_bom.plist");

    // Create sprite with initial frame
    modelCharac = Sprite::createWithSpriteFrameName("Plane_enemy_bom1.png");
    if (modelCharac)
    {
        this->addChild(modelCharac);

        // Create animation using inherited method from SpriteController
        auto animateCharac = Animate::create(createAnimation("Plane_enemy_bom", 30, 0.07f));
        modelCharac->runAction(RepeatForever::create(animateCharac));
    }
    else
    {
        CCLOG("Failed to create sprite with frame 'Plane_enemy_bom1.png'");
    }
}
