#ifndef TUTORIALLAYER_H
#define TUTORIALLAYER_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class TutorialLayer : public cocos2d::Layer {
public:
    static TutorialLayer* create(const std::vector<std::string>& slideImages, const std::string& userDefaultKey);
    virtual bool init(const std::vector<std::string>& slideImages, const std::string& userDefaultKey);
    void pauseGame();
    virtual void onEnter() override;
    virtual ~TutorialLayer();

private:
    void updateSlide();
    void onPrevClicked();
    void onNextClicked();
    void onSkipClicked();
    void onCheckboxClicked(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type);
    void createButtonsAndLabels();
    cocos2d::Size GetSize();
    std::vector<std::string> slideImages;
    int currentSlideIndex;
    cocos2d::Sprite* slideSprite;
    cocos2d::ui::Button* prevButton;
    cocos2d::ui::Button* nextButton;
    cocos2d::ui::Button* skipButton;
    cocos2d::ui::CheckBox* dontShowAgainCheckbox;
    cocos2d::Label* label;
    std::string userDefaultKey;
    cocos2d::Size originalSpriteSize;
    bool buttonsPositionUpdated;
    cocos2d::Sprite* borderSprite;
};

#endif // TUTORIALLAYER_H
