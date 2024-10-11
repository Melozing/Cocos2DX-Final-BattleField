#ifndef __REPLAY_BUTTON_H__
#define __REPLAY_BUTTON_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ReplayButton : public cocos2d::ui::Button {
public:
    static ReplayButton* create(const std::function<void()>& callback);

protected:
    bool init(const std::function<void()>& callback);
};

#endif // __REPLAY_BUTTON_H__
