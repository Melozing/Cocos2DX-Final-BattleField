/*// InvEnemy.cpp
#include "Game2/Enemy/Enemyh/InvEnemy.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Enemy/EnemyUtils.h"
#include "Constants/Constants.h"

USING_NS_CC;

InvEnemy::InvEnemy()
    : _eyeSprite(nullptr), _isInvisible(true), _shootInterval(7.0f), _timeSinceLastShot(0.0f),
    _idleAnimation(nullptr), _shootAnimation(nullptr), _deathAnimation(nullptr), _isMoving(false)
{
}

InvEnemy::~InvEnemy()
{
    CC_SAFE_RELEASE(_idleAnimation);
    CC_SAFE_RELEASE(_shootAnimation);
    CC_SAFE_RELEASE(_deathAnimation);
}

bool InvEnemy::init()
{
    if (!EnemyBase::init())
    {
        return false;
    }

    // Load the sprite frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/inv-enemy.plist");

    // Initialize the sprite with the idle frame
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("idle_inv0.png");
    if (!spriteFrame)
    {
        CCLOG("Sprite frame 'IdleInv0.png' not found in the cache");
        return false;
    }

    if (!Sprite::initWithSpriteFrame(spriteFrame))
    {
        CCLOG("Failed to initialize sprite with sprite frame");
        return false;
    }

    this->setScale(Constants::EnemyScale);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    this->setTag(Constants::EnemyTag); // Set the tag for the enemy

    // Create animations
    createIdleAnimation();
    createAttackAnimation();
    createDeathAnimation();

    // Load the eye sprite
    _eyeSprite = Sprite::create("assets_game/effects/eye.png");
    if (_eyeSprite)
    {
        _eyeSprite->setVisible(false);
        this->addChild(_eyeSprite);
    }

    // Schedule update method
    this->scheduleUpdate();

    // Set up collision detection
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(InvEnemy::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    //CCLOG("InvEnemy initialized successfully");
    return true;
}

void InvEnemy::createIdleAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        sprintf(str, "idle_inv%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
    _idleAnimation = RepeatForever::create(Animate::create(animation));
    _idleAnimation->retain();
}

void InvEnemy::createAttackAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 9; i++)
    {
        sprintf(str, "Riffle%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
    _shootAnimation = Animate::create(animation);
    _shootAnimation->retain();
}

void InvEnemy::createDeathAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 1; i < 7; i++)
    {
        sprintf(str, "death%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
    _deathAnimation = Animate::create(animation);
    _deathAnimation->retain();
}

void InvEnemy::update(float delta)
{
    if (_isDead)
    {
        return;
    }

    _timeSinceLastShot += delta;

    if (_timeSinceLastShot >= _shootInterval)
    {
        shoot();
        _timeSinceLastShot = 0.0f;
    }

    if (_isInvisible)
    {
        moveRandomly();
    }
    else
    {
        moveToPlayer();
    }
}

void InvEnemy::shoot()
{
    _isInvisible = false;
    this->setVisible(true);
    showEyeSprite();
    this->runAction(Sequence::create(_shootAnimation, DelayTime::create(1.5f), CallFunc::create([this]() {
        hideEyeSprite();
        goInvisible();
        }), nullptr));
}

void InvEnemy::goInvisible()
{
    _isInvisible = true;
    this->setVisible(false);
}

void InvEnemy::moveRandomly()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float x = this->getPositionX() + (CCRANDOM_MINUS1_1() * 50);
    float y = this->getPositionY() + (CCRANDOM_MINUS1_1() * 50);

    x = std::max(origin.x, std::min(x, origin.x + visibleSize.width));
    y = std::max(origin.y, std::min(y, origin.y + visibleSize.height));

    this->setPosition(Vec2(x, y));

    // Update rotation to face the player
    updateRotationToPlayer();
}

void InvEnemy::moveToPlayer()
{
    EnemyUtils::moveToPlayer(this, _speed, _isMoving, _idleAnimation);
}

void InvEnemy::showEyeSprite()
{
    if (_eyeSprite)
    {
        _eyeSprite->setVisible(true);
    }
}

void InvEnemy::hideEyeSprite()
{
    if (_eyeSprite)
    {
        _eyeSprite->setVisible(false);
    }
}

bool InvEnemy::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if ((nodeA && nodeA->getTag() == Constants::BulletTag) || (nodeB && nodeB->getTag() == Constants::BulletTag))
    {
        this->die();
        return true;
    }

    return false;
}

void InvEnemy::die()
{
    _isDead = true;
    this->runAction(Sequence::create(_deathAnimation, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr));
}
*/