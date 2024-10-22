#ifndef SPRITE_BATCH_NODE_MANAGER_H
#define SPRITE_BATCH_NODE_MANAGER_H

#include "cocos2d.h"
#include <unordered_map>
#include <string>

class SpriteBatchNodeManager {
public:
    static SpriteBatchNodeManager& getInstance();

    cocos2d::SpriteBatchNode* getBatchNode(const std::string& textureFile);
    void removeBatchNode(const std::string& textureFile);

private:
    SpriteBatchNodeManager() = default;
    ~SpriteBatchNodeManager() = default;

    std::unordered_map<std::string, cocos2d::SpriteBatchNode*> _batchNodes;
};

#endif // SPRITE_BATCH_NODE_MANAGER_H
