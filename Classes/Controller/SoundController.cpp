#include "SoundController.h"
#include "audio/include/AudioEngine.h"
#include "Constants/Constants.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;

using namespace cocos2d::experimental;

SoundController* SoundController::instance = nullptr;

SoundController* SoundController::getInstance() {
    if (instance == nullptr) {
        instance = new SoundController();
    }
    return instance;
}

void SoundController::preloadMusic(const std::string& filePath) {
    AudioEngine::preload(filePath);
}

int SoundController::playMusic(const std::string& filePath, bool loop) {
	Constants::currentSoundTrackPath = filePath;
    stopMusic(filePath);
    int audioId = AudioEngine::play2d(filePath, loop);
    playingMusic[filePath] = audioId;
    return audioId;
}

void SoundController::stopMusic() {
    AudioEngine::stopAll();
    playingMusic.clear();
}

void SoundController::stopMusic(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        AudioEngine::stop(it->second);
        playingMusic.erase(it);
    }
}

bool SoundController::isMusicPlaying(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        return AudioEngine::getState(it->second) == AudioEngine::AudioState::PLAYING;
    }
    return false;
}

float SoundController::getMusicDuration(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    CCLOG("Duration of the sound: %f seconds", AudioEngine::getDuration(it->second));
    if (it != playingMusic.end()) {
        return AudioEngine::getDuration(it->second);
    }
    return 0.0f;
}

void SoundController::scheduleSpawnEvents(const std::vector<float>& timestamps, const std::vector<std::function<void()>>& spawnFunctions) {
    spawnEvents.clear();
    for (size_t i = 0; i < timestamps.size(); ++i) {
        spawnEvents.emplace_back(timestamps[i], spawnFunctions[i]);
    }
    currentEventIndex = 0;
    elapsedTime = 0.0f;
}

void SoundController::setMusicVolume(const std::string& filePath, float volume) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        AudioEngine::setVolume(it->second, volume);
    }
}

void SoundController::pauseMusic(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        AudioEngine::pause(it->second);
    }
}

void SoundController::resumeMusic(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        AudioEngine::resume(it->second);
    }
}

void SoundController::replayMusic(const std::string& filePath) {
    stopMusic(filePath);
    playMusic(filePath);
}

void SoundController::playSoundEffect(const std::string& filePath, bool loop) {
    auto it = preloadedSoundEffects.find(filePath);
    if (it != preloadedSoundEffects.end()) {
        SimpleAudioEngine::getInstance()->playEffect(filePath.c_str(), loop);
    }
}

float SoundController::getSoundEffectDuration(const std::string& filePath) {
    // Preload the sound effect to ensure it is loaded
    AudioEngine::preload(filePath, [filePath](bool isSuccess) {
        if (!isSuccess) {
            CCLOG("Failed to preload sound effect: %s", filePath.c_str());
        }
        });

    // Wait for the preload to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Play the sound effect to get a valid audio ID
    int audioId = AudioEngine::play2d(filePath, false);

    // Get the duration of the sound effect
    float duration = AudioEngine::getDuration(audioId);

    // Stop the sound effect immediately after getting the duration
    AudioEngine::stop(audioId);

    return duration;
}
