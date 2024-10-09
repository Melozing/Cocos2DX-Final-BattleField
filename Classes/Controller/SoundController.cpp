#include "SoundController.h"
#include "audio/include/AudioEngine.h"
#include "fmod/FMODAudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;

SoundController* SoundController::instance = nullptr;

SoundController* SoundController::getInstance() {
    if (instance == nullptr) {
        instance = new SoundController();
    }
    return instance;
}

SoundController::SoundController() : elapsedTime(0.0f), currentEventIndex(0) {
    Director::getInstance()->getScheduler()->schedule([this](float dt) {
        this->update(dt);
        }, this, 0.0f, false, "sound_controller_update_key");
}

void SoundController::preloadMusic(const std::string& filePath) {
    AudioEngine::preload(filePath);
}

void SoundController::playMusic(const std::string& filePath, bool loop) {
    if (!isMusicPlaying(filePath)) {
        int audioId = AudioEngine::play2d(filePath, loop);
        playingMusic[filePath] = audioId;
    }
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

std::vector<float> SoundController::getFrequencyData() {
    return FMODAudioEngine::getInstance()->getFrequencyData();
}

void SoundController::update(float dt) {
    elapsedTime += dt;
    while (currentEventIndex < spawnEvents.size() && elapsedTime >= spawnEvents[currentEventIndex].first) {
        spawnEvents[currentEventIndex].second();
        currentEventIndex++;
    }
}