#ifndef __SPRITE_CONTROLLER_H__
#define __SPRITE_CONTROLLER_H__

#include "cocos2d.h"

USING_NS_CC;

class SpriteController {
public:
    // Animation handling
    static Animation* createAnimation(const std::string& prefixName, int frameCount, float delay);
    static Animation* createForwardReverseAnimation(const std::string& prefixName, int frameCount, float delay);
    void changeAnimation(const std::string& namePrefix, int numOrder);
    static float updateSpriteScale(Sprite* sprite, float size);
    static float updateSpriteScale(cocos2d::Node* node, float size);
    static float calculateScreenRatio(float ratio);
    static Size GetContentSizeSprite(Sprite* sprite);

    // Cache animations
    static void cacheAnimation(const std::string& prefixName, int frameCount, float delay);
    static Animation* getCachedAnimation(const std::string& prefixName);

    // Cache sprite batch nodes
    static void cacheSpriteBatchNode(const std::string& fileName);
    static SpriteBatchNode* getCachedSpriteBatchNode(const std::string& fileName);

private:
    // Sprite and animation
    Sprite* modelCharac;
    Animation* animation;

    // Animation cache
    static std::unordered_map<std::string, Animation*> animationCache;
    static std::unordered_map<std::string, SpriteBatchNode*> spriteBatchNodeCache;
};

#endif // __SPRITE_CONTROLLER_H__
