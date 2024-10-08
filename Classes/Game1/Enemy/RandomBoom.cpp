#include "RandomBoom.h"
#include "cocos2d.h"

USING_NS_CC;

bool RandomBoom::init() {
    if (!Enemy::init()) {
        return false;
    }

    // Load sprite frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/warning_rocket.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/rocket.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");

    // Create sprite batch nodes
    _spriteBatchNodeWarning = SpriteBatchNode::create("assets_game/enemies/warning_rocket.png");
    _spriteBatchNodeMissile = SpriteBatchNode::create("assets_game/enemies/rocket.png");
    _spriteBatchNodeExplosion = SpriteBatchNode::create("assets_game/fx/explosions.png");

    // Add batch nodes to the current node
    this->addChild(_spriteBatchNodeWarning);
    this->addChild(_spriteBatchNodeMissile);
    this->addChild(_spriteBatchNodeExplosion);

    return true;
}

void RandomBoom::update(float delta) {

}

void RandomBoom::spawn(const Vec2& startPosition) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float restrictedWidth = 100.0f;
    float restrictedHeight = visibleSize.height - 100.0f;
    float centerX = visibleSize.width / 2;
    float halfRestrictedWidth = restrictedWidth / 2;
    float minX = centerX - halfRestrictedWidth;
    float maxX = centerX + halfRestrictedWidth;
    Vec2 warningPosition = Vec2(rand() % int(maxX - minX) + minX, rand() % int(restrictedHeight) + 50);

    showWarning(warningPosition);

    this->scheduleOnce([this, warningPosition](float) {
        launchMissile(warningPosition);
        }, 2.0f, "missile_launch_key");
}

void RandomBoom::showWarning(const Vec2& position) {
    if (!_warningSprite) {
        _warningSprite = Sprite::createWithSpriteFrameName("warning_rocket1.png");
        _warningSprite->setScale(SpriteController::updateSpriteScale(_warningSprite, 0.14f));
        _spriteBatchNodeWarning->addChild(_warningSprite);
    }
    _warningSprite->setPosition(position);

    auto warningAnimation = createAnimation("warning_rocket", 4, 0.15f);
    if (warningAnimation) {
        auto animateWarning = Animate::create(warningAnimation);
        _warningSprite->runAction(RepeatForever::create(animateWarning));
    }
}

void RandomBoom::launchMissile(const Vec2& targetPosition) {
    if (!_missileSprite) {
        _missileSprite = Sprite::createWithSpriteFrameName("rocket1.png");
        _missileSprite->setScale(SpriteController::updateSpriteScale(_missileSprite, 0.15f));
        _spriteBatchNodeMissile->addChild(_missileSprite);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 startPosition = (rand() % 2 == 0) ? Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.9f) : Vec2(visibleSize.width * 0.9f, visibleSize.height * 0.9f);
    _missileSprite->setPosition(startPosition);

    Vec2 direction = targetPosition - startPosition;
    float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));
    _missileSprite->setRotation(-angle - 90);

    float missileSpeed = 1.0f;
    auto moveToTarget = MoveTo::create(missileSpeed, targetPosition);
    auto hitTargetCallback = CallFunc::create([this]() {
        this->onMissileHitTarget();
        });

    _missileSprite->runAction(Sequence::create(moveToTarget, hitTargetCallback, nullptr));
}

Size RandomBoom::GetSize() {
    return GetContentSizeSprite(_warningSprite);
}


void RandomBoom::onMissileHitTarget() {
    if (!_warningSprite || !_missileSprite) {
        CCLOG("Warning sprite or missile sprite is null. Cannot create explosion.");
        return;
    }
    Vec2 position = _missileSprite->getPosition();

    // Cleanup missile and warning sprites
    if (_missileSprite) {
        _missileSprite->removeFromParentAndCleanup(true);
        _missileSprite = nullptr;
    }

    if (_warningSprite) {
        _warningSprite->removeFromParentAndCleanup(true);
        _warningSprite = nullptr;
    }

    if (!explosionSprite) {
        explosionSprite = Sprite::createWithSpriteFrameName("explosions7.png");
        explosionSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        explosionSprite->setScale(SpriteController::updateSpriteScale(explosionSprite, 0.13f));
        Size reducedSize = Size(GetContentSizeSprite(explosionSprite).width * 0.75, GetContentSizeSprite(explosionSprite).height * 0.70);
        auto explosionBody = PhysicsBody::createBox(reducedSize);
        explosionBody->setCollisionBitmask(0x02); // Unique bitmask for missiles
        explosionBody->setContactTestBitmask(true);
        explosionBody->setGravityEnable(false);
        explosionBody->setDynamic(false);
        explosionSprite->setPhysicsBody(explosionBody);
        explosionSprite->setPosition(position);
        _spriteBatchNodeExplosion->addChild(explosionSprite);
    }

    auto explosionAnimation = createAnimation("explosions", 10, 0.07f);
    auto animate = Animate::create(explosionAnimation);

    explosionSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([this]() {
            explosionSprite->removeFromParentAndCleanup(true);
            }),
        CallFunc::create([this]() {
            this->removeFromParentAndCleanup(true);
            }),
        nullptr
    ));
}

RandomBoom::~RandomBoom() {
    // Remove sprite frames from cache when object is destroyed
    //SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("assets_game/enemies/warning_rocket.plist");
    //SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("assets_game/enemies/rocket.plist");
    //SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("assets_game/fx/explosions.plist");
}
