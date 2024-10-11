#include "ExitButton.h"

USING_NS_CC;

ExitButton* ExitButton::create(const std::function<void()>& callback) {
    ExitButton* button = new (std::nothrow) ExitButton();
    if (button && button->init(callback)) {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

bool ExitButton::init(const std::function<void()>& callback) {
    if (!Button::init("assets_game/UXUI/Panel/Exit_BTN.png", "assets_game/UXUI/Panel/Exit_BTN_Active.png")) {
        return false;
    }

    this->addClickEventListener([callback](Ref* sender) {
        callback();
        });

    return true;
}
