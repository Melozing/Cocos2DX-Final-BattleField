#include "AudioUtils.h"
#include "Controller/SoundController.h"
#include "Constants/Constants.h"

int AudioUtils::restartMusic(const std::string& filePath) {
    SoundController::getInstance()->stopMusic();
    return SoundController::getInstance()->playMusic(filePath, true);
}
