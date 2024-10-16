#include "BackgroundManager.h"

BackgroundManager* BackgroundManager::instance = nullptr;

BackgroundManager* BackgroundManager::getInstance() {
    if (!instance) {
        instance = new BackgroundManager();
    }
    return instance;
}

void BackgroundManager::setBackground(cocos2d::Node* parent, const std::string& backgroundFile, int zOrder) {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto background = cocos2d::Sprite::create(backgroundFile);
    background->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    background->setContentSize(visibleSize); // Adjust the size to fit the screen
    parent->addChild(background, zOrder);
}

void BackgroundManager::setBackgroundWithOverlay(cocos2d::Node* parent, const std::string& backgroundFile, const cocos2d::Color4B& overlayColor, int zOrder) {
    setBackground(parent, backgroundFile, zOrder);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // Adjust the alpha value to make the overlay darker
    cocos2d::Color4B darkerOverlayColor = overlayColor;
    darkerOverlayColor.a = 200; // Increase alpha value to make it darker

    auto overlay = cocos2d::LayerColor::create(darkerOverlayColor, visibleSize.width, visibleSize.height);
    overlay->setPosition(origin);
    parent->addChild(overlay, zOrder + 1); // Ensure overlay is above the background
}
