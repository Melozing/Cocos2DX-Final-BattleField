//// SuicideBomberEnemy.cpp
//#include "Game2/Enemy/Enemyh/SuicideBomberEnemy.h"
//#include "Game2/Player/PlayerGame2.h"
//#include "Game2/Enemy/EnemyUtils.h"
//#include "Constants/Constants.h"
//
//USING_NS_CC;
//
//SuicideBomberEnemy::SuicideBomberEnemy()
//    : _explosionRadius(100.0f), _timeInRange(0.0f), _requiredTimeInRange(1.0f), _isExploding(false), _isMoving(false)
//{
//}
//
//SuicideBomberEnemy::~SuicideBomberEnemy()
//{
//}
//
//bool SuicideBomberEnemy::init()
//{
//    if (!EnemyBase::init())
//    {
//        return false;
//    }
//
//    // Load the sprite frames
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/player/bommer-enemy.plist");
//
//    // Initialize the sprite with the idle frame
//    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("idle_bomer0.png");
//    if (!spriteFrame)
//    {
//        CCLOG("Sprite frame 'idle_suicide_bomber0.png' not found in the cache");
//        return false;
//    }
//
//    if (!Sprite::initWithSpriteFrame(spriteFrame))
//    {
//        CCLOG("Failed to initialize sprite with sprite frame");
//        return false;
//    }
//
//    this->setScale(Constants::EnemyScale);
//    this->setAnchorPoint(Vec2(0.5, 0.5));
//
//    this->setTag(Constants::EnemyTag); // Set the tag for the enemy
//
//    // Create animations
//    createIdleAnimation();
//    createAttackAnimation();
//    createDeathAnimation();
//
//    // Schedule update method
//    this->scheduleUpdate();
//
//    // Set up collision detection
//    auto contactListener = EventListenerPhysicsContact::create();
//    contactListener->onContactBegin = CC_CALLBACK_1(SuicideBomberEnemy::onContactBegin, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
//
//    //CCLOG("SuicideBomberEnemy initialized successfully");
//    return true;
//}
//
//void SuicideBomberEnemy::createIdleAnimation()
//{
//    Vector<SpriteFrame*> animFrames;
//    char str[100] = { 0 };
//    for (int i = 0; i < 9; i++)
//    {
//        sprintf(str, "idle_bomer%d.png", i);
//        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
//        if (frame)
//        {
//            animFrames.pushBack(frame);
//        }
//    }
//
//    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
//    _idleAnimation = RepeatForever::create(Animate::create(animation));
//    _idleAnimation->retain();
//}
//
//void SuicideBomberEnemy::createAttackAnimation()
//{
//    // Implement attack animation if needed
//}
//
//void SuicideBomberEnemy::createDeathAnimation()
//{
//    Vector<SpriteFrame*> animFrames;
//    char str[100] = { 0 };
//    for (int i = 1; i < 7; i++)
//    {
//        sprintf(str, "death%d.png", i);
//        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
//        if (frame)
//        {
//            animFrames.pushBack(frame);
//        }
//    }
//
//    auto animation = Animation::createWithSpriteFrames(animFrames, Constants::AnimationFrameDelay);
//    _deathAnimation = Animate::create(animation);
//    _deathAnimation->retain();
//}
//
//void SuicideBomberEnemy::update(float delta)
//{
//    if (_isDead || _isExploding)
//    {
//        return;
//    }
//
//    moveToPlayer();
//
//    if (isPlayerInRange())
//    {
//        _timeInRange += delta;
//        if (_timeInRange >= _requiredTimeInRange)
//        {
//            explode();
//        }
//    }
//    else
//    {
//        _timeInRange = 0.0f;
//    }
//}
//
//void SuicideBomberEnemy::moveToPlayer()
//{
//    EnemyUtils::moveToPlayer(this, _speed, _isMoving, _idleAnimation);
//}
//
//bool SuicideBomberEnemy::isPlayerInRange()
//{
//    auto player = dynamic_cast<PlayerGame2*>(this->getParent()->getChildByName("PlayerGame2"));
//    if (player)
//    {
//        Vec2 playerPos = player->getPosition();
//        Vec2 pos = this->getPosition();
//        float distance = pos.distance(playerPos);
//        return distance <= _explosionRadius;
//    }
//    return false;
//}
//
//void SuicideBomberEnemy::explode()
//{
//    _isExploding = true;
//    this->runAction(Sequence::create(_deathAnimation, CallFunc::create([this]() {
//        this->removeFromParent();
//        }), nullptr));
//}
//
//bool SuicideBomberEnemy::onContactBegin(PhysicsContact& contact)
//{
//    auto nodeA = contact.getShapeA()->getBody()->getNode();
//    auto nodeB = contact.getShapeB()->getBody()->getNode();
//
//    if ((nodeA && nodeA->getTag() == Constants::BulletTag) || (nodeB && nodeB->getTag() == Constants::BulletTag))
//    {
//        this->die();
//        return true;
//    }
//
//    return false;
//}
//
//void SuicideBomberEnemy::die()
//{
//    _isDead = true;
//    this->runAction(Sequence::create(_deathAnimation, CallFunc::create([this]() {
//        this->removeFromParent();
//        }), nullptr));
//}
