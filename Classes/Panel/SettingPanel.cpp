#include "SettingPanel.h"
#include "Constants/Constants.h"
#include "Controller/SoundController.h"
#include "Controller/SpriteController.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

SettingPanel* SettingPanel::createPanel(const std::function<void()>& exitAction, const std::function<void()>& quitAction) {
    SettingPanel* panel = new (std::nothrow) SettingPanel();
    if (panel && panel->init(exitAction, quitAction)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool SettingPanel::init(const std::function<void()>& exitAction, const std::function<void()>& quitAction) {
    if (!BasePanel::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create exit button
    auto paddingHCloseBtn = SpriteController::calculateScreenRatio(0.08f);
    auto paddingWCloseBtn = SpriteController::calculateScreenRatio(0.14f);
    // Create and position the close button
    exitButton = ui::Button::create("assets_game/UXUI/Collection/close_btn.png");
    exitButton->setScale(SpriteController::updateSpriteScale(exitButton, 0.135f));
    exitButton->setPosition(Vec2(SpriteController::GetContentSize(boardSprite).width + exitButton->getContentSize().width + paddingWCloseBtn,
        SpriteController::GetContentSize(boardSprite).height + exitButton->getContentSize().height + paddingHCloseBtn));
    exitButton->addClickEventListener([this, exitAction](Ref* sender) {
        SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
        playSoundAndExecuteCallback(exitAction);
        });
    this->addChild(exitButton, -2);

    // Create resume button
    quitButton = ui::Button::create("assets_game/UXUI/Panel/Exit_BTN.png", "assets_game/UXUI/Panel/Exit_BTN_Active.png");
    quitButton->setScale(SpriteController::updateSpriteScale(quitButton, 0.1f));
    quitButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    quitButton->setPosition(Vec2(boardSprite->getContentSize().width / 2, 0 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_PANEL)));
    quitButton->addClickEventListener([this, quitAction](Ref* sender) {
        playSoundAndExecuteCallback(quitAction);
        });
    boardSprite->addChild(quitButton);

    // Create music slider
    musicSlider = ui::Slider::create();
    musicSlider->loadBarTexture("assets_game/UXUI/Panel/Slider_Back.png");
    musicSlider->setScale(SpriteController::updateSpriteScale(musicSlider, 0.15f));
    musicSlider->loadSlidBallTextures("assets_game/UXUI/Panel/SliderNode_Normal.png", "assets_game/UXUI/Panel/SliderNode_Pressed.png", "assets_game/UXUI/Panel/SliderNode_Disabled.png");
    musicSlider->loadProgressBarTexture("assets_game/UXUI/Panel/Slider_PressBar.png");

    // Adjust the size of the ProgressBar
    auto progressBarSize = musicSlider->getContentSize();
    progressBarSize.width *= 0.5f; // Adjust the width to be 50% of the original
    musicSlider->setContentSize(progressBarSize);

    // Restore the saved value
    float savedMusicVolume = UserDefault::getInstance()->getFloatForKey(Constants::UD_musicVolume.c_str(), 0.5f);
    musicSlider->setPercent(savedMusicVolume * 100);

    musicSlider->setPosition(Vec2(boardSprite->getContentSize().width / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_SLIDER),
        boardSprite->getContentSize().height / 2 + SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_SLIDER)));
    musicSlider->addEventListener([this](Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            float volume = musicSlider->getPercent() / 100.0f;
            SoundController::getInstance()->setMusicVolume(volume);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_musicVolume.c_str(), volume); // Save the value
            updateMusicButtonState();
        }
        });
    boardSprite->addChild(musicSlider);

    // Create sound effect slider
    soundEffectSlider = ui::Slider::create();
    soundEffectSlider->loadBarTexture("assets_game/UXUI/Panel/Slider_Back.png");
    soundEffectSlider->setScale(SpriteController::updateSpriteScale(soundEffectSlider, 0.15f));
    soundEffectSlider->loadSlidBallTextures("assets_game/UXUI/Panel/SliderNode_Normal.png", "assets_game/UXUI/Panel/SliderNode_Pressed.png", "assets_game/UXUI/Panel/SliderNode_Disabled.png");
    soundEffectSlider->loadProgressBarTexture("assets_game/UXUI/Panel/Slider_PressBar.png");

    // Adjust the size of the ProgressBar
    auto soundEffectProgressBarSize = soundEffectSlider->getContentSize();
    soundEffectProgressBarSize.width *= 0.5f; // Adjust the width to be 90% of the original
    soundEffectSlider->setContentSize(soundEffectProgressBarSize);

    // Restore the saved value
    float savedSoundEffectVolume = UserDefault::getInstance()->getFloatForKey(Constants::UD_effectsVolume.c_str(), 0.5f);
    soundEffectSlider->setPercent(savedSoundEffectVolume * 100);

    soundEffectSlider->setPosition(Vec2(musicSlider->getPosition().x, musicSlider->getPosition().y - SpriteController::calculateScreenRatio(Constants::PADDING_VERTICAL_UI_SLIDER_SPACING)));
    soundEffectSlider->addEventListener([this](Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            float volume = soundEffectSlider->getPercent() / 100.0f;
            SoundController::getInstance()->setEffectsVolume(volume);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_effectsVolume.c_str(), volume); // Save the value
            updateSoundEffectButtonState();
        }
        });
    boardSprite->addChild(soundEffectSlider);

    // Create music button
    musicButton = ui::Button::create("assets_game/UXUI/Panel/Music_BTN_Active.png", "assets_game/UXUI/Panel/Music_BTN_Active.png");
    musicButton->setScale(SpriteController::updateSpriteScale(musicButton, 0.05f));
    musicButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    musicButton->setPosition(Vec2(musicSlider->getPosition().x - SpriteController::calculateScreenRatio(Constants::PADDING_HORIZONTAL_UI_SLIDER_SPACING),
        musicSlider->getPosition().y));
    musicButton->addClickEventListener([this](Ref* sender) {
        if (musicSlider->getPercent() == 0) {
            musicSlider->setPercent(100);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_musicVolume.c_str(), 100);
            SoundController::getInstance()->setMusicVolume(1.0f);
        }
        else {
            musicSlider->setPercent(0);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_musicVolume.c_str(), 0);
            SoundController::getInstance()->setMusicVolume(0);
        }
        updateMusicButtonState();
        });

    boardSprite->addChild(musicButton);

    // Create sound effect button
    soundEffectButton = ui::Button::create("assets_game/UXUI/Panel/SoundEffect_BTN_Active.png", "assets_game/UXUI/Panel/SoundEffect_BTN_Active.png");
    soundEffectButton->setScale(SpriteController::updateSpriteScale(soundEffectButton, 0.05f));
    soundEffectButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    soundEffectButton->setPosition(Vec2(musicButton->getPosition().x, soundEffectSlider->getPosition().y));
    soundEffectButton->addClickEventListener([this](Ref* sender) {
        if (soundEffectSlider->getPercent() == 0) {
            soundEffectSlider->setPercent(100);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_effectsVolume.c_str(), 100);
            SoundController::getInstance()->setEffectsVolume(1.0f);
        }
        else {
            soundEffectSlider->setPercent(0);
            UserDefault::getInstance()->setFloatForKey(Constants::UD_effectsVolume.c_str(), 0);
            SoundController::getInstance()->setEffectsVolume(0);
        }
        updateSoundEffectButtonState();
        });

    boardSprite->addChild(soundEffectButton);

    // Update button states based on initial slider values
    updateSlidersAndButtons();

    this->addTitleLabel("Setting");

    return true;
}

