#ifndef __PAUSE_BUTTON_H__
#define __PAUSE_BUTTON_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PauseButton : public cocos2d::Node {
public:
    virtual bool init() override;
    void pauseGame();
    void continueGame();
    bool isPaused() const;
    CREATE_FUNC(PauseButton);

private:
    cocos2d::ui::Layout* _pausePanel;
    bool _paused = false; // Add this member to track pause state
};

#endif // __PAUSE_BUTTON_H__
