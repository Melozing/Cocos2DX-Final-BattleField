#include "cocos2d.h"
#include "ui/CocosGUI.h"

class VictoryPanel : public cocos2d::Layer {
public:
    static VictoryPanel* createPanel(const std::function<void()>& replayAction, const std::function<void()>& exitAction);
    virtual bool init();
    CREATE_FUNC(VictoryPanel);

private:
    std::function<void()> _replayAction;
    std::function<void()> _exitAction;
};