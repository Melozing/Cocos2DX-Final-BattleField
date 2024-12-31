#include "BackgroundManager.h"
#include "Controller/SpriteController.h"

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

    // Scale the sprite to fit the screen
    auto spriteSize = background->getContentSize();
    background->setScale(visibleSize.width / spriteSize.width, visibleSize.height / spriteSize.height);

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

void BackgroundManager::preloadBackgrounds(cocos2d::Node* parent, const std::vector<std::string>& spriteFrameNames, int frameCount, float delay, int zOrder) {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    for (const auto& spriteFrameName : spriteFrameNames) {
        auto spriteBatchNode = cocos2d::SpriteBatchNode::create("assets_game/gameplay/" + spriteFrameName + ".png");
        parent->addChild(spriteBatchNode, zOrder);
        spriteBatchNodes[spriteFrameName] = spriteBatchNode;

        cocos2d::Vector<cocos2d::SpriteFrame*> frames;
        for (int i = 1; i <= frameCount; ++i) {
            std::string frameName = spriteFrameName + std::to_string(i) + ".png";
            auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
            frames.pushBack(frame);
        }
        spriteFrames[spriteFrameName] = frames;

        auto background = cocos2d::Sprite::createWithSpriteFrame(frames.front());
        background->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // Scale the sprite to fit the screen
        auto spriteSize = background->getContentSize();
        background->setScale(visibleSize.width / spriteSize.width, visibleSize.height / spriteSize.height);

        spriteBatchNode->addChild(background);
        backgrounds[spriteFrameName] = background;

        auto animation = cocos2d::Animation::createWithSpriteFrames(frames, delay);
        auto animate = cocos2d::Animate::create(animation);
        background->runAction(cocos2d::RepeatForever::create(animate));

        background->setVisible(false);
    }
}

void BackgroundManager::showBackground(const std::string& spriteFrameName) {
    hideAllBackgrounds();
    if (backgrounds.find(spriteFrameName) != backgrounds.end()) {
        backgrounds[spriteFrameName]->setVisible(true);
    }
}

void BackgroundManager::hideAllBackgrounds() {
    for (auto& pair : backgrounds) {
        pair.second->setVisible(false);
    }
}

void BackgroundManager::transitionBackground(const std::string& spriteFrameName, float duration) {
    if (backgrounds.find(spriteFrameName) != backgrounds.end()) {
        auto newBackground = backgrounds[spriteFrameName];
        newBackground->setOpacity(0);
        newBackground->setVisible(true);

        auto fadeIn = cocos2d::FadeIn::create(duration);
        auto fadeOut = cocos2d::FadeOut::create(duration);
        auto blink = cocos2d::Blink::create(duration, 3); // Blink 3 times during the duration

        for (auto& pair : backgrounds) {
            if (pair.second->isVisible() && pair.first != spriteFrameName) {
                pair.second->runAction(fadeOut->clone());
            }
        }

        auto sequence = cocos2d::Sequence::create(blink, fadeIn, nullptr);
        newBackground->runAction(sequence);
    }
}