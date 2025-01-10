#include "Library/UILibrary.h"

using namespace cocos2d;
using namespace ui;

Button* UILibrary::createItemButton(const ItemData& itemData, const std::function<void()>& callback) {
    auto button = Button::create("assets_game/UXUI/Collection/item_frame.png");
    button->setScale9Enabled(true);
    button->setContentSize(Size(100, 100));

    Director::getInstance()->getTextureCache()->addImageAsync(itemData.imagePath, [button, itemData](Texture2D* texture) {
        auto itemImage = Sprite::createWithTexture(texture);
        if (itemImage) {
            float scaleX = button->getContentSize().width / itemImage->getContentSize().width;
            float scaleY = button->getContentSize().height / itemImage->getContentSize().height;
            float scale = std::min(scaleX, scaleY);
            itemImage->setScale(scale);
            itemImage->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 2));
            button->addChild(itemImage);
        }
        else {
            CCLOG("Failed to create itemImage for item: %s", itemData.name.c_str());
        }
        });

    button->addClickEventListener([callback](Ref* sender) {
        SoundController::getInstance()->playSoundEffect(Constants::Library);
        callback();
        });

    return button;
}

Layout* UILibrary::createItemLayout(const ItemData& itemData, const std::function<void()>& callback) {
    auto layout = Layout::create();
    layout->setContentSize(Size(120, 120));

    auto button = createItemButton(itemData, callback);
    button->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
    layout->addChild(button);

    /*auto label = Label::createWithSystemFont(itemData.name, "Arial", 14);
    if (label) {
        label->setPosition(Vec2(layout->getContentSize().width / 2, -10));
        layout->addChild(label);
    }
    else {
        CCLOG("Failed to create label for item: %s", itemData.name.c_str());
    }*/

    return layout;
}

ScrollView* UILibrary::createScrollView(const Size& size, bool custom) {
    auto scrollView = ScrollView::create();
    if (scrollView) {
        scrollView->setDirection(ScrollView::Direction::VERTICAL);
        scrollView->setContentSize(size);
        scrollView->setInnerContainerSize(size);
        scrollView->setBounceEnabled(true);

        if (custom) {
            scrollView->setScrollBarEnabled(false); // Disable default scroll bar

            // Custom scroll bar background
            auto scrollBarBg = Sprite::create("assets_game/UXUI/Collection/Scroll Bar/scroll_bar.png");
            scrollBarBg->setAnchorPoint(Vec2(1.0, 0.5));
            scrollBarBg->setContentSize(Size(scrollBarBg->getContentSize().width, size.height - 20));
            scrollBarBg->setPosition(Vec2(size.width - 5, size.height / 2));
            scrollView->addChild(scrollBarBg);

            // Custom scroll bar handler
            auto scrollBarHandler = Sprite::create("assets_game/UXUI/Collection/Scroll Bar/scroll_handler-HOVER.png");
            scrollBarHandler->setAnchorPoint(Vec2(0.5, 0.5));
            scrollBarHandler->setPosition(Vec2(scrollBarBg->getContentSize().width / 2, scrollBarBg->getContentSize().height / 2));
            scrollBarBg->addChild(scrollBarHandler);

            // Add touch event listener to scroll bar handler
            auto touchListener = EventListenerTouchOneByOne::create();
            touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
                if (scrollBarHandler->getBoundingBox().containsPoint(touch->getLocation())) {
                    return true;
                }
                return false;
                };
            touchListener->onTouchMoved = [=](Touch* touch, Event* event) {
                auto delta = touch->getDelta();
                auto newPos = scrollBarHandler->getPositionY() + delta.y;
                newPos = std::max(newPos, scrollBarHandler->getContentSize().height / 2);
                newPos = std::min(newPos, scrollBarBg->getContentSize().height - scrollBarHandler->getContentSize().height / 2);
                scrollBarHandler->setPositionY(newPos);

                float percent = (newPos - scrollBarHandler->getContentSize().height / 2) / (scrollBarBg->getContentSize().height - scrollBarHandler->getContentSize().height);
                float maxScrollHeight = scrollView->getInnerContainerSize().height - scrollView->getContentSize().height;
                scrollView->setInnerContainerPosition(Vec2(0, -maxScrollHeight * percent));
                };
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, scrollBarHandler);

            // Add scroll event listener to scrollView
            scrollView->addEventListener([=](Ref* sender, ScrollView::EventType type) {
                if (type == ScrollView::EventType::SCROLLING) {
                    float percent = -scrollView->getInnerContainerPosition().y / (scrollView->getInnerContainerSize().height - scrollView->getContentSize().height);
                    float newPos = percent * (scrollBarBg->getContentSize().height - scrollBarHandler->getContentSize().height) + scrollBarHandler->getContentSize().height / 2;
                    scrollBarHandler->setPositionY(newPos);
                }
                });

            // Add mouse scroll event listener to scrollView
            auto mouseListener = EventListenerMouse::create();
            mouseListener->onMouseScroll = [=](Event* event) {
                EventMouse* e = (EventMouse*)event;
                if (scrollView->getBoundingBox().containsPoint(e->getLocationInView())) {
                    float delta = e->getScrollY() * 10; // Adjust scroll speed
                    auto newPos = scrollView->getInnerContainerPosition().y + delta;
                    newPos = std::max(newPos, scrollView->getContentSize().height - scrollView->getInnerContainerSize().height);
                    newPos = std::min(newPos, 0.0f);
                    scrollView->setInnerContainerPosition(Vec2(0, newPos));

                    float percent = -newPos / (scrollView->getInnerContainerSize().height - scrollView->getContentSize().height);
                    float handlerPos = percent * (scrollBarBg->getContentSize().height - scrollBarHandler->getContentSize().height) + scrollBarHandler->getContentSize().height / 2;
                    scrollBarHandler->setPositionY(handlerPos);
                }
                };
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, scrollView);
        }
    }
    else {
        CCLOG("Failed to create ScrollView");
    }
    return scrollView;
}

