#include "EnemyPlaneBoss.h"
#include "Manager/ObjectPoolGame3.h"
#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "Controller/SpriteController.h"
#include "utils/PhysicsShapeCache.h"
#include "Game3/Items/ItemPoolGane3.h"
#include "FX/Explodable.h"
#include "cocos2d.h"

USING_NS_CC;

EnemyPlaneBoss* EnemyPlaneBoss::create() {
    EnemyPlaneBoss* enemy = new (std::nothrow) EnemyPlaneBoss();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlaneBoss::init() {
    if (!EnemyPlaneBase::init()) {
        return false;
    }

    initAnimation();

    // Initialize health
    Constants::CurrentHealthEnemyPlaneBoss = 0;

    // Initialize visibleSize and origin
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    // Initialize phase
    currentPhase = Phase::StartPHASE;

    // Initialize warning sprite
    _warningSprite = Sprite::createWithSpriteFrameName("warning_rocket1.png"); // Đường dẫn tới hình ảnh cảnh báo
    _warningSprite->setVisible(false);
    this->addChild(_warningSprite, Constants::ORDER_LAYER_CHARACTER + 3);

    return true;
}

float EnemyPlaneBoss::getHealth() {
    return Constants::CurrentHealthEnemyPlaneBoss;
}

void EnemyPlaneBoss::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/EnemyPlaneBoss.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode, Constants::ORDER_LAYER_CHARACTER);
    }

    modelCharac = Sprite::createWithSpriteFrameName("EnemyPlaneBoss1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.7f)); // Adjust scale as needed
    spriteBatchNode->addChild(modelCharac, Constants::ORDER_LAYER_CHARACTER);

    auto animateCharac = Animate::create(SpriteController::createAnimation("EnemyPlaneBoss", 3, 0.1f)); // Adjust frame count and duration as needed
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBoss::spawnEnemy(float timeToUltimate) {
    this->reset(); // Reset the boss state
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Set initial position to the top of the screen
    this->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height + this->getContentSize().height / 2));

    // Set initial scale to a smaller value
    this->setScale(0.1f);

    // Create a move action to move the enemy to the upper region of the screen
    auto moveToUpperRegion = MoveTo::create(Constants::moveToUpperRegion, Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.8f)); // Adjust duration and target position as needed

    // Create a scale action to scale the enemy to its original size
    auto scaleToOriginalSize = ScaleTo::create(Constants::scaleToOriginalSize, 1.0f); // Adjust duration as needed

    // Create a spawn action to run both move and scale actions simultaneously
    auto spawnAction = Spawn::create(moveToUpperRegion, scaleToOriginalSize, nullptr);

    // Run the spawn action and then start moving left and right
    this->runAction(Sequence::create(spawnAction, CallFunc::create([this, timeToUltimate]() {
        // Dispatch event to show the boss health bar
        __NotificationCenter::getInstance()->postNotification("SHOW_BOSS_HEALTH_BAR");
        Constants::TimeToUltimate = timeToUltimate;
        this->graduallyIncreaseHealth();
        }), nullptr));
}

void EnemyPlaneBoss::graduallyIncreaseHealth() {
    Constants::CurrentHealthEnemyPlaneBoss = 0;
    float targetHealth = Constants::HealthEnemyPlaneBoss;
    float duration = Constants::DurationGraduallyHealthBoss; // Desired duration to fill the health bar (in seconds)
    float increment = targetHealth / (duration / 0.02f); // Calculate increment based on duration

    SoundController::getInstance()->playSoundEffect(Constants::BossHealthSFX);

    // Logic to gradually increase health to targetHealth
    this->schedule([this, targetHealth, increment](float dt) {
        if (Constants::CurrentHealthEnemyPlaneBoss < targetHealth) {
            Constants::CurrentHealthEnemyPlaneBoss += increment; // Increment health
            if (Constants::CurrentHealthEnemyPlaneBoss > targetHealth) {
                Constants::CurrentHealthEnemyPlaneBoss = targetHealth;
            }
            __NotificationCenter::getInstance()->postNotification("UPDATE_BOSS_HEALTH_BAR");
        }
        else {
            __NotificationCenter::getInstance()->postNotification("SHOW_ULTIMATE_SKILL_BADGE");
            
            __NotificationCenter::getInstance()->postNotification("HANDLE_ULTIMATE_SKILL_BADGE");
            this->unschedule("graduallyIncreaseHealth");
            this->createPhysicsBody();
            this->moveLeftRight();
            this->executePhaseSkills();
            this->executeUltimateSkill(Constants::TimeToUltimate);
        }
        }, 0.02f, "graduallyIncreaseHealth"); // Adjust the interval as needed
}


