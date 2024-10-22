#include "Game1/Enemy/FallingTreePool.h"

USING_NS_CC;

FallingTreePool* FallingTreePool::getInstance() {
    static FallingTreePool instance;
    return &instance;
}

void FallingTreePool::initPool(int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        FallingTree* tree = FallingTree::create();
        if (tree) {
            tree->retain();
            tree->reset();
            _availableTrees.push(tree);
        }
    }
}

FallingTree* FallingTreePool::getEnemy() {
    if (_availableTrees.empty()) {
        FallingTree* tree = FallingTree::create();
        if (tree) {
            tree->retain();
            tree->reset();
            return tree;
        }
        return nullptr;
    }
    FallingTree* tree = _availableTrees.front();
    _availableTrees.pop();
    return tree;
}

void FallingTreePool::returnEnemy(FallingTree* tree) {
    if (tree) {
        tree->reset();
        _availableTrees.push(tree);
    }
}

void FallingTreePool::resetPool() {
    while (!_availableTrees.empty()) {
        FallingTree* tree = _availableTrees.front();
        _availableTrees.pop();
        tree->release();
    }
}
