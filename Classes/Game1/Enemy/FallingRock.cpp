#include "FallingRock.h"
#include "FallingRockPool.h"
#include "Controller/SpriteController.h"
#include "cocos2d.h"

USING_NS_CC;

FallingRock* FallingRock::create() {
    FallingRock* ret = new (std::nothrow) FallingRock();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FallingRock::init() {
    if (!Sprite::init()) { // Assuming Sprite is the superclass
        return false;
    }

    // Randomly select either ROCK or LANDMINE
    _spriteType = (rand() % 2 == 0) ? SpriteType::ROCK : SpriteType::LANDMINE;

    return true;
}

void FallingRock::reset() {
    this->setVisible(false);
}

Size FallingRock::GetSize() {
    return GetContentSizeSprite(_currentSprite);
}

void FallingRock::initAnimation() {
    std::string spriteFrameName;

    // Depending on the sprite type, set appropriate properties
    if (_spriteType == SpriteType::ROCK) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/falling_rock.plist");
        spriteFrameName = "falling_rock1.png";
        _animationDelay = 0.07f; // Animation delay for rock

        _spriteBatchNodeRock = SpriteBatchNode::create("assets_game/enemies/falling_rock.png");

        if (_spriteBatchNodeRock->getParent() == nullptr) {
            this->addChild(_spriteBatchNodeRock);
        }

        // Create the sprite and add it to the rock batch node
        _currentSprite = Sprite::createWithSpriteFrameName(spriteFrameName);
        _spriteBatchNodeRock->addChild(_currentSprite);
    }
    else {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/landmine.plist");
        spriteFrameName = "landmine1.png";
        _animationDelay = 0.15f; // Animation delay for landmine

        _spriteBatchNodeLandmine = SpriteBatchNode::create("assets_game/enemies/landmine.png");


        if (_spriteBatchNodeLandmine->getParent() == nullptr) {
            this->addChild(_spriteBatchNodeLandmine);
        }

        // Create the sprite and add it to the landmine batch node
        _currentSprite = Sprite::createWithSpriteFrameName(spriteFrameName);
        _spriteBatchNodeLandmine->addChild(_currentSprite);
    }
    _spriteScale = SpriteController::updateSpriteScale(_currentSprite, 0.13f); // Scale for rock
    _currentSprite->setScale(_spriteScale);

    // Create animation with the customized delay for rock or landmine
    auto animateCharac = Animate::create(createAnimation((_spriteType == SpriteType::ROCK) ? "falling_rock" : "landmine", 4, _animationDelay));
    _currentSprite->runAction(RepeatForever::create(animateCharac));
}

void FallingRock::spawn(const Vec2& startPosition) {
    this->setPosition(startPosition);
    this->setVisible(true);
    initAnimation();
    // Define target position off-screen at the bottom
    Vec2 endPosition = Vec2(startPosition.x, -SpriteController::calculateScreenRatio(Constants::FALLINGROCK_ITEMS_OFFSET));

    // Calculate distance and duration based on speed
    float distance = startPosition.distance(endPosition);
    float duration = distance / _speed;

    // Move down action
    auto moveDown = MoveTo::create(duration, endPosition);

    // Callback to remove rock when it moves off-screen
    auto removeRock = CallFunc::create([this]() {
        this->returnToPool();
        });

    // Run move action and remove when done
    this->runAction(Sequence::create(moveDown, removeRock, nullptr));
}

void FallingRock::returnToPool() {
    this->setVisible(false);
    this->stopAllActions();
    this->removeFromParentAndCleanup(false);
    FallingRockPool::getInstance()->returnEnemy(this);
}