void EnemyPlaneBoss::moveLeftRight() {
    // Define the left and right positions
    auto moveLeft = MoveTo::create(3.0f, Vec2(origin.x + visibleSize.width * 0.35f, this->getPositionY()));
    auto moveRight = MoveTo::create(3.0f, Vec2(origin.x + visibleSize.width * 0.65f, this->getPositionY()));

    // Create a sequence to move left and right repeatedly
    auto moveSequence = Sequence::create(moveRight, moveLeft, nullptr);
    auto repeatMove = RepeatForever::create(moveSequence);

    // Run the repeat move action
    this->runAction(repeatMove);
}

void EnemyPlaneBoss::reset() {
    this->unschedule("UltimateSkillSchedule");
    this->unschedule("drop_booms_key");
    this->unschedule("launch_missiles_key");
    this->stopAllActions();
    this->setVisible(true);
    this->setScale(1.0f); // Reset scale to the original size
    Constants::CurrentHealthEnemyPlaneBoss = 0;
}

Size EnemyPlaneBoss::GetSize() {
    return SpriteController::GetContentSize(modelCharac);
}

void EnemyPlaneBoss::takeDamage(float damage) {
    Constants::CurrentHealthEnemyPlaneBoss -= damage;
    if (Constants::CurrentHealthEnemyPlaneBoss <= 0) {
        // Update phase before moving up and returning to the pool
        if (currentPhase == Phase::PHASE_1) {
            moveUpAndReturnToPool();
        }
        else if (currentPhase == Phase::PHASE_2) {
            handleBossFinisher();
        }
    }
}

void EnemyPlaneBoss::handleBossFinisher() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    this->unschedule("UltimateSkillSchedule");
    this->unschedule("drop_booms_key");
    this->unschedule("launch_missiles_key");
    this->stopAllActions();

    this->startExplosions();


    // Return active weapons to the pool
    auto boomPool = BoomForEnemyPlanePool::getInstance();
    for (auto& child : this->getParent()->getChildren()) {
        auto boom = dynamic_cast<BoomForEnemyPlane*>(child);
        if (boom && boom->isVisible()) {
            boom->setVisible(false);
            boomPool->returnObject(boom);
        }
    }

    auto missilemPool = MissileForEnemyPlanePool::getInstance();
    for (auto& child : this->getParent()->getChildren()) {
        auto missile = dynamic_cast<MissileForEnemyPlane*>(child);
        if (missile && missile->isVisible()) {
            missile->setVisible(false);
            missilemPool->returnObject(missile);
        }
    }
    auto delayTime = DelayTime::create(5.0f);

    // Create a sequence to move to center X, then run the move and scale actions, then return to pool
    auto sequence = Sequence::create(
        CallFunc::create([this]() {
            this->fadeOutAndRemove();
        })
        ,
        CallFunc::create([this]() {
            // Dispatch event to hide the boss health bar
            __NotificationCenter::getInstance()->postNotification("HIDE_BOSS_HEALTH_BAR");
            __NotificationCenter::getInstance()->postNotification("HIDE_ULTIMATE_SKILL_BADGE");
            })
        ,
        delayTime,
        CallFunc::create([this]() {
            this->isExploding = false; // Stop explosions
            //this->removeFromParentAndCleanup(false);
            EnemyPlaneBossPool::getInstance()->returnObject(this);
        }), 
        CallFunc::create([this]() {
            __NotificationCenter::getInstance()->postNotification("FINISH_AND_CHANGE_SCENE");
            }),
        nullptr);

    // Run the sequence action
    this->runAction(sequence);
}

