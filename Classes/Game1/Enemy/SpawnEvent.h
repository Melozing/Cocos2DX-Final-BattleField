#ifndef SPAWN_EVENT_H
#define SPAWN_EVENT_H

#include <string>

struct SpawnEvent {
    float spawnTime;
    std::string enemyType;
    bool spawned;

    SpawnEvent(float time, const std::string& type, bool spawned = false)
        : spawnTime(time), enemyType(type), spawned(spawned) {}
};

#endif // SPAWN_EVENT_H
