#include "Library/UILibrary.h"

using namespace cocos2d;
using namespace ui;

Button* UILibrary::createItemButton(const ItemData& itemData, const std::function<void()>& callback) {
    // Create the button with the item frame
    auto button = Button::create("assets_game/UXUI/Collection/item_frame.png");
    button->setScale9Enabled(true);
    button->setContentSize(Size(100, 100)); // Set the size of the item frame

    // Create the item image and add it to the button
    auto itemImage = Sprite::create(itemData.imagePath);
    if (itemImage) {
        float scaleX = button->getContentSize().width / itemImage->getContentSize().width;
        float scaleY = button->getContentSize().height / itemImage->getContentSize().height;
        float scale = std::min(scaleX, scaleY); // Maintain aspect ratio
        itemImage->setScale(scale);
        itemImage->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 2));
        button->addChild(itemImage);
    }

    button->addClickEventListener([callback](Ref* sender) {
        callback();
        });

    return button;
}

Layout* UILibrary::createItemLayout(const ItemData& itemData, const std::function<void()>& callback) {
    auto layout = Layout::create();
    layout->setContentSize(Size(120, 120)); // Set the size of the layout

    auto button = createItemButton(itemData, callback);
    button->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
    layout->addChild(button);

    auto label = Label::createWithSystemFont(itemData.name, "Arial", 14);
    label->setPosition(Vec2(layout->getContentSize().width / 2, -10));
    layout->addChild(label);

    return layout;
}

ScrollView* UILibrary::createInfoScrollView(const Size& size) {
    auto scrollView = ScrollView::create();
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(size);
    scrollView->setInnerContainerSize(size);
    scrollView->setBounceEnabled(true);
    return scrollView;
}



