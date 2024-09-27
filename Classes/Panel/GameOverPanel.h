#ifndef GAMEOVERPANEL_H
#define GAMEOVERPANEL_H

#include "cocos2d.h"
#include <functional>

class GameOverPanel : public cocos2d::Layer {
public:
    static GameOverPanel* createPanel(const std::function<void()>& retryAction, const std::function<void()>& exitAction);

    virtual bool init(const std::function<void()>& retryAction, const std::function<void()>& exitAction);

private:
    std::function<void()> retryAction; // Callback for retry action
    std::function<void()> exitAction;  // Callback for exit action

    void setupUI(); // Setup UI elements

    void onRetryButtonClicked(cocos2d::Ref* sender); // Callback for retry button
    void onExitButtonClicked(cocos2d::Ref* sender);  // Callback for exit button

    void fadeOutAndRemove(); // Handle fade out and removal of the panel
};

#endif // GAMEOVERPANEL_H
