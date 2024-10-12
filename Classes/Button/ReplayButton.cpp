#include "ReplayButton.h"

USING_NS_CC;

ReplayButton* ReplayButton::create(const std::function<void()>& callback) {
    ReplayButton* button = new (std::nothrow) ReplayButton();
    if (button && button->init(callback)) {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

bool ReplayButton::init(const std::function<void()>& callback) {
    return BaseButton::init("assets_game/UXUI/Panel/Replay_BTN.png", "assets_game/UXUI/Panel/Replay_BTN_Active.png", callback);
}