bool SettingPanel::init() {
    return init(nullptr, nullptr);
}

void SettingPanel::playSoundAndExecuteCallback(const std::function<void()>& callback) {
    if (callback) {
        callback();
    }
}

void SettingPanel::updateMusicButtonState() {
    if (musicSlider->getPercent() == 0) {
        musicButton->loadTextures("assets_game/UXUI/Panel/Music_BTN_Disabled.png", "assets_game/UXUI/Panel/Music_BTN_Disabled.png");
    }
    else {
        musicButton->loadTextures("assets_game/UXUI/Panel/Music_BTN_Active.png", "assets_game/UXUI/Panel/Music_BTN_Active.png");
    }
}

void SettingPanel::updateSoundEffectButtonState() {
    if (soundEffectSlider->getPercent() == 0) {
        soundEffectButton->loadTextures("assets_game/UXUI/Panel/SoundEffect_BTN_Disabled.png", "assets_game/UXUI/Panel/SoundEffect_BTN_Disabled.png");
    }
    else {
        soundEffectButton->loadTextures("assets_game/UXUI/Panel/SoundEffect_BTN_Active.png", "assets_game/UXUI/Panel/SoundEffect_BTN_Active.png");
    }
}

void SettingPanel::updateSlidersAndButtons() {
    float savedMusicVolume = UserDefault::getInstance()->getFloatForKey(Constants::UD_musicVolume.c_str(), 0.5f);
    float savedEffectsVolume = UserDefault::getInstance()->getFloatForKey(Constants::UD_effectsVolume.c_str(), 0.5f);

    musicSlider->setPercent(savedMusicVolume * 100);
    soundEffectSlider->setPercent(savedEffectsVolume * 100);

    updateMusicButtonState();
    updateSoundEffectButtonState();
}