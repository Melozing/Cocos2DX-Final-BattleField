#include "SpriteFrameCacheManager.h"

SpriteFrameCacheManager& SpriteFrameCacheManager::getInstance() {
    static SpriteFrameCacheManager instance;
    return instance;
}

void SpriteFrameCacheManager::loadSpriteFrames(const std::string& plistFile) {
    if (_referenceCount[plistFile] == 0 && !cocos2d::SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded(plistFile)) {
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFile);
    }
    _referenceCount[plistFile]++;
}

void SpriteFrameCacheManager::unloadSpriteFrames(const std::string& plistFile) {
    if (_referenceCount[plistFile] > 0) {
        _referenceCount[plistFile]--;
        if (_referenceCount[plistFile] == 0) {
            cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistFile);
        }
    }
}

