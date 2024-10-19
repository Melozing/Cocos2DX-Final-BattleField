// EnemyPlane1.cpp

#include "EnemyPlane1.h"
#include "Constants/Constants.h"

USING_NS_CC;

EnemyPlane1::EnemyPlane1()
    : modelCharac(nullptr)
{
}

EnemyPlane1::~EnemyPlane1()
{
}

EnemyPlane1* EnemyPlane1::createEnemyPlane1()
{
    EnemyPlane1* enemy = new (std::nothrow) EnemyPlane1();
    if (enemy && enemy->init())
    {
        enemy->autorelease();
        enemy->initAnimation();
        enemy->startMoving();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlane1::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // Get the screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Set position to the center of the screen
    this->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->setScale(3.0f); // Example scale
    this->moveAndReturn(visibleSize);

    return true;
}



void EnemyPlane1::initAnimation()
{
    // Load sprite frames from .plist file
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets_game/enemies/Plane_enemy_bom.plist");

    // Create sprite with initial frame
    modelCharac = Sprite::createWithSpriteFrameName("Plane_enemy_bom1.png");
    if (modelCharac)
    {
        this->addChild(modelCharac);

        // Create animation using inherited method from SpriteController
        auto animateCharac = Animate::create(createAnimation("Plane_enemy_bom", 8, 0.07f));

        // Create a repeat action to loop the animation indefinitely
        auto repeatAction = RepeatForever::create(animateCharac);

        // Run the repeat action
        modelCharac->runAction(repeatAction);

        if (this->getPositionX() < Director::getInstance()->getVisibleSize().width / 2)
        {
            // Moving from right to left, flip the sprite
            modelCharac->setFlippedX(true);
        }
        else
        {
            // Moving from left to right, reset the flip
            modelCharac->setFlippedX(false);
        }
        //// Schedule a function to check the direction and flip the sprite if necessary
        //this->schedule([this](float) {
        //    
        //    }, "check_direction_key");
    }
    else
    {
        CCLOG("Failed to create sprite with frame 'Plane_enemy_bom1.png'");
    }
}

void EnemyPlane1::startMoving()
{
    // Get the screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Set initial position to the left side of the screen with random y position
    float startX = origin.x - this->getContentSize().width / 2;
    float startY = origin.y + visibleSize.height / 2 + CCRANDOM_0_1() * (visibleSize.height / 2);
    this->setPosition(Vec2(startX, startY));

    // Calculate the end position outside the right side of the screen
    Vec2 endPositionRight = Vec2(origin.x + visibleSize.width + this->getContentSize().width / 2, startY);

    // Calculate the duration for moving from left to right based on speed
    float distanceLeftToRight = endPositionRight.x - startX;
    float moveDurationLeftToRight = distanceLeftToRight / Constants::SpeedEnemyPlane1;

    // Create the action to move from left to right
    auto moveRight = MoveTo::create(moveDurationLeftToRight, endPositionRight);

    // Create the action to flip the sprite horizontally
    auto flipLeft = CallFunc::create([this]() {
        modelCharac->setFlippedX(true);
        });

    // Calculate the end position outside the left side of the screen
    Vec2 endPositionLeft = Vec2(origin.x - this->getContentSize().width / 2, startY);

    // Calculate the duration for moving from right to left based on speed
    float distanceRightToLeft = endPositionRight.x - endPositionLeft.x;
    float moveDurationRightToLeft = distanceRightToLeft / Constants::SpeedEnemyPlane1;

    // Create the action to move from right to left
    auto moveLeft = MoveTo::create(moveDurationRightToLeft, endPositionLeft);

    // Create the action to remove the sprite from the parent node
    auto removeSelf = CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
        });

    // Create a sequence of actions: move right, flip, move left, remove
    auto sequence = Sequence::create(moveRight, flipLeft, moveLeft, removeSelf, nullptr);

    // Run the sequence action
    this->runAction(sequence);
}


