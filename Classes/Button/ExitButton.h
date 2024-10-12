#ifndef __EXIT_BUTTON_H__
#define __EXIT_BUTTON_H__

#include "BaseButton.h"

class ExitButton : public BaseButton {
public:
    static ExitButton* create(const std::function<void()>& callback);
    bool init(const std::function<void()>& callback);
};

#endif // __EXIT_BUTTON_H__
