#ifndef __GAME_OVER_PANEL_H__
#define __GAME_OVER_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameOverPanel : public cocos2d::Layer {
public:
    static GameOverPanel* createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction);
    virtual bool init(const std::function<void()>& retryAction, const std::function<void()>& exitAction);
    static GameOverPanel* create(const std::function<void()>& retryAction, const std::function<void()>& exitAction);
};

#endif // __GAME_OVER_PANEL_H__
