// PauseButton.h
#ifndef __PAUSE_BUTTON_H__
#define __PAUSE_BUTTON_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PauseButton : public cocos2d::Node {
public:
    virtual bool init();
    void pauseGame();
    void continueGame();
    bool isPaused() const;

    CREATE_FUNC(PauseButton);
};

#endif // __PAUSE_BUTTON_H__
