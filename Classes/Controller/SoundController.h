#ifndef __SOUND_CONTROLLER_H__
#define __SOUND_CONTROLLER_H__

#include "cocos2d.h"
#include <vector>
#include <functional>
#include <unordered_map>

class SoundController {
public:
    static SoundController* getInstance();

    void preloadMusic(const std::string& filePath);
    void playMusic(const std::string& filePath, bool loop = false);
    void stopMusic();
    void stopMusic(const std::string& filePath);
    void scheduleSpawnEvents(const std::vector<float>& timestamps, const std::vector<std::function<void()>>& spawnFunctions);
    bool isMusicPlaying(const std::string& filePath);
    float getMusicDuration(const std::string& filePath);
    std::vector<float> getFrequencyData();
private:
    SoundController();
    static SoundController* instance;
    std::vector<std::pair<float, std::function<void()>>> spawnEvents;
    float elapsedTime;
    size_t currentEventIndex;
    std::unordered_map<std::string, int> playingMusic; // Track playing music
    void update(float dt);
};

#endif // __SOUND_CONTROLLER_H__
