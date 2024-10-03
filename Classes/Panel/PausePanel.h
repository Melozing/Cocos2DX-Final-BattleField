#ifndef __PAUSE_PANEL_H__
#define __PAUSE_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PausePanel : public cocos2d::ui::Layout {
public:
    static PausePanel* createPanel(const std::function<void()>& resumeCallback);

private:
    bool init(const std::function<void()>& resumeCallback);
};

#endif // __PAUSE_PANEL_H__