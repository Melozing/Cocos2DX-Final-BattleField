#ifndef __SETTING_PANEL_H__
#define __SETTING_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Panel/BasePanel.h"

class SettingPanel : public BasePanel {
public:
    static SettingPanel* createPanel(const std::function<void()>& exitAction, const std::function<void()>& quitAction);
    virtual bool init(const std::function<void()>& exitAction, const std::function<void()>& quitAction);
    virtual bool init() override;

private:
    void playSoundAndExecuteCallback(const std::function<void()>& callback);
    void updateMusicButtonState();
    void updateSoundEffectButtonState();
    void updateSlidersAndButtons();

    cocos2d::ui::Slider* musicSlider;
    cocos2d::ui::Slider* soundEffectSlider;
    cocos2d::ui::Button* musicButton;
    cocos2d::ui::Button* soundEffectButton;
    cocos2d::ui::Button* exitButton;
    cocos2d::ui::Button* quitButton;
};

#endif // __SETTING_PANEL_H__
