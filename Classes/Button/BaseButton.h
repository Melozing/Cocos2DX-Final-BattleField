#ifndef __BASE_BUTTON_H__
#define __BASE_BUTTON_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Controller/SpriteController.h"

class BaseButton : public cocos2d::ui::Button {
public:
    virtual bool init(const std::string& normalImage, const std::string& selectedImage, const std::function<void()>& callback);
    void updateScale(float size);
};

#endif // __BASE_BUTTON_H__
