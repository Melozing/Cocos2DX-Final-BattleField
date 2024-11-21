#include "Cursor.h"
#include "Controller/GameController.h"

USING_NS_CC;

Cursor::Cursor() : _mousePos(Vec2::ZERO), _isUpdating(false) {}

Cursor* Cursor::create(const std::string& filename) {
    Cursor* pRet = new(std::nothrow) Cursor();
    if (pRet && pRet->initWithFile(filename)) {
        pRet->autorelease();
        pRet->initMouseListener();
        pRet->setInitialPosition();
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

void Cursor::setInitialPosition() {
    // Create a temporary event listener to get the initial mouse position
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = [this, mouseListener](EventMouse* event) mutable {
        _mousePos = Vec2(event->getCursorX(), event->getCursorY());
        updateCursorPosition();
        _eventDispatcher->removeEventListener(mouseListener);
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Cursor::onMouseMove(cocos2d::Event* event) {
    auto e = static_cast<EventMouse*>(event);
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());
    updateCursorPosition();
}

void Cursor::updateCursorPosition() {
    if (GameController::getInstance()->isGameOver() || GameController::getInstance()->isPaused()) return;

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
