#ifndef __EXIT_BUTTON_H__
#define __EXIT_BUTTON_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ExitButton : public cocos2d::ui::Button {
public:
    static ExitButton* create(const std::function<void()>& callback);

protected:
    bool init(const std::function<void()>& callback);
};

#endif // __EXIT_BUTTON_H__
