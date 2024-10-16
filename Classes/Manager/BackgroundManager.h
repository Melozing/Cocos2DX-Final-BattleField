#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include "cocos2d.h"
#include "Constants/Constants.h"

class BackgroundManager {
public:
    static BackgroundManager* getInstance();
    void setBackground(cocos2d::Node* parent, const std::string& backgroundFile, int zOrder = Constants::ORDER_LAYER_BACKGROUND);
    void setBackgroundWithOverlay(cocos2d::Node* parent, const std::string& backgroundFile, const cocos2d::Color4B& overlayColor = cocos2d::Color4B(0, 0, 0, 128), int zOrder = Constants::ORDER_LAYER_BACKGROUND);

private:
    BackgroundManager() = default;
    static BackgroundManager* instance;
};

#endif // BACKGROUNDMANAGER_H
