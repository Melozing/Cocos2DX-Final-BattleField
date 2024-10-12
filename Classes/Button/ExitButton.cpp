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
    return BaseButton::init("assets_game/UXUI/Panel/Close_BTN.png", "assets_game/UXUI/Panel/Close_BTN.png", callback);
}
