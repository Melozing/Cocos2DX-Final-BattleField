#ifndef __REPLAY_BUTTON_H__
#define __REPLAY_BUTTON_H__

#include "BaseButton.h"

class ReplayButton : public BaseButton {
public:
    static ReplayButton* create(const std::function<void()>& callback);
    bool init(const std::function<void()>& callback);
};

#endif // __REPLAY_BUTTON_H__
