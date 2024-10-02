// Cursor.cpp
#include "Game2/Cursor/Cursor.h"

USING_NS_CC;

Cursor::Cursor() : _mousePos(Vec2::ZERO) {}

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

void Cursor::onMouseMove(Event* event) {
    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());
}

void Cursor::updateCursor(float delta) {
    auto mousePos = Director::getInstance()->convertToGL(_mousePos);

    // Get the window size
    auto winSize = Director::getInstance()->getWinSize();

    // Invert the y-coordinate
    mousePos.y = winSize.height - mousePos.y;

    // Clamp the mouse position within the window boundaries
    mousePos.x = std::max(0.0f, std::min(mousePos.x, winSize.width));
    mousePos.y = std::max(0.0f, std::min(mousePos.y, winSize.height));

    this->setPosition(mousePos);
}
