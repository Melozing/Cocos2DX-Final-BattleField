#include "FallingRock.h"
#include "FallingRockPool.h"

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
    if (!Node::init()) { // Assuming Node is the superclass
        return false;
    }
    // Load sprite frames for both rock and landmine
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/falling_rock.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/landmine.plist");

    // Create the sprite batch nodes
    _spriteBatchNodeRock = SpriteBatchNode::create("assets_game/enemies/falling_rock.png");
    _spriteBatchNodeLandmine = SpriteBatchNode::create("assets_game/enemies/landmine.png");

    this->addChild(_spriteBatchNodeRock);
    this->addChild(_spriteBatchNodeLandmine);

    // Randomly select either ROCK or LANDMINE
    _spriteType = (rand() % 2 == 0) ? SpriteType::ROCK : SpriteType::LANDMINE;

    // Initialize the animation based on the chosen sprite type
    initAnimation();
    // Schedule update to run every frame
    this->scheduleUpdate();

    return true;
}

void FallingRock::reset() {
    _spriteBatchNodeRock = SpriteBatchNode::create("assets_game/enemies/falling_rock.png");
    _spriteBatchNodeLandmine = SpriteBatchNode::create("assets_game/enemies/landmine.png");
}

Size FallingRock::GetSize() {
    return GetContentSizeSprite(_currentSprite);
}

void FallingRock::initAnimation() {
    std::string spriteFrameName;

    // Depending on the sprite type, set appropriate properties
    if (_spriteType == SpriteType::ROCK) {
        spriteFrameName = "falling_rock1.png";
        _animationDelay = 0.07f; // Animation delay for rock

        // Create the sprite and add it to the rock batch node
        _currentSprite = Sprite::createWithSpriteFrameName(spriteFrameName);
        _spriteBatchNodeRock->addChild(_currentSprite);
    }
    else {
        spriteFrameName = "landmine1.png";
        _animationDelay = 0.15f; // Animation delay for landmine

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

    // Define target position off-screen at the bottom
    Vec2 endPosition = Vec2(startPosition.x, -50);

    // Calculate distance and duration based on speed
    float distance = startPosition.distance(endPosition);
    float duration = distance / _speed;

    // Move down action
    auto moveDown = MoveTo::create(duration, endPosition);

    // Callback to remove rock when it moves off-screen
    auto removeRock = CallFunc::create([this]() {
        this->removeWhenOutOfScreen();
        });

    // Run move action and remove when done
    this->runAction(Sequence::create(moveDown, removeRock, nullptr));
}

void FallingRock::update(float delta) {
    Vec2 currentPosition = this->getPosition();
    Vec2 movement = Vec2(0, -_speed * delta); // Moving downwards

    this->setPosition(currentPosition + movement);

	this->removeWhenOutOfScreen();
}

void FallingRock::removeWhenOutOfScreen() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (this->getPosition().y < -this->getContentSize().height - 50.0f) {
        this->stopAllActions();
        this->removeFromParentAndCleanup(false);
        FallingRockPool::getInstance()->returnEnemy(this);
    }
}


FallingRock::~FallingRock() {
    // Remove sprite frames from cache when object is destroyed
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("assets_game/enemies/falling_rock.plist");
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("assets_game/enemies/landmine.plist");
}
