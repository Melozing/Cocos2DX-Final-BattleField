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
    _health = 100;
    _damage = 10;
    _speed = Constants::EnemySpeed;
    _attackRange = 50.0f;
}

MeleeEnemy::~MeleeEnemy()
{
    CC_SAFE_RELEASE(_idleAnimation);
    CC_SAFE_RELEASE(_attackAnimation);
    CC_SAFE_RELEASE(_deathAnimation);
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

    // Create animations
    createIdleAnimation();
    createAttackAnimation();
    createDeathAnimation();

    // Schedule update method
    this->scheduleUpdate();
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
    _attackAnimation = RepeatForever::create(Animate::create(animation));
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
    if (_isDead)
    {
        return;
    }

    if (_isAttacking)
    {
        return;
    }

    auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
    if (player)
    {
        float distanceToPlayer = this->getPosition().distance(player->getPosition());
        if (distanceToPlayer <= _attackRange)
        {
            attackPlayer();
        }
        else
        {
            moveToPlayer();
        }
    }
}

void MeleeEnemy::attackPlayer()
{
    _isAttacking = true;
    this->runAction(Sequence::create(_attackAnimation, CallFunc::create([this]() {
        auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
        if (player)
        {
            player->takeDamage(_damage); // Gây sát thương cho người chơi
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
