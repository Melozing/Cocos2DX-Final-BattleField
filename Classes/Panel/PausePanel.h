#ifndef __PAUSE_PANEL_H__
#define __PAUSE_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Panel/BasePanel.h"

class PausePanel : public BasePanel {
public:
    static PausePanel* createPanel(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction);
    virtual bool init(const std::function<void()>& resumeCallback, const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction);
    virtual bool init() override;
};

#endif // __PAUSE_PANEL_H__
