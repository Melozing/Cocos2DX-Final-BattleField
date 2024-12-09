#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "ui/CocosGUI.h"
#include "Constants/Constants.h"

USING_NS_CC;

float SpriteController::updateSpriteScale(Sprite* sprite, float size)
{
    float SPRITE_SCALE_RATIO = size;
    Size screenSize = Director::getInstance()->getVisibleSize();
    Size spriteSize = sprite->getContentSize();

    float scaleX = (screenSize.width * SPRITE_SCALE_RATIO) / spriteSize.width;
    float scaleY = (screenSize.height * SPRITE_SCALE_RATIO) / spriteSize.height;

    float scale = std::min(scaleX, scaleY);
    return scale;
}

Size SpriteController::GetContentSizeSprite(Sprite* sprite) {
    auto originalSize = sprite->getContentSize();
    auto scaledSize = Size(originalSize.width * sprite->getScaleX(),
        originalSize.height * sprite->getScaleY());
    return scaledSize;
}

void SpriteController::changeAnimation(const std::string& namePrefix, int numOrder)
{
    animation = createAnimation(namePrefix, numOrder, 0.035f);
    auto animate = Animate::create(animation);
    modelCharac->runAction(animate);
}

Animation* SpriteController::createAnimation(const std::string& prefixName, int frameCount, float delay)
{
    Vector<SpriteFrame*> animFrames;

    for (int i = 1; i < frameCount; ++i)
    {
        std::string frameName = prefixName + StringUtils::format("%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    return Animation::createWithSpriteFrames(animFrames, delay);
}

Animation* SpriteController::createForwardReverseAnimation(const std::string& prefixName, int frameCount, float delay)
{
    Vector<SpriteFrame*> animFrames;

    // Add frames from 1 to frameCount
    for (int i = 1; i <= frameCount; ++i)
    {
        std::string frameName = prefixName + StringUtils::format("%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    // Add frames from frameCount back to 1
    for (int i = frameCount; i >= 1; --i)
    {
        std::string frameName = prefixName + StringUtils::format("%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
    }

    return Animation::createWithSpriteFrames(animFrames, delay);
}

float SpriteController::calculateScreenRatio(float ratio) {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    return (visibleSize.width * ratio);
}

float SpriteController::updateSpriteScale(cocos2d::Node* node, float size) {
    if (node) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto contentSize = node->getContentSize();

        // Calculate the target size based on the percentage of the screen size
        float targetWidth = visibleSize.width * size;
        float targetHeight = visibleSize.height * size;

        // Calculate the scale factors
        float scaleX = targetWidth / contentSize.width;
        float scaleY = targetHeight / contentSize.height;

        // Return the minimum scale factor to maintain aspect ratio
        return std::min(scaleX, scaleY);
    }
    return 1.0f;
}