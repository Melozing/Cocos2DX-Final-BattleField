// Include necessary headers
#include "TutorialLayer.h"
#include "ui/CocosGUI.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"
#include "Controller/SoundController.h"
#include "Controller/GameController.h"

TutorialLayer* TutorialLayer::create(const std::vector<std::string>& slideImages, const std::string& userDefaultKey) {
    TutorialLayer* ret = new (std::nothrow) TutorialLayer();
    if (ret && ret->init(slideImages, userDefaultKey)) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

bool TutorialLayer::init(const std::vector<std::string>& slideImages, const std::string& userDefaultKey) {
    if (!Layer::init()) {
        return false;
    }

    this->slideImages = slideImages;
    this->userDefaultKey = userDefaultKey;
    this->currentSlideIndex = 0;
    this->buttonsPositionUpdated = false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // Create slide sprite
    slideSprite = cocos2d::Sprite::create(slideImages[currentSlideIndex]);
    originalSpriteSize = slideSprite->getContentSize();
    slideSprite->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    this->addChild(slideSprite, Constants::ORDER_LAYER_UI);


    // Create buttons and labels
    createButtonsAndLabels();

    updateSlide();


    // Notify to hide cursor
    __NotificationCenter::getInstance()->postNotification("HideCursorNotification");
    GameController::getInstance()->toggleCursorVisibility(true);

    return true;
}

void TutorialLayer::createButtonsAndLabels() {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    // Create prev button
    prevButton = cocos2d::ui::Button::create(
        "assets_game/UXUI/Collection/Scroll Bar/scroll_bot_arrow.png",
        "assets_game/UXUI/Collection/Scroll Bar/scroll_bot_arrow-HOVER.png");
    prevButton->setScale(SpriteController::updateSpriteScale(prevButton, 0.05));
    prevButton->addClickEventListener(CC_CALLBACK_0(TutorialLayer::onPrevClicked, this));
    this->addChild(prevButton, Constants::ORDER_LAYER_UI);

    // Create next button
    nextButton = cocos2d::ui::Button::create(
        "assets_game/UXUI/Collection/Scroll Bar/scroll_top_arrow.png",
        "assets_game/UXUI/Collection/Scroll Bar/scroll_top_arrow-HOVER.png");
    nextButton->setScale(SpriteController::updateSpriteScale(nextButton, 0.05));
    nextButton->addClickEventListener(CC_CALLBACK_0(TutorialLayer::onNextClicked, this));
    this->addChild(nextButton, Constants::ORDER_LAYER_UI);

    // Create checkbox and label
    dontShowAgainCheckbox = cocos2d::ui::CheckBox::create(
        "assets_game/UXUI/Panel/SliderNode_Normal.png",
        "assets_game/UXUI/Panel/SliderNode_Pressed.png");
    dontShowAgainCheckbox->setScale(SpriteController::updateSpriteScale(dontShowAgainCheckbox, 0.05));
    dontShowAgainCheckbox->addEventListener(CC_CALLBACK_2(TutorialLayer::onCheckboxClicked, this));
    this->addChild(dontShowAgainCheckbox, Constants::ORDER_LAYER_UI);

    label = cocos2d::Label::createWithTTF("Don't show again", Constants::FONT_GAME, 11);
    label->setPosition(dontShowAgainCheckbox->getPosition() + cocos2d::Vec2(100, 0));
    this->addChild(label, Constants::ORDER_LAYER_UI);

    // Create skip button
    skipButton = cocos2d::ui::Button::create(
        "assets_game/UXUI/Panel/Forward_BTN.png",
        "assets_game/UXUI/Panel/Forward_BTN_Active.png");
    skipButton->setScale(SpriteController::updateSpriteScale(skipButton, 0.05));
    skipButton->addClickEventListener(CC_CALLBACK_0(TutorialLayer::onSkipClicked, this));
    this->addChild(skipButton, Constants::ORDER_LAYER_UI);
}

void TutorialLayer::updateSlide() {
    // Load the new texture
    slideSprite->setTexture(slideImages[currentSlideIndex]);

    // Set the size of the slide sprite to a fixed size
    auto fixedSize = cocos2d::Size(SpriteController::calculateScreenRatio(0.3f), SpriteController::calculateScreenRatio(0.53f)); // Set your desired fixed size here

    // Get the original size of the new texture
    auto originalSize = slideSprite->getContentSize();

    // Calculate the scale factors to fit the original size into the fixed size
    float scaleX = fixedSize.width / originalSize.width;
    float scaleY = fixedSize.height / originalSize.height;
    float scale = std::min(scaleX, scaleY);

    // Apply the scale to the slide sprite
    slideSprite->setScale(scale);

    // Center the slide sprite
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    slideSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    // Update the state of the prev and next buttons
    prevButton->setEnabled(currentSlideIndex > 0);
    nextButton->setEnabled(currentSlideIndex < slideImages.size() - 1);

    // Update button positions only once
    if (!buttonsPositionUpdated) {
        float paddingTop = SpriteController::calculateScreenRatio(0.02f);
        float spacing = SpriteController::calculateScreenRatio(0.015f);
        prevButton->setPosition(cocos2d::Vec2(slideSprite->getPosition().x - spacing, slideSprite->getPosition().y - this->GetSize().height / 2 - paddingTop));
        nextButton->setPosition(cocos2d::Vec2(slideSprite->getPosition().x + spacing, slideSprite->getPosition().y - this->GetSize().height / 2 - paddingTop));
        dontShowAgainCheckbox->setPosition(cocos2d::Vec2(slideSprite->getPosition().x - this->GetSize().width / 2.2, slideSprite->getPosition().y - this->GetSize().height / 2 - paddingTop));
        label->setPosition(dontShowAgainCheckbox->getPosition() + cocos2d::Vec2(SpriteController::calculateScreenRatio(0.06f), 0));
        skipButton->setPosition(cocos2d::Vec2(slideSprite->getPosition().x + this->GetSize().width / 2.2, slideSprite->getPosition().y - this->GetSize().height / 2 - paddingTop));
        buttonsPositionUpdated = true;
    }
    // Pause the game after the tutorial layer is added
    this->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(1.0f), // Delay to ensure the layer is fully added
        cocos2d::CallFunc::create([this]() {
            this->pauseGame();
            }),
        nullptr));
}


