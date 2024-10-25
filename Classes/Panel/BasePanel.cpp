#include "BasePanel.h"
#include "Constants/Constants.h"
#include "Controller/SpriteController.h"

USING_NS_CC;

bool BasePanel::init() {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Create a semi-transparent background
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(background);


    // Create board sprite
    boardSprite = Sprite::create("assets_game/UXUI/Panel/Board.png");
    boardSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    boardSprite->setScale(SpriteController::updateSpriteScale(boardSprite, 0.3f));
    boardSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(boardSprite);

    return true;
}