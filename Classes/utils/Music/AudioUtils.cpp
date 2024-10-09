#include "AudioUtils.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"

void AudioUtils::restartMusic(const std::string& filePath) {
    SoundController::getInstance()->stopMusic();
    SoundController::getInstance()->playMusic(filePath, true);
}