void TutorialLayer::onPrevClicked() {
    if (currentSlideIndex > 0) {
        currentSlideIndex--;
        updateSlide();
    }
}

void TutorialLayer::onNextClicked() {
    if (currentSlideIndex < slideImages.size() - 1) {
        currentSlideIndex++;
        updateSlide();
    }
}

void TutorialLayer::onSkipClicked() {
    // Resume the game
    cocos2d::Director::getInstance()->resume();
    this->removeFromParentAndCleanup(true);
}

void TutorialLayer::onCheckboxClicked(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type) {
    if (type == cocos2d::ui::CheckBox::EventType::SELECTED) {
        // Save the preference to not show the tutorial again
        cocos2d::UserDefault::getInstance()->setBoolForKey(userDefaultKey.c_str(), true);
    }
    else if (type == cocos2d::ui::CheckBox::EventType::UNSELECTED) {
        // Save the preference to show the tutorial again
        cocos2d::UserDefault::getInstance()->setBoolForKey(userDefaultKey.c_str(), false);
    }
}

cocos2d::Size TutorialLayer::GetSize() {
    return SpriteController::GetContentSizeSprite(slideSprite);
}

void TutorialLayer::pauseGame() {
    cocos2d::Director::getInstance()->pause();
    SoundController::getInstance()->pauseMusic(Constants::currentSoundTrackPath);
}

void TutorialLayer::onEnter() {
    Layer::onEnter();
    GameController::getInstance()->setTutorialLayerActive(true);
}

void TutorialLayer::onExit() {
    Layer::onExit();
    GameController::getInstance()->setTutorialLayerActive(false);
    GameController::getInstance()->toggleCursorVisibility(false);
    SoundController::getInstance()->resumeMusic(Constants::currentSoundTrackPath);
    
    // Notify to show cursor
    __NotificationCenter::getInstance()->postNotification("ShowCursorNotification");
}