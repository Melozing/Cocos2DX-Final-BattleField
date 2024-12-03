#include "FallingTree.h"
#include "FallingTreePool.h"
#include "utils/PhysicsShapeCache.h"
#include "cocos2d.h"

USING_NS_CC;

FallingTree* FallingTree::create() {
    FallingTree* ret = new (std::nothrow) FallingTree();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FallingTree::init() {
    if (!FallingObject::init()) {
        return false;
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/FallingTree.plist");
    return true;
}

void FallingTree::initAnimation() {
    std::string spriteFrameName = "FallingTree1.png";
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/FallingTree.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    _currentSprite = Sprite::createWithSpriteFrameName(spriteFrameName);
    _currentSprite->setScale(SpriteController::updateSpriteScale(_currentSprite, 0.12f));
    spriteBatchNode->addChild(_currentSprite);

    auto animateCharac = Animate::create(createAnimation("FallingTree", 79, 0.01f));
    _currentSprite->runAction(RepeatForever::create(animateCharac));
    this->createPhysicsBody();
}

FallingTree::~FallingTree() {
    // Clean up if necessary
}

void FallingTree::returnToPool() {
    FallingObject::reset();
    FallingTreePool::getInstance()->returnEnemy(this);
}

void FallingTree::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = _currentSprite->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/FallingTree.plist", "FallingTree", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "FallingTree", originalSize, 0.123f);

    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setCollisionBitmask(0x02);
        this->setPhysicsBody(physicsBody);
    }
}