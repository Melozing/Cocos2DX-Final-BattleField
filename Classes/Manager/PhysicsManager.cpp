#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::getInstance() {
    static PhysicsManager instance;
    return &instance;
}

void PhysicsManager::setPhysicsBody(cocos2d::Node* node, int bitmask, bool isDynamic) {
    auto body = cocos2d::PhysicsBody::createBox(node->getContentSize());
    body->setCollisionBitmask(bitmask);
    body->setContactTestBitmask(true);
    body->setDynamic(isDynamic);
    node->setPhysicsBody(body);
}