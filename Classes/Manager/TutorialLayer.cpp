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

    // Create a semi-transparent background
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(background);

    this->slideImages = slideImages;
    this->userDefaultKey = userDefaultKey;
    this->currentSlideIndex = 0;
    this->buttonsPositionUpdated = false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // Create border sprite
    borderSprite = cocos2d::Sprite::create("assets_game/UXUI/Panel/Panel_Tutorial.png"); // Use your border image path
    borderSprite->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    this->addChild(borderSprite, Constants::ORDER_LAYER_UI - 1); // Add border sprite behind slideSprite

    // Create slide sprite
    slideSprite = cocos2d::Sprite::create(slideImages[currentSlideIndex]);
    originalSpriteSize = slideSprite->getContentSize();
    slideSprite->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    this->addChild(slideSprite, Constants::ORDER_LAYER_UI);


    // Create buttons and labels
    this->createButtonsAndLabels();

    this->updateSlide();


    // Notify to hide cursor
    __NotificationCenter::getInstance()->postNotification("HideCursorNotification");
    GameController::getInstance()->toggleCursorVisibility(true);

    // Pause the game after the tutorial layer is added
    this->runAction(cocos2d::Sequence::create(
        DelayTime::create(1.0f),
        cocos2d::CallFunc::create([this]() {
            this->pauseGame();
            __NotificationCenter::getInstance()->postNotification("DisableJoystickNotification");
            }),
        nullptr));

    return true;
}

void TutorialLayer::createButtonsAndLabels() {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    // Create prev button
    prevButton = cocos2d::ui::Button::create(
        "assets_game/UXUI/Panel/back_active.png",
        "assets_game/UXUI/Panel/back_inactive.png");
    prevButton->setScale(SpriteController::updateSpriteScale(prevButton, 0.1));
    prevButton->addClickEventListener(CC_CALLBACK_0(TutorialLayer::onPrevClicked, this));
    this->addChild(prevButton, Constants::ORDER_LAYER_UI);

    // Create next button
    nextButton = cocos2d::ui::Button::create(
        "assets_game/UXUI/Panel/prev_active.png",
        "assets_game/UXUI/Panel/prev_inactive.png");
    nextButton->setScale(SpriteController::updateSpriteScale(nextButton, 0.1));
    nextButton->addClickEventListener(CC_CALLBACK_0(TutorialLayer::onNextClicked, this));
    this->addChild(nextButton, Constants::ORDER_LAYER_UI);

    // Create checkbox and label
    dontShowAgainCheckbox = cocos2d::ui::CheckBox::create(
        "assets_game/UXUI/Panel/check_box_inactive.png",
        "assets_game/UXUI/Panel/check_box_active.png");
    dontShowAgainCheckbox->setScale(SpriteController::updateSpriteScale(dontShowAgainCheckbox, 0.05));
    dontShowAgainCheckbox->addEventListener(CC_CALLBACK_2(TutorialLayer::onCheckboxClicked, this));
    this->addChild(dontShowAgainCheckbox, Constants::ORDER_LAYER_UI);

    label = cocos2d::Label::createWithTTF("Don't show again", Constants::FONT_GAME, 10);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(dontShowAgainCheckbox->getPosition());
    this->addChild(label, Constants::ORDER_LAYER_UI);

    // Create skip button
    skipButton = cocos2d::ui::Button::create(
        "assets_game/UXUI/Panel/skip_button.png",
        "assets_game/UXUI/Panel/skip_button.png");
    skipButton->setScale(SpriteController::updateSpriteScale(skipButton, 0.1));
    skipButton->addClickEventListener(CC_CALLBACK_0(TutorialLayer::onSkipClicked, this));
    this->addChild(skipButton, Constants::ORDER_LAYER_UI);
}

void TutorialLayer::updateSlide() {
    // Load the new texture
    slideSprite->setTexture(slideImages[currentSlideIndex]);

    // Set the size of the slide sprite to a fixed size
    auto fixedSize = cocos2d::Size(SpriteController::calculateScreenRatio(0.6f), SpriteController::calculateScreenHeightRatio(0.93f)); // Set your desired fixed size here

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

    // Adjust the border sprite size and position
    borderSprite->setScale(scale * 1.615f); // Adjust the scale factor as needed to fit around the slideSprite
    borderSprite->setPosition(slideSprite->getPosition());

    // Update the state of the prev and next buttons
    prevButton->setEnabled(currentSlideIndex > 0);
    nextButton->setEnabled(currentSlideIndex < slideImages.size() - 1);

    // Update button positions only once
    if (!buttonsPositionUpdated) {
        float paddingHorizontal = SpriteController::calculateScreenRatio(0.05f);
        prevButton->setPosition(cocos2d::Vec2(slideSprite->getPosition().x - slideSprite->getContentSize().width * slideSprite->getScale() / 2 - paddingHorizontal, slideSprite->getPosition().y));
        nextButton->setPosition(cocos2d::Vec2(slideSprite->getPosition().x + slideSprite->getContentSize().width * slideSprite->getScale() / 2 + paddingHorizontal, slideSprite->getPosition().y));
        float paddingTop = SpriteController::calculateScreenRatio(0.05f);
        float paddingWidth = SpriteController::calculateScreenRatio(0.05f);
        float paddingWidthSkip = SpriteController::calculateScreenRatio(0.04f);
        dontShowAgainCheckbox->setPosition(cocos2d::Vec2(borderSprite->getPosition().x / 2 - paddingWidth, slideSprite->getPosition().y - this->GetSize().height / 2 - paddingTop));
        label->setPosition(dontShowAgainCheckbox->getPosition() + cocos2d::Vec2(SpriteController::calculateScreenRatio(0.053f), 0));
        skipButton->setPosition(cocos2d::Vec2(slideSprite->getPosition().x + this->GetSize().width / 2 - paddingWidthSkip, slideSprite->getPosition().y - this->GetSize().height / 2 - paddingTop));
        __NotificationCenter::getInstance()->postNotification("HideCursorNotification");
        buttonsPositionUpdated = true;
    }
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
    return SpriteController::GetContentSize(slideSprite);
}

void TutorialLayer::pauseGame() {
    cocos2d::Director::getInstance()->pause();
    SoundController::getInstance()->pauseMusic(Constants::currentSoundTrackPath);
}

void TutorialLayer::onEnter() {
    Layer::onEnter();
    GameController::getInstance()->setTutorialLayerActive(true);
}

TutorialLayer::~TutorialLayer() {
    GameController::getInstance()->setTutorialLayerActive(false);
    GameController::getInstance()->toggleCursorVisibility(false);
    SoundController::getInstance()->resumeMusic(Constants::currentSoundTrackPath);

    // Notify to show cursor
    __NotificationCenter::getInstance()->postNotification("ShowCursorNotification");
    __NotificationCenter::getInstance()->postNotification("EnableJoystickNotification", (Ref*)true);
}