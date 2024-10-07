// MeleeEnemyGame2.cpp
#include "Game2/Enemy/MeleeEnemyGame2.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "Game2/Player/PlayerGame2.h"
#include "Bullet/Bullet.h"

USING_NS_CC;

MeleeEnemyGame2::MeleeEnemyGame2()
    : _velocity(Vec2::ZERO),
    _speed(Constants::EnemySpeed),
    _isMoving(false),
    _isAttacking(false),
    _isDead(false)
{
}

MeleeEnemyGame2::~MeleeEnemyGame2()
{
}

MeleeEnemyGame2* MeleeEnemyGame2::createMeleeEnemyGame2()
{
    MeleeEnemyGame2* enemy = new (std::nothrow) MeleeEnemyGame2();
    if (enemy && enemy->init())
    {
        enemy->autorelease();
        return enemy;
    }
    else
    {
        delete enemy;
        return nullptr;
    }
}

bool MeleeEnemyGame2::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // Load the sprite frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/melee-enemy.plist");

    // Initialize the sprite with the idle frame
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("IdleKnife0.png");
    if (!spriteFrame)
    {
        CCLOG("Sprite frame 'IdleKnife0.png' not found in the cache");
        return false;
    }

    if (!Sprite::initWithSpriteFrame(spriteFrame))
    {
        CCLOG("Failed to initialize sprite with sprite frame");
        return false;
    }

    this->setPosition(Vec2(Constants::InitialEnemyPosX, Constants::InitialEnemyPosY));
    this->setScale(Constants::EnemyScale);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    // Add PhysicsBody to the enemy
    auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setGravityEnable(false); // Disable gravity
    physicsBody->setDynamic(false);
    this->setPhysicsBody(physicsBody);


    this->setTag(Constants::EnemyTag); // Set the tag for the enemy

    // Create animations
    createIdleAnimation();
    createWalkAnimation();
    createAttackAnimation();
    createDeathAnimation();
    // Schedule update method
    this->scheduleUpdate();

    // Set up collision detection
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MeleeEnemyGame2::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    CCLOG("MeleeEnemyGame2 initialized successfully");
    return true;
}


void MeleeEnemyGame2::createIdleAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        sprintf(str, "IdleKnife%d.png", i);
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

void MeleeEnemyGame2::createWalkAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 6; i++)
    {
        sprintf(str, "Walkknife%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
    _walkAnimation = RepeatForever::create(Animate::create(animation));
    _walkAnimation->retain();
}

void MeleeEnemyGame2::createAttackAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        sprintf(str, "Knife_%03d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
    _attackAnimation = Animate::create(animation);
    _attackAnimation->retain();
}

void MeleeEnemyGame2::createDeathAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 6; i++)
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

void MeleeEnemyGame2::update(float delta)
{
    CCLOG("MeleeEnemyGame2 update called");
    if (_isDead)
    {
        return;
    }

    if (_isAttacking)
    {
        return;
    }

    moveToPlayer();
}

void MeleeEnemyGame2::updateRotationToPlayer()
{
    auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
    if (player)
    {
        Vec2 playerPos = player->getPosition();
        Vec2 pos = this->getPosition();
        Vec2 dirToPlayer = playerPos - pos;
        float angle = CC_RADIANS_TO_DEGREES(-dirToPlayer.getAngle());
        this->setRotation(angle+90);
    }
    else
    {
        CCLOG("Player not found");
    }
}


void MeleeEnemyGame2::moveToPlayer()
{
    auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
    if (player)
    {
        Vec2 playerPos = player->getPosition();
        Vec2 pos = this->getPosition();
        Vec2 dirToPlayer = playerPos - pos;
        dirToPlayer.normalize();
        _velocity = dirToPlayer * _speed;

        pos += _velocity * Director::getInstance()->getDeltaTime();
        this->setPosition(pos);

        // Update rotation to face the player
        updateRotationToPlayer();

        if (!_isMoving)
        {
            this->stopAllActions();
            this->runAction(_walkAnimation);
            _isMoving = true;
        }

        CCLOG("Enemy moving to player: (%f, %f)", pos.x, pos.y);

        if (pos.distance(playerPos) < Constants::AttackRange)
        {
            attackPlayer();
        }
    }
    else
    {
        CCLOG("Player not found");
    }
}





void MeleeEnemyGame2::attackPlayer()
{
    _isAttacking = true;
    this->runAction(Sequence::create(_attackAnimation, CallFunc::create([this]() {
        auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
        if (player)
        {
            player->die();
        }
        _isAttacking = false;
        }), nullptr));
}

void MeleeEnemyGame2::die()
{
    _isDead = true;
    this->runAction(Sequence::create(_deathAnimation, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr));
}

bool MeleeEnemyGame2::onContactBegin(PhysicsContact& contact)
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
