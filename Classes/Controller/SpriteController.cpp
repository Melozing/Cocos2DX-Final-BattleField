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

float SpriteController::calculateScreenRatio(float ratio) {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    return (visibleSize.width * ratio);
}