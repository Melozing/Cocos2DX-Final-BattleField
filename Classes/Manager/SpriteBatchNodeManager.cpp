#include "SpriteBatchNodeManager.h"

SpriteBatchNodeManager& SpriteBatchNodeManager::getInstance() {
    static SpriteBatchNodeManager instance;
    return instance;
}

cocos2d::SpriteBatchNode* SpriteBatchNodeManager::getBatchNode(const std::string& textureFile) {
    if (_batchNodes.find(textureFile) == _batchNodes.end()) {
        _batchNodes[textureFile] = cocos2d::SpriteBatchNode::create(textureFile);
    }
    return _batchNodes[textureFile];
}

void SpriteBatchNodeManager::removeBatchNode(const std::string& textureFile) {
    if (_batchNodes.find(textureFile) != _batchNodes.end()) {
        _batchNodes.erase(textureFile);
    }
}
