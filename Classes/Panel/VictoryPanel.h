#ifndef __VICTORY_PANEL_H__
#define __VICTORY_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class VictoryPanel : public cocos2d::Layer {
public:
    static VictoryPanel* createPanel(const std::function<void()>& replayAction, const std::function<void()>& exitAction);

    virtual bool init(const std::function<void()>& replayAction, const std::function<void()>& exitAction);

    // Implement the "static create()" method manually
    static VictoryPanel* create(const std::function<void()>& replayAction, const std::function<void()>& exitAction);

private:
    std::function<void()> _replayAction;
    std::function<void()> _exitAction;
};

#endif // __VICTORY_PANEL_H__
