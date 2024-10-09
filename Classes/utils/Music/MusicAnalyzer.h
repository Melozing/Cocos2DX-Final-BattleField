#ifndef __MUSIC_ANALYZER_H__
#define __MUSIC_ANALYZER_H__

#include "MusicEvent.h"
#include <vector>
#include <string>

class MusicAnalyzer {
public:
    static MusicAnalyzer* getInstance();
    void analyzeMusic(const std::string& filePath);
    void update(float dt);
    std::vector<MusicEvent> getMusicEvents(float dt);
    float getCurrentFrequency() const;
    float getCurrentPitch() const;
    bool isBeatDetected() const; // Add this method

private:
    MusicAnalyzer();
    static MusicAnalyzer* instance;
    std::vector<float> spectrum;
    std::string musicFilePath; // Store the file path
    float currentFrequency;
    float currentPitch;
    bool isMusicPlaying; // Add this line
    bool beatDetected; // Add this line
};

#endif // __MUSIC_ANALYZER_H__
