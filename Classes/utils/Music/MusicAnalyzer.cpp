#include "MusicAnalyzer.h"
#include "fmod/FMODAudioEngine.h"
#include "cocos2d.h"
#include <numeric>

MusicAnalyzer* MusicAnalyzer::instance = nullptr;

MusicAnalyzer* MusicAnalyzer::getInstance() {
    if (instance == nullptr) {
        instance = new MusicAnalyzer();
    }
    return instance;
}

MusicAnalyzer::MusicAnalyzer() : musicFilePath(""), currentFrequency(0.0f), currentPitch(0.0f), isMusicPlaying(false), beatDetected(false), soundID(-1) {
    spectrum.resize(512, 0.0f);
}

void MusicAnalyzer::analyzeMusic(const std::string& filePath) {
    if (isMusicPlaying) {
        return; // Music is already playing, no need to load and play again
    }
    musicFilePath = filePath; // Store the file path
    FMODAudioEngine::getInstance()->load(filePath); // Load sound without playing
    soundID = FMODAudioEngine::getInstance()->playSound(filePath); // Play the sound and store soundID
    isMusicPlaying = true; // Set flag to indicate music is playing
}

void MusicAnalyzer::stopMusic() {
    if (!isMusicPlaying) {
        return; // No music is playing, nothing to stop
    }
    FMODAudioEngine::getInstance()->stopSound(soundID); // Stop the sound using soundID
    isMusicPlaying = false; // Update the flag
}

void MusicAnalyzer::update(float dt) {
    FMODAudioEngine::getInstance()->update(dt);
    FMODAudioEngine::getInstance()->getSpectrum(spectrum.data(), spectrum.size());
    currentFrequency = std::accumulate(spectrum.begin(), spectrum.end(), 0.0f) / spectrum.size();
    currentPitch = *std::max_element(spectrum.begin(), spectrum.end());

    // Adjusted beat detection logic
    beatDetected = currentFrequency > 0.001f; // Adjust threshold as needed
}

void MusicAnalyzer::pauseMusic() {
    if (isMusicPlaying) {
        FMODAudioEngine::getInstance()->pauseSound(soundID);
    }
}

void MusicAnalyzer::resumeMusic() {
    if (isMusicPlaying) {
        FMODAudioEngine::getInstance()->resumeSound(soundID);
    }
}

void MusicAnalyzer::replayMusic() {
    if (isMusicPlaying) {
        stopMusic(); // Stop current music if any
    }
    analyzeMusic(musicFilePath); // Load and play the same sound again
}

std::vector<MusicEvent> MusicAnalyzer::getMusicEvents(float dt) {
    std::vector<MusicEvent> events;

    // Log current frequency and pitch
    //CCLOG("Current Frequency: %f", currentFrequency);
    //CCLOG("Current Pitch: %f", currentPitch);

    if (beatDetected) {
        events.emplace_back(MusicEventType::BEAT, dt);
    }

    // Dummy logic for detecting various events
    if (currentFrequency > 0.005f) {
        events.emplace_back(MusicEventType::KICK, dt);
    }
    if (currentPitch > 0.01f) {
        events.emplace_back(MusicEventType::SNARE, dt);
    }
    return events;
}

float MusicAnalyzer::getCurrentFrequency() const {
    return currentFrequency;
}

float MusicAnalyzer::getCurrentPitch() const {
    return currentPitch;
}

bool MusicAnalyzer::isBeatDetected() const {
    return beatDetected;
}

float MusicAnalyzer::getMusicDuration(const std::string& filePath) {
    return FMODAudioEngine::getInstance()->getSoundDuration(filePath);
}
