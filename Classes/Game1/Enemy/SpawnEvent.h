#ifndef SPAWN_EVENT_H
#define SPAWN_EVENT_H

#include <string>

struct SpawnEvent {
    float spawnTime;
    std::string enemyType;
    bool spawned;

    // Default constructor
    SpawnEvent() : spawnTime(0.0f), enemyType(""), spawned(false) {}

    // Parameterized constructor
    SpawnEvent(float time, const std::string& type, bool spawned = false)
        : spawnTime(time), enemyType(type), spawned(spawned) {}
};

#endif // SPAWN_EVENT_H
