// MeleeEnemy.cpp
#include "Game2/Enemy/Enemyh/MeleeEnemy.h"
#include "Constants/Constants.h"
#include "utils/MathFunction.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Enemy/EnemyUtils.h"

USING_NS_CC;

MeleeEnemy::MeleeEnemy()
    : _velocity(Vec2::ZERO), _isMoving(false)
{
}

MeleeEnemy::~MeleeEnemy()
{
}

bool MeleeEnemy::init()
{
    if (!EnemyBase::init())
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

    this->setScale(Constants::EnemyScale);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    this->setTag(Constants::EnemyTag); // Set the tag for the enemy

    // Create animations
    createIdleAnimation();
    createAttackAnimation();
    createDeathAnimation();

    // Schedule update method
    this->scheduleUpdate();

    // Set up collision detection
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MeleeEnemy::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    //CCLOG("MeleeEnemy initialized successfully");
    return true;
}

void MeleeEnemy::createIdleAnimation()
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

void MeleeEnemy::createAttackAnimation()
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

void MeleeEnemy::createDeathAnimation()
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

void MeleeEnemy::update(float delta)
{
    //CCLOG("MeleeEnemy update called");
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

void MeleeEnemy::moveToPlayer()
{
    EnemyUtils::moveToPlayer(this, _speed, _isMoving, _idleAnimation);
}

void MeleeEnemy::attackPlayer()
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

void MeleeEnemy::die()
{
    _isDead = true;
    this->runAction(Sequence::create(_deathAnimation, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr));
}

bool MeleeEnemy::onContactBegin(PhysicsContact& contact)
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
