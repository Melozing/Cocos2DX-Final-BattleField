#ifndef __SOUND_CONTROLLER_H__
#define __SOUND_CONTROLLER_H__

#include "cocos2d.h"
#include <vector>
#include <functional>
#include <unordered_map>

class SoundController {
public:
    // Singleton instance
    static SoundController* getInstance();

    // Music control
    void preloadMusic(const std::string& filePath);
    int playMusic(const std::string& filePath, bool loop = false);
    void stopMusic();
    void stopMusic(const std::string& filePath);
    void setMusicVolume(const std::string& filePath, float volume);
    bool isMusicPlaying(const std::string& filePath);
    float getMusicDuration(const std::string& filePath);
    void pauseMusic(const std::string& filePath);
    void resumeMusic(const std::string& filePath);
    void replayMusic(const std::string& filePath);

    // Sound effects control
    int playSoundEffect(const std::string& filePath, bool loop = false);
    float getSoundEffectDuration(const std::string& filePath);

    // Event scheduling
    void scheduleSpawnEvents(const std::vector<float>& timestamps, const std::vector<std::function<void()>>& spawnFunctions);

private:
    // Private constructor for singleton pattern
    SoundController();
    static SoundController* instance;

    // Private methods
    void update(float dt);

    // Private attributes
    std::vector<std::pair<float, std::function<void()>>> spawnEvents;
    float elapsedTime;
    size_t currentEventIndex;
    std::unordered_map<std::string, int> playingMusic;
};

#endif // __SOUND_CONTROLLER_H__
