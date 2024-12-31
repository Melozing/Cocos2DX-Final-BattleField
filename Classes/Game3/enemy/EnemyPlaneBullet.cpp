#include "EnemyPlaneBullet.h"
#include "Manager/ObjectPoolGame3.h"
#include "utils/PhysicsShapeCache.h"
#include "Constants/Constants.h"

USING_NS_CC;

EnemyPlaneBullet* EnemyPlaneBullet::create() {
    EnemyPlaneBullet* enemy = new (std::nothrow) EnemyPlaneBullet();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlaneBullet::init() {
    if (!EnemyPlaneBase::init()) {
        return false;
    }

    initAnimation();

    // Initialize explosion

    // Initialize warning sign
    warningSign = Sprite::create("assets_game/enemies/WarningSignBulletPlane.png");
    warningSign->setScale(SpriteController::updateSpriteScale(modelCharac, 0.02f));
    warningSign->setVisible(false);
    this->addChild(warningSign);
    reset();
    return true;
}

Size EnemyPlaneBullet::GetSize() {
    return GetContentSizeSprite(modelCharac);
}

void EnemyPlaneBullet::initAnimation() {
    spriteBatchNode = SpriteBatchNode::create("assets_game/enemies/EnemyPlaneBullet.png");

    if (spriteBatchNode->getParent() == nullptr) {
        this->addChild(spriteBatchNode);
    }

    modelCharac = Sprite::createWithSpriteFrameName("EnemyPlaneBullet1.png");
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.13f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("EnemyPlaneBullet", 39, 0.033f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBullet::spawnEnemy(cocos2d::Node* parent, float skillTime, bool spawnWithSkill, const std::string& direction, const std::string& position) {
        this->createPhysicsBody();
        this->resetSprite();
        this->setVisible(true);

        auto visibleSize = Director::getInstance()->getVisibleSize();

        // Define fixed y-coordinates for spawning
        float lowY = visibleSize.height * 0.65f;
        float midY = visibleSize.height * 0.7f;
        float highY = visibleSize.height * 0.75f;

        float fixedY;
        if (position == "low") {
            fixedY = lowY;
        }
        else if (position == "mid") {
            fixedY = midY;
        }
        else {
            fixedY = highY;
        }

        if (direction == "Right") {
            this->setPosition(Vec2(-this->getContentSize().width / 2, fixedY));
            this->moveFromLeftToRight(visibleSize, Constants::EnemyPlaneBulletGame3Speed);
        }
        else {
            this->setPosition(Vec2(visibleSize.width + this->getContentSize().width / 2, fixedY));
            this->moveFromRightToLeft(visibleSize, Constants::EnemyPlaneBulletGame3Speed);
        }

        if (spawnWithSkill) {
            // Schedule to show warning sign and spawn bullets
            this->scheduleOnce([this](float) {
                this->showWarningSign();
                }, skillTime, "show_warning_sign_key");
        }
}


void EnemyPlaneBullet::showWarningSign() {
    warningSign->setVisible(true);
    warningSign->setPosition(modelCharac->getPosition());

    // Create blinking effect
    auto blink = Blink::create(2.0f, 6); // Blink for 2 seconds, 6 times
    warningSign->runAction(blink);

    // Schedule to spawn bullets after 2 seconds
    this->scheduleOnce([this](float) {
        this->spawnBullets();
        }, 2.0f, "spawn_bullets_key");
}

void EnemyPlaneBullet::hideWarningSign() {
    warningSign->stopAllActions();
    this->unschedule("show_warning_sign_key");
    warningSign->setVisible(false);
}

void EnemyPlaneBullet::spawnBullets() {
    this->hideWarningSign(); // Hide warning sign when spawning bullets

    bool movingFromLeft = this->getPositionX() < Director::getInstance()->getVisibleSize().width / 2;

    for (int i = 0; i < 3; ++i) {
        this->scheduleOnce([this, i, movingFromLeft](float) {
            auto bullet = BulletForEnemyPlanePool::getInstance()->getObject();
            if (bullet) {
                bullet->setPosition(this->getPosition());
                if (bullet->getParent() == nullptr) {
                    this->getParent()->addChild(bullet);
                }
                bullet->setVisible(true);
                float angle = movingFromLeft ? 200.0f + i * 5.0f : -200.0f - i * 5.0f; // Adjust angle for each bullet
                bullet->moveDown(angle);
            }
            }, i * 0.3f, "spawn_bullet_key_" + std::to_string(i));
    }
}

void EnemyPlaneBullet::createPhysicsBody() {
    if (this->getPhysicsBody() != nullptr) {
        this->removeComponent(this->getPhysicsBody());
    }

    auto physicsCache = PhysicsShapeCache::getInstance();
    physicsCache->addShapesWithFile("physicsBody/EnemyPlaneBullet.plist");

    auto originalSize = modelCharac->getTexture()->getContentSize();
    auto scaledSize = this->GetSize();

    auto physicsBody = physicsCache->createBody("EnemyPlaneBullet", originalSize, scaledSize);
    physicsCache->resizeBody(physicsBody, "EnemyPlaneBullet", originalSize, 0.9f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}

void EnemyPlaneBullet::returnToPool() {
    this->hideWarningSign();
    this->unschedule("spawn_bullets_key");
    this->setVisible(false);
    this->removeFromParentAndCleanup(false);
    this->reset();
    EnemyPlaneBulletPool::getInstance()->returnObject(this);
}
