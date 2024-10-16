#ifndef __MUSIC_ANALYZER_H__
#define __MUSIC_ANALYZER_H__

#include "MusicEvent.h"
#include <vector>
#include <string>

class MusicAnalyzer {
public:
    static MusicAnalyzer* getInstance();
    void analyzeMusic(const std::string& filePath);
    void stopMusic(); 
    void update(float dt);
    std::vector<MusicEvent> getMusicEvents(float dt);
    float getCurrentFrequency() const;
    float getCurrentPitch() const;
    bool isBeatDetected() const;
    float getMusicDuration(const std::string& filePath);
    void pauseMusic();
    void resumeMusic();
    void replayMusic();
    bool isMusicPlaying;
private:
    MusicAnalyzer();
    static MusicAnalyzer* instance;
    std::vector<float> spectrum;
    std::string musicFilePath;
    float currentFrequency;
    float currentPitch;
    bool beatDetected;
    int soundID;
};

#endif // __MUSIC_ANALYZER_H__
