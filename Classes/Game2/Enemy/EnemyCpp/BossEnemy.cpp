/*// BossEnemy.cpp
#include "Game2/Enemy/Enemyh/BossEnemy.h"
#include "Game2/Player/PlayerGame2.h"
#include "Game2/Enemy/EnemyUtils.h"
#include "Constants/Constants.h"

USING_NS_CC;

BossEnemy::BossEnemy()
    : _shootInterval(0.1f), _timeSinceLastShot(0.0f), _reloadTime(1.5f), _isMoving(false), _isReloading(false), _hasUsedSpecialSkill2(false), _maxHealth(200), _reloadSprite(nullptr)
{
    _health = _maxHealth;
    _speed = Constants::BossSpeed;
    _damage = Constants::BossDamage;
}

BossEnemy::~BossEnemy()
{
}

bool BossEnemy::init()
{
    if (!EnemyBase::init())
    {
        return false;
    }

    // Load the sprite frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/sniper-enemy.plist");

    // Initialize the sprite with the idle frame
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("idlegun0.png");
    if (!spriteFrame)
    {
        CCLOG("Sprite frame 'idle_boss0.png' not found in the cache");
        return false;
    }

    if (!Sprite::initWithSpriteFrame(spriteFrame))
    {
        CCLOG("Failed to initialize sprite with sprite frame");
        return false;
    }

    this->setScale(Constants::BossScale);
    this->setAnchorPoint(Vec2(0.5, 0.5));

    this->setTag(Constants::BossTag); // Set the tag for the boss

    // Create animations
    createIdleAnimation();
    createAttackAnimation();
    createDeathAnimation();

    // Schedule update method
    this->scheduleUpdate();

    // Set up collision detection
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(BossEnemy::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Create reload sprite
    _reloadSprite = Sprite::create("assets_game/effects/reload.png");
    if (_reloadSprite)
    {
        _reloadSprite->setVisible(false);
        this->addChild(_reloadSprite);
    }

    //CCLOG("BossEnemy initialized successfully");
    return true;
}

void BossEnemy::createIdleAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        sprintf(str, "idlegun%d.png", i);
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

void BossEnemy::createAttackAnimation()
{
    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        sprintf(str, "Gunshot%d.png", i);
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

void BossEnemy::createDeathAnimation()
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

void BossEnemy::update(float delta)
{
    if (_isDead)
    {
        return;
    }

    moveToPlayer();

    if (_isReloading)
    {
        _timeSinceLastShot += delta;
        if (_timeSinceLastShot >= _reloadTime)
        {
            _isReloading = false;
            _reloadSprite->setVisible(false);
            _timeSinceLastShot = 0.0f;
        }
    }
    else
    {
        _timeSinceLastShot += delta;
        if (_timeSinceLastShot >= _shootInterval)
        {
            shoot();
            _timeSinceLastShot = 0.0f;
        }
    }

    // Special skills
    if (_health <= _maxHealth / 2 && !_hasUsedSpecialSkill2)
    {
        specialSkill2();
        _hasUsedSpecialSkill2 = true;
    }

    // Update health UI
    updateHealthUI();
}

void BossEnemy::moveToPlayer()
{
    EnemyUtils::moveToPlayer(this, _speed, _isMoving, _idleAnimation);
}

void BossEnemy::shoot()
{
    // Implement shooting logic
    // After shooting 100 bullets, start reloading
    static int bulletsShot = 0;
    bulletsShot++;
    if (bulletsShot >= 100)
    {
        reload();
        bulletsShot = 0;
    }
}

void BossEnemy::reload()
{
    _isReloading = true;
    _reloadSprite->setVisible(true);
}

void BossEnemy::spawnEnemies(int count)
{
    // Implement logic to spawn enemies at the boss's position
}

void BossEnemy::specialSkill1()
{
    spawnEnemies(5);
}

void BossEnemy::specialSkill2()
{
    _health = _maxHealth;
    spawnEnemies(12);
    _damage += Constants::BossDamageIncrease;
}

void BossEnemy::updateHealthUI()
{
    // Implement logic to update health UI
}

bool BossEnemy::onContactBegin(PhysicsContact& contact)
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

void BossEnemy::die()
{
    _isDead = true;
    this->runAction(Sequence::create(_deathAnimation, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr));
}
*/