void EnemyPlane1::move(const Size& visibleSize) {
    float startX = -this->getContentSize().width / 2;  // Ngoài màn hình bên trái
    float endX = visibleSize.width + this->getContentSize().width / 2;  // Ngoài màn hình bên phải

    // Tính toán thời gian di chuyển dựa trên tốc độ từ Constants::SpeedEnemyPlane1
    float distance = endX - startX;
    float moveDuration = distance / Constants::SpeedEnemyPlane1;

    // Tạo hành động di chuyển từ trái sang phải
    auto moveRight = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));

    // Tạo hành động xóa đối tượng
    auto removeSelf = CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
        });

    // Tạo chuỗi hành động: di chuyển và sau đó xóa
    auto sequence = Sequence::create(moveRight, removeSelf, nullptr);

    // Thực thi chuỗi hành động
    this->runAction(sequence);
}

void EnemyPlane1::moveRightToLeft(const Size& visibleSize) {
    float startX = visibleSize.width + this->getContentSize().width / 2;  // Ngoài màn hình bên phải
    float endX = -this->getContentSize().width / 2;  // Ngoài màn hình bên trái

    // Tính toán thời gian di chuyển dựa trên tốc độ từ Constants::SpeedEnemyPlane1
    float distance = startX - endX;
    float moveDuration = distance / Constants::SpeedEnemyPlane1;

    // Tạo hành động di chuyển từ phải sang trái
    auto moveLeft = MoveTo::create(moveDuration, Vec2(endX, this->getPositionY()));

    // Tạo hành động xóa đối tượng
    auto removeSelf = CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
        });

    // Tạo chuỗi hành động: di chuyển và sau đó xóa
    auto sequence = Sequence::create(moveLeft, removeSelf, nullptr);

    // Thực thi chuỗi hành động
    this->runAction(sequence);
}

// EnemyPlane1.cpp

void EnemyPlane1::moveAndReturn(const Size& visibleSize) {
    float startXLeft = -this->getContentSize().width / 2;  // Outside the left side of the screen
    float endXRight = visibleSize.width + this->getContentSize().width / 2;  // Outside the right side of the screen

    // Calculate the duration for moving from left to right based on speed
    float distanceLeftToRight = endXRight - startXLeft;
    float moveDurationLeftToRight = distanceLeftToRight / Constants::SpeedEnemyPlane1;

    // Create the action to move from left to right
    auto moveRight = MoveTo::create(moveDurationLeftToRight, Vec2(endXRight, this->getPositionY()));

    // Create the action to flip the sprite horizontally
    auto flipRight = CallFunc::create([this]() {
        modelCharac->setFlippedX(false);
        });

    float startXRight = visibleSize.width + this->getContentSize().width / 2;  // Outside the right side of the screen
    float endXLeft = -this->getContentSize().width / 2;  // Outside the left side of the screen

    // Calculate the duration for moving from right to left based on speed
    float distanceRightToLeft = startXRight - endXLeft;
    float moveDurationRightToLeft = distanceRightToLeft / Constants::SpeedEnemyPlane1;

    // Create the action to move from right to left
    auto moveLeft = MoveTo::create(moveDurationRightToLeft, Vec2(endXLeft, this->getPositionY()));

    // Create the action to flip the sprite back to its original orientation
    auto flipLeft = CallFunc::create([this]() {
        modelCharac->setFlippedX(true);
        });

    // Create a sequence of actions: move right, flip, move left, flip back
    auto sequence = Sequence::create(moveRight, flipRight, moveLeft, flipLeft, nullptr);

    // Create a repeat action to loop the sequence indefinitely
    auto repeatAction = RepeatForever::create(sequence);

    // Run the repeat action
    this->runAction(repeatAction);
}


void EnemyPlane1::spawnEnemyAfterDelay(float delay, cocos2d::Node* parent)
{
    auto spawnEnemy = CallFunc::create([parent]() {
        auto enemyPlane1 = EnemyPlane1::createEnemyPlane1();
        if (enemyPlane1)
        {
            parent->addChild(enemyPlane1);
        }
        else
        {
            CCLOG("Failed to create EnemyPlane1");
        }
        });

    auto sequence = Sequence::create(DelayTime::create(delay), spawnEnemy, nullptr);
    parent->runAction(sequence);
}