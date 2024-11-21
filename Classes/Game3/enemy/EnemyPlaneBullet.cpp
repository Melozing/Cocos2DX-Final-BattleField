#include "EnemyPlaneBullet.h"
#include "EnemyPlaneBulletPool.h"
#include "BulletForEnemyPlanePool.h"
#include "utils/PhysicsShapeCache.h"
#include "Constants/Constants.h"

USING_NS_CC;

EnemyPlaneBullet* EnemyPlaneBullet::createEnemyBullet() {
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

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/EnemyPlaneBullet.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/fx/explosions.plist");

    initAnimation();
    createPhysicsBody();

    // Initialize explosion batch node
    explosionBatchNode = SpriteBatchNode::create("assets_game/fx/explosions.png");
    this->addChild(explosionBatchNode);

    // Initialize warning sign
    warningSign = Sprite::create("assets_game/enemies/WarningSignBulletPlane.png");
    warningSign->setScale(SpriteController::updateSpriteScale(modelCharac, 0.02f));
    warningSign->setVisible(false);
    this->addChild(warningSign);

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
    modelCharac->setScale(SpriteController::updateSpriteScale(modelCharac, 0.07f));
    spriteBatchNode->addChild(modelCharac);

    auto animateCharac = Animate::create(SpriteController::createAnimation("EnemyPlaneBullet", 7, 0.07f));
    modelCharac->runAction(RepeatForever::create(animateCharac));
}

void EnemyPlaneBullet::spawnEnemy(cocos2d::Node* parent) {
    auto enemy = EnemyPlaneBulletPool::getInstance()->getEnemy();
    if (enemy) {
        enemy->resetSprite();
        parent->addChild(enemy);
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // Define the range to spawn lower on the screen with a narrower range
        float lowerBuffer = visibleSize.height / 5; // Lower on the screen
        float randomY = random(visibleSize.height * 0.5f, visibleSize.height * 0.5f + lowerBuffer);

        bool spawnFromLeft = random(0, 1) == 0;

        if (spawnFromLeft) {
            enemy->setPosition(Vec2(-enemy->getContentSize().width / 2, randomY));
            enemy->moveFromLeftToRight(visibleSize, Constants::EnemyPlaneBulletGame3Speed);
        }
        else {
            enemy->setPosition(Vec2(visibleSize.width + enemy->getContentSize().width / 2, randomY));
            enemy->moveFromRightToLeft(visibleSize, Constants::EnemyPlaneBulletGame3Speed);
        }

        // Schedule to show warning sign and spawn bullets
        enemy->scheduleOnce([enemy](float) {
            enemy->showWarningSign();
            }, 2.0f, "show_warning_sign_key");
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
    warningSign->setVisible(false);
    warningSign->stopAllActions();
}

void EnemyPlaneBullet::spawnBullets() {
    hideWarningSign(); // Hide warning sign when spawning bullets

    bool movingFromLeft = this->getPositionX() < Director::getInstance()->getVisibleSize().width / 2;

    for (int i = 0; i < 3; ++i) {
        this->scheduleOnce([this, i, movingFromLeft](float) {
            auto bullet = BulletForEnemyPlanePool::getInstance()->getBullet();
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

void EnemyPlaneBullet::reset() {
    modelCharac->setVisible(true);
    this->setPosition(Vec2::ZERO);
    this->stopAllActions();
    createPhysicsBody();
}

void EnemyPlaneBullet::explode() {
    this->removeComponent(this->getPhysicsBody());

    if (!explosionSprite) {
        explosionSprite = Sprite::createWithSpriteFrameName("explosions7.png");
        explosionSprite->setScale(SpriteController::updateSpriteScale(explosionSprite, 0.078f));
        explosionBatchNode->addChild(explosionSprite);
    }

    explosionSprite->setPosition(modelCharac->getPosition());
    modelCharac->setVisible(false);
    explosionSprite->setVisible(true);

    auto explosionAnimation = SpriteController::createAnimation("explosions", 10, 0.041f);
    auto animate = Animate::create(explosionAnimation);

    explosionSprite->runAction(Sequence::create(
        animate,
        CallFunc::create([this]() {
            explosionSprite->setVisible(false);
            this->returnToPool();
            }),
        nullptr
    ));
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
    physicsCache->resizeBody(physicsBody, "EnemyPlaneBullet", originalSize, 0.5f);
    if (physicsBody) {
        physicsBody->setContactTestBitmask(true);
        physicsBody->setDynamic(false);
        physicsBody->setGravityEnable(false);

        this->setPhysicsBody(physicsBody);
    }
}