void EnemyPlaneBoss::fadeOutAndRemove() {
    auto fadeOut = FadeOut::create(5.0f); // Adjust duration as needed
    auto removeSelf = RemoveSelf::create();
    auto sequence = Sequence::create(fadeOut, removeSelf, nullptr);
    this->runAction(sequence);
}

void EnemyPlaneBoss::updatePhase() {
    // Increment the phase
    if (currentPhase == Phase::PHASE_1) {
        Constants::HealthEnemyPlaneBoss = 700;
        SoundController::getInstance()->playMusic(Constants::pathSoundBossGame3Phase2, false);
        currentPhase = Phase::PHASE_2;
    }
    else {
        currentPhase = Phase::PHASE_1;
        Constants::HealthEnemyPlaneBoss = 1700;
        SoundController::getInstance()->playMusic(Constants::pathSoundBossGame3Phase1, false);
    }
}

void EnemyPlaneBoss::executePhaseSkills() {
    switch (currentPhase) {
    case Phase::PHASE_1:
        // Execute skills for phase 1
        this->dropBooms();
        this->schedule([this](float dt) {
            this->dropBooms();
            }, 3.5f, "drop_booms_key");
        break;
    case Phase::PHASE_2:
        // Execute skills for phase 2
        this->launchMissiles();
        this->schedule([this](float dt) {
            this->launchMissiles();
            }, 5.0f, "launch_missiles_key");
        break;
    case Phase::PHASE_3:
        // Execute skills for phase 3 with increased power and frequency
        this->schedule([this](float dt) {
            this->launchMissiles();
            }, 3.0f, "launch_missiles_key");
        break;
    }
}


void EnemyPlaneBoss::dropBooms() {
    auto boomPool = BoomForEnemyPlanePool::getInstance();

    // Calculate positions using the Ratio method
    float offsetX = SpriteController::calculateScreenRatio(0.15f); // Increase the ratio for wider X distance
    float offsetY = SpriteController::calculateScreenRatio(0.07f); // Decrease the ratio for lower Y position

    Vec2 positions[4] = {
        Vec2(this->getPositionX() - 1.5 * offsetX, this->getPositionY() - offsetY), // Left far
        Vec2(this->getPositionX() - offsetX, this->getPositionY() - offsetY), // Left near
        Vec2(this->getPositionX() + offsetX, this->getPositionY() - offsetY), // Right near
        Vec2(this->getPositionX() + 1.5 * offsetX, this->getPositionY() - offsetY) // Right far
    };

    for (const auto& pos : positions) {
        auto boom = boomPool->getObject();
        if (boom) {
            boom->setPosition(pos);
            if (boom->getParent() != nullptr) {
                boom->removeFromParent();
            }
            this->getParent()->addChild(boom);
            boom->setVisible(true);
            boom->moveDown(pos.x < this->getPositionX());
        }
    }
}

