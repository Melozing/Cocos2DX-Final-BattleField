#include "EnemyPlan3.h"
#include "cocos2d.h"
#include "Constants/Constants.h"

USING_NS_CC;

EnemyPlan3* EnemyPlan3::createEnemyPlan3() {
    EnemyPlan3* enemy = new (std::nothrow) EnemyPlan3();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlan3::init() {
    
    if (!Sprite::initWithFile("assets_game/enemies/Ship.png")) {
        return false;
    }

    
    auto visibleSize = Director::getInstance()->getVisibleSize();

   
    this->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - this->getContentSize().height / 2));

    this->setScale(Constants::PlayerScale3);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->move(visibleSize);

    return true;
}

//ham di chuyen enemy
void EnemyPlan3::move(const Size& visibleSize) {

    float startX = this->getContentSize().width / 2;  // Biên trái
    float endX = visibleSize.width - this->getContentSize().width / 2;  // Biên phải

    // Tạo hành động di chuyển qua trái và qua phải
    auto moveLeft = MoveTo::create(2.0f, Vec2(startX, this->getPositionY())); // Di chuyển về biên trái
    auto moveRight = MoveTo::create(2.0f, Vec2(endX, this->getPositionY()));  // Di chuyển về biên phải

    // Tạo chuỗi hành động lặp lại
    auto sequence = Sequence::create(moveRight, moveLeft, nullptr);
    auto repeatAction = RepeatForever::create(sequence);

    // Thực thi hành động di chuyển lặp lại
    this->runAction(repeatAction);
}