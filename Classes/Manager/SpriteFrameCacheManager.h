#ifndef SPRITE_FRAME_CACHE_MANAGER_H
#define SPRITE_FRAME_CACHE_MANAGER_H

#include "cocos2d.h"
#include <unordered_map>
#include <string>

class SpriteFrameCacheManager {
public:
    static SpriteFrameCacheManager& getInstance();

    void loadSpriteFrames(const std::string& plistFile);
    void unloadSpriteFrames(const std::string& plistFile);

private:
    SpriteFrameCacheManager() = default;
    ~SpriteFrameCacheManager() = default;

    std::unordered_map<std::string, int> _referenceCount;
};

#endif // SPRITE_FRAME_CACHE_MANAGER_H