void EnemyPlaneBoss::moveUpAndReturnToPool() {
    this->unschedule("UltimateSkillSchedule");
    this->unschedule("drop_booms_key");
    this->unschedule("launch_missiles_key");

    // Stop all actions to prevent further movement
    this->stopAllActions();

    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }
   
    this->startExplosions();


    // Return active weapons to the pool
    auto boomPool = BoomForEnemyPlanePool::getInstance();
    for (auto& child : this->getParent()->getChildren()) {
        auto boom = dynamic_cast<BoomForEnemyPlane*>(child);
        if (boom && boom->isVisible()) {
            boom->setVisible(false);
            boomPool->returnObject(boom);
        }
    }

    auto missilemPool = MissileForEnemyPlanePool::getInstance();
    for (auto& child : this->getParent()->getChildren()) {
        auto missile = dynamic_cast<MissileForEnemyPlane*>(child);
        if (missile && missile->isVisible()) {
            missile->setVisible(false);
            missilemPool->returnObject(missile);
        }
    }

    // Create a move action to move the boss to the center of the screen on the X axis
    auto moveToCenterX = MoveTo::create(1.0f, Vec2(origin.x + visibleSize.width / 2, this->getPositionY()));

    // Create a move action to move the boss up and out of the screen (higher than before)
    auto moveToTop = MoveTo::create(3.0f, Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height + this->GetSize().height * 2));

    // Create a scale action to scale the boss down to a smaller size
    auto scaleDown = ScaleTo::create(3.0f, 0.1f);

    // Create a spawn action to run both move and scale actions simultaneously
    auto moveAndScale = Spawn::create(scaleDown, moveToTop, nullptr);

    // Create a sequence to move to center X, then run the move and scale actions, then return to pool
    auto sequence = Sequence::create(moveToCenterX, moveAndScale, 
        CallFunc::create([this]() {
            // Dispatch event to hide the boss health bar
                __NotificationCenter::getInstance()->postNotification("HIDE_BOSS_HEALTH_BAR");
                __NotificationCenter::getInstance()->postNotification("HIDE_ULTIMATE_SKILL_BADGE");
            })
        ,
        CallFunc::create([this]() {
            this->isExploding = false; // Stop explosions
            //this->removeFromParentAndCleanup(false);
            EnemyPlaneBossPool::getInstance()->returnObject(this);
        }), nullptr);

    // Run the sequence action
    this->runAction(sequence);
}

void EnemyPlaneBoss::startExplosions() {
    this->isExploding = true;

    float delayBetweenExplosions = 0.1f; // Adjust the delay between explosions as needed
   
    if (currentPhase == Phase::PHASE_1) {
        dropUpgradeItem();
    }

    this->schedule([this](float dt) {
        SoundController::getInstance()->playSoundEffect(Constants::EnemyCrepExplodeSFX);
        }, 0.9, "sound_explode");

    this->schedule([this](float dt) {
        if (this->isExploding) {
            float randomX = random(-this->GetSize().width / 2, this->GetSize().width / 2);
            float randomY = random(-this->GetSize().height / 2.7, this->GetSize().height / 2.7);
            auto explosion = Explosion::create(Vec2(randomX, randomY), nullptr); // Adjust scale as needed
            this->addChild(explosion, Constants::ORDER_LAYER_CHARACTER + 1);
        }
        else {
            this->unschedule("explosion_schedule_key");
            this->unschedule("sound_explode");
        }
        }, delayBetweenExplosions, "explosion_schedule_key");
}

