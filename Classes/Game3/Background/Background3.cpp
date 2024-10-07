#include "Game3/Background/Background3.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

Background3* Background3::createBackground(const std::string& filePath) {
    Background3* background = new (std::nothrow) Background3();
    if (background && background->init(filePath)) {
        background->autorelease();
        return background;
    }
    CC_SAFE_DELETE(background);
    return nullptr;
}

bool Background3::init(const std::string& filePath) {
    if (!Node::init()) {
        return false;
    }

   
    auto backgroundSprite = Sprite::create(filePath);
    if (!backgroundSprite) {
        CCLOG("Failed to load background image from path: %s", filePath.c_str());
        return false;
    }

  
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto spriteSize = backgroundSprite->getContentSize();

   
    float scaleX = visibleSize.width / spriteSize.width;
    float scaleY = visibleSize.height / spriteSize.height;

   
    float scale = std::max(scaleX, scaleY);
    backgroundSprite->setScale(scale);

 
    backgroundSprite->setPosition(visibleSize / 2);

  
    this->addChild(backgroundSprite);

    return true;
}


