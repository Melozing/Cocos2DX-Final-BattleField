#ifndef __GAME_OVER_PANEL_H__
#define __GAME_OVER_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Panel/BasePanel.h"

class GameOverPanel : public BasePanel {
public:
    static GameOverPanel* createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction);
    virtual bool init(const std::function<void()>& retryAction, const std::function<void()>& exitAction, const std::function<void()>& backAction);
};

#endif // __GAME_OVER_PANEL_H__
