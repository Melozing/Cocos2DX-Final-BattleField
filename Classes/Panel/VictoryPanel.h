#ifndef __VICTORY_PANEL_H__
#define __VICTORY_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Panel/BasePanel.h"

class VictoryPanel : public BasePanel {
public:
    static VictoryPanel* createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction);
    virtual bool init(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction);
};

#endif // __VICTORY_PANEL_H__
