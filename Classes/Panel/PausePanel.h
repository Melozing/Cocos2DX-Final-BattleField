#ifndef __PAUSE_PANEL_H__
#define __PAUSE_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PausePanel : public cocos2d::ui::Layout {
public:
    static PausePanel* createPanel(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction);
    virtual bool init(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction);
};

#endif // __PAUSE_PANEL_H__
