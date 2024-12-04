#include "cocos2d.h"
#include "Controller/SpriteController.h"
#include "ui/CocosGUI.h"
#include "Constants/Constants.h"

USING_NS_CC;

std::unordered_map<std::string, Animation*> SpriteController::animationCache;
std::unordered_map<std::string, SpriteBatchNode*> SpriteController::spriteBatchNodeCache; // Initialize cache for SpriteBatchNode

float SpriteController::updateSpriteScale(Sprite* sprite, float size) {
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
    auto scaledSize = Size(originalSize.width * sprite->getScaleX(), originalSize.height * sprite->getScaleY());
    return scaledSize;
}

void SpriteController::changeAnimation(const std::string& namePrefix, int numOrder) {
    animation = getCachedAnimation(namePrefix);
    if (!animation) {
        animation = createAnimation(namePrefix, numOrder, 0.035f);
        cacheAnimation(namePrefix, numOrder, 0.035f);
    }
    auto animate = Animate::create(animation);
    modelCharac->runAction(RepeatForever::create(animate)); // Use RepeatForever to loop the animation
}

Animation* SpriteController::createAnimation(const std::string& prefixName, int frameCount, float delay) {
    Vector<SpriteFrame*> animFrames;

    for (int i = 1; i < frameCount; ++i) {
        std::string frameName = prefixName + StringUtils::format("%d.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            animFrames.pushBack(frame);
        }
    }

    return Animation::createWithSpriteFrames(animFrames, delay);
}

void SpriteController::cacheAnimation(const std::string& prefixName, int frameCount, float delay) {
    auto animation = createAnimation(prefixName, frameCount, delay);
    animationCache[prefixName] = animation;
}

Animation* SpriteController::getCachedAnimation(const std::string& prefixName) {
    auto it = animationCache.find(prefixName);
    if (it != animationCache.end()) {
        return it->second;
    }
    return nullptr;
}

void SpriteController::cacheSpriteBatchNode(const std::string& fileName) {
    auto spriteBatchNode = SpriteBatchNode::create(fileName);
    spriteBatchNodeCache[fileName] = spriteBatchNode;
}

SpriteBatchNode* SpriteController::getCachedSpriteBatchNode(const std::string& fileName) {
    auto it = spriteBatchNodeCache.find(fileName);
    if (it != spriteBatchNodeCache.end()) {
        return it->second;
    }
    return nullptr;
}
