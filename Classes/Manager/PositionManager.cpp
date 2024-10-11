#include "PositionManager.h"

PositionManager& PositionManager::getInstance() {
    static PositionManager instance;
    return instance;
}

cocos2d::Vec2 PositionManager::getRandomSpawnPosition(const cocos2d::Size& visibleSize) {
    float x = cocos2d::RandomHelper::random_real(0.0f, visibleSize.width);
    float y = cocos2d::RandomHelper::random_real(0.0f, visibleSize.height);
    return cocos2d::Vec2(x, y);
}

bool PositionManager::isPositionOccupied(const cocos2d::Vec2& position) {
    for (const auto& occupiedPosition : occupiedPositions) {
        if (position.distance(occupiedPosition) < 50.0f) { // Adjust the distance threshold as needed
            return true;
        }
    }
    return false;
}

void PositionManager::addOccupiedPosition(const cocos2d::Vec2& position) {
    occupiedPositions.push_back(position);
}

void PositionManager::clearOccupiedPositions() {
    occupiedPositions.clear();
}