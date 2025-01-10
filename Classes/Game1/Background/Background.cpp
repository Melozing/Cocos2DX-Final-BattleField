#include "Game1/Background/Background.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

Background* Background::createBackground(const std::string& filePath, float scrollSpeed)
{
    Background* background = new (std::nothrow) Background();
    if (background && background->init(filePath, scrollSpeed))
    {
        background->autorelease();
        return background;
    }
    CC_SAFE_DELETE(background);
    return nullptr;
}

bool Background::init(const std::string& filePath, float scrollSpeed)
{
    if (!Node::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    background1 = Sprite::create(filePath);
    background2 = Sprite::create(filePath);

	background1->setAnchorPoint(Vec2(0.5, 0.5));
    background2->setAnchorPoint(Vec2(0.5, 0.5));

    background1->setScale(visibleSize.width / background1->getContentSize().width,
        visibleSize.height / SpriteController::GetContentSizeSprite(background1).height);
    background2->setScale(visibleSize.width / background2->getContentSize().width,
        visibleSize.height / SpriteController::GetContentSizeSprite(background2).height);

    background1->setPosition(visibleSize.width / 2, (visibleSize.height / 2));
    background2->setPosition(visibleSize.width / 2, (visibleSize.height + visibleSize.height / 2));

    this->addChild(background1);
    this->addChild(background2);

    this->scrollSpeed = scrollSpeed;

    return true;
}

void Background::update(float delta)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float hightBonus = 0;

    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_MAC) {
		hightBonus = SpriteController::calculateScreenHeightRatio(0.205);
    }

    background1->setPositionY(background1->getPositionY() - scrollSpeed * delta);
    background2->setPositionY(background2->getPositionY() - scrollSpeed * delta);

    if (background1->getPositionY() <= -visibleSize.height / 2)
    {
        background1->setPositionY(background2->getPositionY() + visibleSize.height);
    }
    if (background2->getPositionY() <= -visibleSize.height / 2)
    {
        background2->setPositionY(background1->getPositionY() + visibleSize.height);
    }
}
