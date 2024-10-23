#pragma once
#include "cocos2d.h"

class PositionManager {
public:
    static PositionManager& getInstance();

    cocos2d::Vec2 getRandomSpawnPosition(const cocos2d::Size& visibleSize);
    bool isPositionOccupied(const cocos2d::Vec2& position);
    void addOccupiedPosition(const cocos2d::Vec2& position);
    void clearOccupiedPositions();
    bool isOutsidePlayerMovementArea(const cocos2d::Vec2& position, const cocos2d::Size& visibleSize, float minX, float maxX, float minY, float maxY);

private:
    PositionManager() = default;
    std::vector<cocos2d::Vec2> occupiedPositions;
};
