#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include "cocos2d.h"
#include "Constants/Constants.h"

class BackgroundManager {
public:
    static BackgroundManager* getInstance();
    void setBackground(cocos2d::Node* parent, const std::string& backgroundFile, int zOrder = Constants::ORDER_LAYER_BACKGROUND);
    void setBackgroundWithOverlay(cocos2d::Node* parent, const std::string& backgroundFile, const cocos2d::Color4B& overlayColor = cocos2d::Color4B(0, 0, 0, 128), int zOrder = Constants::ORDER_LAYER_BACKGROUND);
    void preloadBackgrounds(cocos2d::Node* parent, const std::vector<std::string>& spriteFrameNames, int frameCount, float delay, int zOrder = Constants::ORDER_LAYER_BACKGROUND);
    void showBackground(const std::string& spriteFrameName);
    void hideAllBackgrounds();
    void transitionBackground(const std::string& spriteFrameName, float duration);

private:
    BackgroundManager() = default;
    static BackgroundManager* instance;
    std::unordered_map<std::string, cocos2d::Sprite*> backgrounds;
    std::unordered_map<std::string, cocos2d::SpriteBatchNode*> spriteBatchNodes;
    std::unordered_map<std::string, cocos2d::Vector<cocos2d::SpriteFrame*>> spriteFrames;
};

#endif // BACKGROUNDMANAGER_H
