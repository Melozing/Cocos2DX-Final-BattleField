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
    int playMusic(const std::string& filePath, bool loop = false);
    void stopMusic();
    void stopMusic(const std::string& filePath);
    void setMusicVolume(const std::string& filePath, float volume);
    void scheduleSpawnEvents(const std::vector<float>& timestamps, const std::vector<std::function<void()>>& spawnFunctions);
    bool isMusicPlaying(const std::string& filePath);
    float getMusicDuration(const std::string& filePath);
    void pauseMusic(const std::string& filePath);
    void resumeMusic(const std::string& filePath);
    void replayMusic(const std::string& filePath);

private:
    SoundController(); // Declaration only
    static SoundController* instance;
    std::vector<std::pair<float, std::function<void()>>> spawnEvents;
    float elapsedTime;
    size_t currentEventIndex;
    std::unordered_map<std::string, int> playingMusic;
    void update(float dt);
};

#endif // __SOUND_CONTROLLER_H__
