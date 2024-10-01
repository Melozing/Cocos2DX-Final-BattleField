// GameOverPanel.h
#ifndef __GAME_OVER_PANEL_H__
#define __GAME_OVER_PANEL_H__

#include "cocos2d.h"

class GameOverPanel : public cocos2d::Layer {
public:
    static GameOverPanel* createPanel(const std::function<void()>& retryCallback, const std::function<void()>& exitCallback);

private:
    bool init(const std::function<void()>& retryCallback, const std::function<void()>& exitCallback);
};

#endif // __GAME_OVER_PANEL_H__
