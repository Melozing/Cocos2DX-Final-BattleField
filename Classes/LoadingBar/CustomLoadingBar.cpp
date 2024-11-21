#include "CustomLoadingBar.h"
#include "Controller/SpriteController.h"

CustomLoadingBar* CustomLoadingBar::create(const std::string& loadingBarImage, const std::string& borderImage, float scale) {
    CustomLoadingBar* ret = new (std::nothrow) CustomLoadingBar();
    if (ret && ret->init(loadingBarImage, borderImage, scale)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CustomLoadingBar::init(const std::string& loadingBarImage, const std::string& borderImage, float scale) {
    if (!Node::init()) {
        return false;
    }

    // Create and position the loading bar
    loadingBar = ui::LoadingBar::create(loadingBarImage);
    loadingBar->setPercent(0);
    loadingBar->setScale(SpriteController::updateSpriteScale(loadingBar, scale));
    this->addChild(loadingBar);

    // Create and position the border
    border = Sprite::create(borderImage);
    border->setScale(SpriteController::updateSpriteScale(border, scale));
    this->addChild(border, -1); // Place border behind the loading bar

    return true;
}

void CustomLoadingBar::setPercent(float percent) {
    loadingBar->setPercent(percent);
}

float CustomLoadingBar::getPercent() const {
    return loadingBar->getPercent();
}

ui::LoadingBar* CustomLoadingBar::getLoadingBar() const {
    return loadingBar;
}

cocos2d::Sprite* CustomLoadingBar::getBorder() const {
    return border;
}

void CustomLoadingBar::setLoadingBarPosition(const Vec2& position) {
    loadingBar->setPosition(position);
}

void CustomLoadingBar::setBorderPosition(const Vec2& position) {
    border->setPosition(position);
}

void CustomLoadingBar::setLoadingBarRotation(float rotation) {
    loadingBar->setRotation(rotation);
}

void CustomLoadingBar::setBorderRotation(float rotation) {
    border->setRotation(rotation);
}

void CustomLoadingBar::setLoadingBarScale(float scale) {
    loadingBar->setScale(scale);
}

void CustomLoadingBar::setBorderScale(float scale) {
    border->setScale(scale);
}