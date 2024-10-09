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

MusicAnalyzer::MusicAnalyzer() : musicFilePath(""), currentFrequency(0.0f), currentPitch(0.0f), isMusicPlaying(false), beatDetected(false) {
    spectrum.resize(512, 0.0f);
}

void MusicAnalyzer::analyzeMusic(const std::string& filePath) {
    if (isMusicPlaying) {
        return; // Music is already playing, no need to load and play again
    }
    musicFilePath = filePath; // Store the file path
    FMODAudioEngine::getInstance()->load(filePath); // Load sound without playing
    FMODAudioEngine::getInstance()->playSound(filePath); // Play the sound
    isMusicPlaying = true; // Set flag to indicate music is playing
}

void MusicAnalyzer::update(float dt) {
    FMODAudioEngine::getInstance()->update(dt);
    FMODAudioEngine::getInstance()->getSpectrum(spectrum.data(), spectrum.size());
    currentFrequency = std::accumulate(spectrum.begin(), spectrum.end(), 0.0f) / spectrum.size();
    currentPitch = *std::max_element(spectrum.begin(), spectrum.end());

    // Adjusted beat detection logic
    beatDetected = currentFrequency > 0.001f; // Adjust threshold as needed
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
    if (currentFrequency > 0.002f && currentPitch > 0.005f) {
        events.emplace_back(MusicEventType::MELODY, dt);
    }

    // Frequency bands
    float lowFrequency = std::accumulate(spectrum.begin(), spectrum.begin() + spectrum.size() / 3, 0.0f) / (spectrum.size() / 3);
    float midFrequency = std::accumulate(spectrum.begin() + spectrum.size() / 3, spectrum.begin() + 2 * spectrum.size() / 3, 0.0f) / (spectrum.size() / 3);
    float highFrequency = std::accumulate(spectrum.begin() + 2 * spectrum.size() / 3, spectrum.end(), 0.0f) / (spectrum.size() / 3);

    if (lowFrequency > 0.003f) {
        events.emplace_back(MusicEventType::LOW, dt);
    }
    if (midFrequency > 0.003f) {
        events.emplace_back(MusicEventType::MID, dt);
    }
    if (highFrequency > 0.003f) {
        events.emplace_back(MusicEventType::HIGH, dt);
    }

    if (currentFrequency > 0.007f && currentPitch < 0.005f) {
        events.emplace_back(MusicEventType::DROP, dt);
    }
    if (currentFrequency < 0.002f && currentPitch > 0.01f) {
        events.emplace_back(MusicEventType::RISE, dt);
    }
    if (currentFrequency > 0.004f && currentPitch > 0.008f) {
        events.emplace_back(MusicEventType::CLAP, dt);
    }
    if (currentFrequency > 0.006f && currentPitch > 0.009f) {
        events.emplace_back(MusicEventType::HAT, dt);
    }
    if (currentFrequency > 0.008f && currentPitch > 0.01f) {
        events.emplace_back(MusicEventType::BASS, dt);
    }
    if (currentFrequency > 0.009f && currentPitch > 0.011f) {
        events.emplace_back(MusicEventType::VOCAL, dt);
    }
    if (currentFrequency > 0.01f && currentPitch > 0.012f) {
        events.emplace_back(MusicEventType::SYNTH, dt);
    }
    if (currentFrequency > 0.011f && currentPitch > 0.013f) {
        events.emplace_back(MusicEventType::PAD, dt);
    }
    if (currentFrequency > 0.012f && currentPitch > 0.014f) {
        events.emplace_back(MusicEventType::FX, dt);
    }
    if (currentFrequency > 0.013f && currentPitch > 0.015f) {
        events.emplace_back(MusicEventType::PERCUSSION, dt);
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
