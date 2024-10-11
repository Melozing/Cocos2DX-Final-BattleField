#include "NewEnemy.h"

USING_NS_CC;

NewEnemy* NewEnemy::create() {
    NewEnemy* pRet = new(std::nothrow) NewEnemy();
    if (pRet && pRet->initWithFile("assets_game/enemies/ship.png")) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

void NewEnemy::spawn(const Vec2& position) {
    this->setPosition(position);
    // Add any additional initialization or animation here
}

Size NewEnemy::GetSize() const {
    return this->getContentSize();
}
