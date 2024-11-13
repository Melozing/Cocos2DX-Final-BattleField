// EnemyBase.cpp
#include "EnemyBase.h"
#include "Game2/Player/PlayerGame2.h"

USING_NS_CC;

EnemyBase::EnemyBase()
    : _health(100), _speed(Constants::EnemySpeed), _damage(10), _attackRange(50.0f), _isDead(false), _isAttacking(false), _isMoving(false)
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

    createPhysicsBody();
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
    _isDead = true;
    auto animateCharac = Animate::create(createDeathAnimation());
    this->runAction(Sequence::create(animateCharac, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr));
}

void EnemyBase::attack()
{
    _isAttacking = true;
    auto animateCharac = Animate::create(createAttackAnimation());
    this->runAction(Sequence::create(animateCharac, CallFunc::create([this]() {
        _isAttacking = false;
        }), nullptr));
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

void EnemyBase::setAttackRange(float range)
{
    _attackRange = range;
}

float EnemyBase::getAttackRange() const
{
    return _attackRange;
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

void EnemyBase::moveToPlayer()
{
    EnemyUtils::moveToPlayer(this, _speed, _isMoving, createAnimation("walk", 5, 0.07f));
}

Size EnemyBase::GetSize() {
    return GetContentSizeSprite(this);
}

void EnemyBase::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsBody = PhysicsBody::createBox(this->GetSize());
    physicsBody->setContactTestBitmask(true);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    this->addComponent(physicsBody);
}

