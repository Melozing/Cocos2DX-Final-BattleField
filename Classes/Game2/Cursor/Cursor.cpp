#include "Game2/Cursor/Cursor.h"

USING_NS_CC;

Cursor::Cursor() : _mousePos(Vec2::ZERO), _isUpdating(false) {}

Cursor* Cursor::create(const std::string& filename) {
    Cursor* pRet = new(std::nothrow) Cursor();
    if (pRet && pRet->initWithFile(filename)) {
        pRet->autorelease();
        pRet->initMouseListener();
        return pRet;
    }
    else {
        delete pRet;
        return nullptr;
    }
}

void Cursor::initMouseListener() {
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(Cursor::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Cursor::onMouseMove(cocos2d::Event* event) {
    auto e = static_cast<EventMouse*>(event);
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());
    updateCursorPosition();
}

void Cursor::updateCursorPosition() {
    auto mousePos = Director::getInstance()->convertToGL(_mousePos);
    auto winSize = Director::getInstance()->getWinSize();
    mousePos.y = winSize.height - mousePos.y;
    mousePos.x = std::max(0.0f, std::min(mousePos.x, winSize.width));
    mousePos.y = std::max(0.0f, std::min(mousePos.y, winSize.height));
    this->setPosition(mousePos);
}

void Cursor::changeSprite(const std::string& filename) {
    if (this->initWithFile(filename)) {
        CCLOG("Sprite changed to %s", filename.c_str());
        this->autorelease();
    }
    else {
        CCLOG("Failed to change sprite to %s", filename.c_str());
    }
}

void Cursor::changeColor(const Color3B& color) {
    this->setColor(color);
}