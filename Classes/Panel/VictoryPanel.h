#ifndef __VICTORY_PANEL_H__
#define __VICTORY_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class VictoryPanel : public cocos2d::Layer {
public:
    static VictoryPanel* createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction);
    virtual bool init(const std::function<void()>& retryAction, const std::function<void()>& exitAction);
    static VictoryPanel* create(const std::function<void()>& retryAction, const std::function<void()>& exitAction);
};

#endif // __VICTORY_PANEL_H__

