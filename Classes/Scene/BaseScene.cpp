#include "BaseScene.h"
#include "cocos2d.h"

USING_NS_CC;

bool BaseScene::init() {
    if (!Scene::init()) {
        return false;
    }

    _pauseButton = PauseButton::create();
    _pauseButton->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    float padding = 10.0f;
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    //auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    //_pauseButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width - _pauseButton->getContentSize().width / 2 - padding,
    //    origin.y + visibleSize.height - _pauseButton->getContentSize().height / 2 - padding));
    this->addChild(_pauseButton, 1);

    // Add keyboard event listener for pause functionality
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            if (_pauseButton && !_pauseButton->isPaused()) {
                _pauseButton->pauseGame();
            }
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
            if (_pauseButton && _pauseButton->isPaused()) {
                _pauseButton->continueGame();
            }
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
