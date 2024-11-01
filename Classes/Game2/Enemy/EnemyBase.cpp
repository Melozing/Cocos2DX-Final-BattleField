// EnemyBase.cpp
#include "EnemyBase.h"
#include "Game2/Player/PlayerGame2.h"

USING_NS_CC;

EnemyBase::EnemyBase()
    : _health(100), _speed(Constants::EnemySpeed), _damage(10), _isDead(false), _isAttacking(false)
{
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::init()
{
    if (!Sprite::init())
    {
        return false;
    }
    _physicsBody = PhysicsBody::createBox(this->getContentSize());
    _physicsBody->setContactTestBitmask(true);
    _physicsBody->setDynamic(false); // Set to true if the enemy should be affected by physics
    _physicsBody->setCategoryBitmask(0x01);
    _physicsBody->setCollisionBitmask(0x01);
    _physicsBody->setContactTestBitmask(0x01);
    this->setPhysicsBody(_physicsBody);
    this->scheduleUpdate();
    return true;
}

void EnemyBase::update(float delta)
{
    if (_isDead)
    {
        return;
    }

    moveToPlayer();
}

void EnemyBase::die()
{
    /* _isDead = true;
     this->runAction(Sequence::create(createDeathAnimation(), CallFunc::create([this]() {
         this->removeFromParent();
         }), nullptr));*/
}

void EnemyBase::attack()
{
    /* _isAttacking = true;
     this->runAction(Sequence::create(createAttackAnimation(), CallFunc::create([this]() {
         _isAttacking = false;
         }), nullptr));*/
}

void EnemyBase::setHealth(int health)
{
    _health = health;
}

int EnemyBase::getHealth() const
{
    return _health;
}

void EnemyBase::setSpeed(float speed)
{
    _speed = speed;
}

float EnemyBase::getSpeed() const
{
    return _speed;
}

void EnemyBase::setDamage(int damage)
{
    _damage = damage;
}

int EnemyBase::getDamage() const
{
    return _damage;
}

void EnemyBase::updateRotationToPlayer()
{
    auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
    if (player)
    {
        Vec2 playerPos = player->getPosition();
        Vec2 pos = this->getPosition();
        Vec2 dirToPlayer = playerPos - pos;
        float angle = CC_RADIANS_TO_DEGREES(-dirToPlayer.getAngle());
        this->setRotation(angle + 90);
    }
    else
    {
        CCLOG("Player not found");
    }
}

void EnemyBase::takeDamage(int damage) {
    _health -= damage;
    if (_health <= 0) {
        die();
    }
}