#include "BaseButton.h"

USING_NS_CC;

bool BaseButton::init(const std::string& normalImage, const std::string& selectedImage, const std::function<void()>& callback) {
    if (!Button::init(normalImage, selectedImage)) {
        return false;
    }

    this->addClickEventListener([callback](Ref* sender) {
        callback();
        });

    return true;
}

void BaseButton::updateScale(float size) {
    float scale = SpriteController::updateSpriteScale(this, size);
    this->setScale(scale);
}
