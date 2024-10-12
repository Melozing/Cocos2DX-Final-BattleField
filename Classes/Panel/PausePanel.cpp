#include "PausePanel.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

PausePanel* PausePanel::createPanel(const std::function<void()>& resumeCallback) {
    PausePanel* panel = new (std::nothrow) PausePanel();
    if (panel && panel->init(resumeCallback)) {
        panel->autorelease();
        return panel;
    }
    delete panel;
    return nullptr;
}

bool PausePanel::init(const std::function<void()>& resumeCallback) {
    if (!Layout::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create background
    auto background = ui::Layout::create();
    background->setContentSize(visibleSize);
    background->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    background->setBackGroundColor(Color3B(0, 0, 0));
    background->setBackGroundColorOpacity(150);
    background->setName("PauseBackground"); // Set name for the background
    this->addChild(background);

    // Create resume button
    auto resumeButton = ui::Button::create("assets_game/UXUI/Buttons/Pause_BTN.png", "assets_game/UXUI/Buttons/Pause_BTN_Active.png");
    resumeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    resumeButton->addClickEventListener([resumeCallback](Ref* sender) {
        resumeCallback();
        });
    this->addChild(resumeButton);

    this->setName("PausePanel"); // Set name for the panel

    return true;
}
