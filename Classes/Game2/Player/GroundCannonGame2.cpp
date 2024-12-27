#include "GroundCannonGame2.h"

USING_NS_CC;

GroundCannonGame2* GroundCannonGame2::createGround()
{
    GroundCannonGame2* ground = GroundCannonGame2::create();
    if (ground && ground->init())
    {
        return ground;
    }
    CC_SAFE_DELETE(ground);
    return nullptr;
}

bool GroundCannonGame2::init()
{
    if (!Sprite::initWithFile("assets_game/player/LandCannon.png"))
    {
        return false;
    }

    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    setupPhysicsBody();

    return true;
}

void GroundCannonGame2::setupPhysicsBody()
{
    auto groundBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
    groundBody->setDynamic(false);
    groundBody->setGravityEnable(false);
    groundBody->setContactTestBitmask(true);
    groundBody->setCollisionBitmask(0x01);
    this->setPhysicsBody(groundBody);
}