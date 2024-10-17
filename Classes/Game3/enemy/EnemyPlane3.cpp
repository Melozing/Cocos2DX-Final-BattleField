#include "EnemyPlane3.h"
#include "cocos2d.h"
#include "Constants/Constants.h"

USING_NS_CC;

EnemyPlane3* EnemyPlane3::createEnemyPlan3() {
    EnemyPlane3* enemy = new (std::nothrow) EnemyPlane3();
    if (enemy && enemy->init()) {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlane3::init() {
    if (!Sprite::initWithFile("assets_game/enemies/Ship.png")) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Tạo giá trị y ngẫu nhiên trong khoảng từ giữa màn hình đến đỉnh màn hình
    float randomY = random(visibleSize.height / 2, visibleSize.height);

    // Đặt vị trí khởi tạo của enemy ngoài màn hình bên trái với tọa độ y ngẫu nhiên
    this->setPosition(Vec2(-this->getContentSize().width / 2, randomY));

    this->setScale(Constants::PlayerScale3);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->moveAndReturn(visibleSize);

    return true;
}



// hàm di chuyển enemy từ trái sang phải và sau đó từ phải sang trái
void EnemyPlane3::moveAndReturn(const Size& visibleSize) {
    float startXLeft = -this->getContentSize().width / 2;  // Ngoài màn hình bên trái
    float endXRight = visibleSize.width + this->getContentSize().width / 2;  // Ngoài màn hình bên phải

    // Tính toán thời gian di chuyển dựa trên tốc độ từ Constants::EnemyGame3Speed_1
    float distanceLeftToRight = endXRight - startXLeft;
    float moveDurationLeftToRight = distanceLeftToRight / Constants::EnemyGame3Speed_1;

    // Tạo hành động di chuyển từ trái sang phải
    auto moveRight = MoveTo::create(moveDurationLeftToRight, Vec2(endXRight, this->getPositionY()));

    // Tạo hành động lật ảnh theo trục X
    auto flipRight = FlipX::create(true);

    float startXRight = visibleSize.width + this->getContentSize().width / 2;  // Ngoài màn hình bên phải
    float endXLeft = -this->getContentSize().width / 2;  // Ngoài màn hình bên trái

    // Tính toán thời gian di chuyển dựa trên tốc độ từ Constants::EnemyGame3Speed_1
    float distanceRightToLeft = startXRight - endXLeft;
    float moveDurationRightToLeft = distanceRightToLeft / Constants::EnemyGame3Speed_1;

    // Tạo hành động di chuyển từ phải sang trái
    auto moveLeft = MoveTo::create(moveDurationRightToLeft, Vec2(endXLeft, this->getPositionY()));

    // Tạo hành động lật ảnh theo trục X trở lại
    auto flipLeft = FlipX::create(false);

    // Tạo hành động xóa đối tượng
    auto removeSelf = CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
        });

    // Tạo chuỗi hành động: di chuyển từ trái sang phải, lật ảnh, di chuyển từ phải sang trái, lật ảnh trở lại và cuối cùng xóa đối tượng
    auto sequence = Sequence::create(moveRight, flipRight, moveLeft, flipLeft, removeSelf, nullptr);

    // Thực thi chuỗi hành động
    this->runAction(sequence);
}

// hàm di chuyển enemy từ trái sang phải
void EnemyPlane3::move(const Size& visibleSize) {
    float startX = -this->getContentSize().width / 2;  // Ngoài màn hình bên trái
    float endX = visibleSize.width + this->getContentSize().width / 2;  // Ngoài màn hình bên phải

    // Tính toán thời gian di chuyển dựa trên tốc độ từ Constants::EnemyGame3Speed_1
    float distance = endX - startX;
    float moveDuration = distance / Constants::EnemyGame3Speed_1;

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

// hàm di chuyển enemy từ phải sang trái
void EnemyPlane3::moveRightToLeft(const Size& visibleSize) {
    float startX = visibleSize.width + this->getContentSize().width / 2;  // Ngoài màn hình bên phải
    float endX = -this->getContentSize().width / 2;  // Ngoài màn hình bên trái

    // Tính toán thời gian di chuyển dựa trên tốc độ từ Constants::EnemyGame3Speed_1
    float distance = startX - endX;
    float moveDuration = distance / Constants::EnemyGame3Speed_1;

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

// hàm spawn enemy sau một khoảng thời gian trì hoãn
void EnemyPlane3::spawnEnemyAfterDelay(float delay, Node* parent) {
    auto delayAction = DelayTime::create(delay);
    auto spawnAction = CallFunc::create([parent]() {
        auto enemy = EnemyPlane3::createEnemyPlan3();
        if (enemy) {
            parent->addChild(enemy);
        }
        });

    auto sequence = Sequence::create(delayAction, spawnAction, nullptr);
    auto repeatAction = RepeatForever::create(sequence);
    parent->runAction(repeatAction);
}