void EnemyPlaneBoss::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBodyFromPlist("physicsBody/EnemyPlaneBoss.plist", "EnemyPlaneBoss", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "EnemyPlaneBoss", originalSize, 1.40f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

void EnemyPlaneBoss::dropUpgradeItem() {
    auto upgradeItem = UpgradeBulletItemPool::getInstance()->getItem();
    if (upgradeItem) {
        upgradeItem->setStartPosition(this->getPosition());
        upgradeItem->moveDown();
        this->getParent()->addChild(upgradeItem);
    }
}

void EnemyPlaneBoss::launchMissiles() {
    auto missilePool = MissileForEnemyPlanePool::getInstance();

    // Calculate positions using the Ratio method
    float offsetX = SpriteController::calculateScreenRatio(0.15f); // Increase the ratio for wider X distance
    float offsetY = SpriteController::calculateScreenRatio(0.07f); // Decrease the ratio for lower Y position

    Vec2 positions[4] = {
        Vec2(this->getPositionX() - 1.5 * offsetX, this->getPositionY() - offsetY), // Left far
        Vec2(this->getPositionX() - offsetX, this->getPositionY() - offsetY), // Left near
        Vec2(this->getPositionX() + offsetX, this->getPositionY() - offsetY), // Right near
        Vec2(this->getPositionX() + 1.5 * offsetX, this->getPositionY() - offsetY) // Right far
    };

    for (const auto& pos : positions) {
        auto missile = missilePool->getObject();
        if (missile) {
            missile->setPosition(pos);
            if (missile->getParent() != nullptr) {
                missile->removeFromParent();
            }
            this->getParent()->addChild(missile,Constants::ORDER_LAYER_PLAYER + 1);
            missile->setVisible(true);
            missile->moveDown(pos.x < this->getPositionX());
        }
    }
}

void EnemyPlaneBoss::showWarningSign() {
    _warningSprite->setVisible(true);
    _warningSprite->setPosition(modelCharac->getPosition());

    // Create blinking effect
    auto blink = Blink::create(2.0f, 6); // Blink for 2 seconds, 6 times
    _warningSprite->runAction(blink);
}

void EnemyPlaneBoss::hideWarningSign() {
    _warningSprite->setVisible(false);
    _warningSprite->stopAllActions();
}

void EnemyPlaneBoss::executeUltimateSkill(float timeToUltimate) {
    this->schedule([this](float dt) {
        if (this->getPhysicsBody() != nullptr) {
            this->removeComponent(this->getPhysicsBody());
        }
        this->unschedule("drop_booms_key");
        this->unschedule("launch_missiles_key");
        this->stopAllActions();

        // Move to the center and remove physics body
        auto moveToCenterX = MoveTo::create(1.0f, Vec2(origin.x + visibleSize.width / 2, this->getPositionY()));

        // Show warning sprite
        auto showWarningSprite = CallFunc::create([this]() {
            this->showWarningSign();
            });

        auto hideWarningSprite = CallFunc::create([this]() {
            this->hideWarningSign();
            });

        // Launch finisher missiles
        auto launchFinisher = CallFunc::create([this]() {
            this->launchFinisherMissiles();
            _warningSprite->setVisible(false); // Hide the warning sprite after launching missiles
            });

        // Create a sequence of actions
        auto sequence = Sequence::create(showWarningSprite, moveToCenterX,  DelayTime::create(0.5f), hideWarningSprite, launchFinisher, nullptr);
        this->runAction(sequence);
    }, timeToUltimate, "UltimateSkillSchedule");
}


void EnemyPlaneBoss::launchFinisherMissiles() {
    this->unschedule("UltimateSkillSchedule");

    auto missilePool = FinisherMissilesPool::getInstance();

    // Calculate positions using the Ratio method
    float offsetX = SpriteController::calculateScreenRatio(0.15f); // Increase the ratio for wider X distance
    float offsetY = SpriteController::calculateScreenRatio(0.07f); // Decrease the ratio for lower Y position

    Vec2 positions[4] = {
        Vec2(this->getPositionX() - 1.5 * offsetX, this->getPositionY() - offsetY), // Left far
        Vec2(this->getPositionX() - offsetX, this->getPositionY() - offsetY), // Left near
        Vec2(this->getPositionX() + offsetX, this->getPositionY() - offsetY), // Right near
        Vec2(this->getPositionX() + 1.5 * offsetX, this->getPositionY() - offsetY) // Right far
    };

    float durationSkill = 0.3f; // Adjust the delay between each launch as needed

    for (int i = 0; i < 6; i++) {
        this->scheduleOnce([this, missilePool, positions](float dt) {
        for (const auto& pos : positions) {
                auto missile = missilePool->getObject();
                if (missile) {
                    missile->setPosition(pos);
                    if (missile->getParent() != nullptr) {
                        missile->removeFromParent();
                    }
                    this->getParent()->addChild(missile);
                    missile->setVisible(true);
                    missile->moveDown(pos.x > this->getPositionX());
                }
        }
            }, i* durationSkill, "launchFinisherMissilesKey" + std::to_string(i));
    }
}

Phase EnemyPlaneBoss::getCurrentPhase() {
    return currentPhase;
}