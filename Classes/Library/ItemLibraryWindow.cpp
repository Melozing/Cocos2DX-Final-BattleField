#include "Library/ItemLibraryWindow.h"
#include "Scene/MainMenuScene.h"
#include "ui/UIWidget.h"
USING_NS_CC;
using namespace ui;

bool ItemLibraryWindow::init() {
    if (!Layer::init()) {
        return false;
    }

    if (!ItemLibrary::getInstance().loadItemsFromXml("xml/items_library.xml")) {
        CCLOG("Failed to load items from XML");
        return false;
    }

    createBackground();
    createIntroSection(); // Call the new function for introduction
    createItemSection();
    createInfoSection();
    createExitButton();

    return true;
}

void ItemLibraryWindow::createBackground() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    mainBg = Sprite::create("assets_game/UXUI/Collection/c_full_header.png");
    if (!mainBg) {
        CCLOG("Failed to create mainBg sprite");
        return;
    }
    mainBg->setContentSize(Size(visibleSize.width * 0.8, visibleSize.height * 0.8));
    mainBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(mainBg);
}

cocos2d::Rect ItemLibraryWindow::getMainBgBoundingBox() const {
    return mainBg->getBoundingBox();
}

void ItemLibraryWindow::createIntroSection() {
    // Add introduction background image
    auto introBg = Sprite::create("assets_game/UXUI/Collection/intro.png");
    if (!introBg) {
        CCLOG("Failed to create introBg sprite");
        return;
    }
    const float scaleFactor = 0.7f; // 70%
    const Size mainBgSize = mainBg->getContentSize();
    const Size targetSize = Size(mainBgSize.width * scaleFactor, introBg->getContentSize().height);
    float scaleX = targetSize.width / introBg->getContentSize().width;
    float scaleY = targetSize.height / introBg->getContentSize().height;
    introBg->setScale(scaleX, scaleY);
    introBg->setPosition(Vec2(mainBgSize.width / 2, mainBgSize.height - introBg->getContentSize().height * scaleY / 2 - 50));
    mainBg->addChild(introBg);

    // Create intro label
    introLabel = Label::createWithSystemFont("Welcome to the Item Library", "Arial", 24);
    if (!introLabel) {
        CCLOG("Failed to create introLabel");
        return;
    }
    introLabel->setPosition(Vec2(introBg->getContentSize().width / 2, introBg->getContentSize().height / 2));
    introBg->addChild(introLabel);
}

void ItemLibraryWindow::createExitButton() {
    auto exitButton = Button::create("assets_game/UXUI/Collection/c_header_close.png", "assets_game/UXUI/Collection/c_header_close-PUSH.png");
    if (!exitButton) {
        CCLOG("Failed to create exitButton");
        return;
    }
    exitButton->setScale(0.6f);
    exitButton->setPosition(Vec2(mainBg->getContentSize().width - 40, mainBg->getContentSize().height - 35));
    exitButton->addClickEventListener([=](Ref* sender) {
        auto mainMenuScene = dynamic_cast<MainMenu*>(Director::getInstance()->getRunningScene());
        if (mainMenuScene) {
            mainMenuScene->toggleItemLibraryWindow();
        }
        });
    mainBg->addChild(exitButton);
}

void ItemLibraryWindow::createItemSection() {
    auto itemsBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    if (!itemsBg) {
        return;
    }
    itemsBg->setContentSize(Size(mainBg->getContentSize().width / 3 - 10, mainBg->getContentSize().height - 60));
    itemsBg->setAnchorPoint(Vec2(-0.04f, 0));
    itemsBg->setPosition(Vec2(5, 5));
    mainBg->addChild(itemsBg);

    scrollView = ScrollView::create();
    if (!scrollView) {
        return;
    }
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(itemsBg->getContentSize());
    scrollView->setBounceEnabled(true);
    scrollView->setScrollBarEnabled(false);
    scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    scrollView->setPosition(Vec2(itemsBg->getContentSize().width / 2, itemsBg->getContentSize().height / 2 - 15));
    itemsBg->addChild(scrollView);

    const auto& items = ItemLibrary::getInstance().getItems();
    int columns = 2;
    float paddingX = 20.0f;
    float paddingY = 20.0f;
    float itemWidth = (itemsBg->getContentSize().width - (columns + 1) * paddingX) / columns;
    float itemHeight = itemWidth;
    int rows = (items.size() + columns - 1) / columns;
    float totalHeight = rows * (itemHeight + paddingY) + paddingY;
    scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, totalHeight));

    for (size_t i = 0; i < items.size(); ++i) {
        int row = i / columns;
        int col = i % columns;

        float posX = paddingX + col * (itemWidth + paddingX) + itemWidth / 2;
        float posY = totalHeight - paddingY - row * (itemHeight + paddingY) - itemHeight / 2;
        auto itemLayout = UILibrary::createItemLayout(items[i], [=]() {
            updateItemInfo(items[i]);
            });
        if (!itemLayout) {
            CCLOG("Failed to create itemLayout for item %d", items[i].id);
            continue;
        }
        itemLayout->setAnchorPoint(Vec2(0.5, 0.5));
        itemLayout->setPosition(Vec2(posX, posY));
        scrollView->addChild(itemLayout);
    }
}

void ItemLibraryWindow::createInfoSection() {
    auto infoBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    if (!infoBg) {
        CCLOG("Failed to create infoBg sprite");
        return;
    }
    infoBg->setContentSize(Size(mainBg->getContentSize().width * 2 / 3 - 10, mainBg->getContentSize().height - 60));
    infoBg->setAnchorPoint(Vec2(0, 0));
    infoBg->setPosition(Vec2(mainBg->getContentSize().width / 3, 5));
    mainBg->addChild(infoBg);

    infoScrollView = ScrollView::create();
    if (!infoScrollView) {
        return;
    }
    infoScrollView->setDirection(ScrollView::Direction::VERTICAL);
    infoScrollView->setContentSize(infoBg->getContentSize() - Size(20, 20));
    infoScrollView->setBounceEnabled(true);
    infoScrollView->setScrollBarEnabled(false);
    infoScrollView->setAnchorPoint(Vec2(0.5, 0.5));
    infoScrollView->setPosition(Vec2(infoBg->getContentSize().width / 2, infoBg->getContentSize().height / 2));
    infoBg->addChild(infoScrollView);

    // Create custom scroll bar background
    auto scrollBarBg = Sprite::create("assets_game/UXUI/Collection/Scroll Bar/scroll_bar.png");
    scrollBarBg->setAnchorPoint(Vec2(1.0, 0.5));
    scrollBarBg->setContentSize(Size(scrollBarBg->getContentSize().width, infoBg->getContentSize().height - 20)); // Adjust height
    scrollBarBg->setPosition(Vec2(infoBg->getContentSize().width - 5, infoBg->getContentSize().height / 2));
    infoBg->addChild(scrollBarBg);

    // Create custom scroll bar handler
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
        float maxScrollHeight = infoScrollView->getInnerContainerSize().height - infoScrollView->getContentSize().height;
        infoScrollView->setInnerContainerPosition(Vec2(0, -maxScrollHeight * percent));
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, scrollBarHandler);

    // Add scroll event listener to infoScrollView
    infoScrollView->addEventListener([=](Ref* sender, ScrollView::EventType type) {
        if (type == ScrollView::EventType::SCROLLING) {
            float percent = -infoScrollView->getInnerContainerPosition().y / (infoScrollView->getInnerContainerSize().height - infoScrollView->getContentSize().height);
            float newPos = percent * (scrollBarBg->getContentSize().height - scrollBarHandler->getContentSize().height) + scrollBarHandler->getContentSize().height / 2;
            scrollBarHandler->setPositionY(newPos);
        }
        });

    itemInfoImage = Sprite::create();
    if (!itemInfoImage) {
        CCLOG("Failed to create itemInfoImage");
        return;
    }
    itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height - 100));
    itemInfoImage->setAnchorPoint(Vec2(0.5, 1.0));
    infoScrollView->addChild(itemInfoImage);

    // Create intro label
    introLabel = Label::createWithSystemFont("", "Arial", 24);
    if (!introLabel) {
        CCLOG("Failed to create introLabel");
        return;
    }
    introLabel->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height - 50));
    infoScrollView->addChild(introLabel);
}

void ItemLibraryWindow::updateItemInfo(const ItemData& item) {
    std::string info = item.name + "\n\n" + item.description;
    introLabel->setString(info);

    itemInfoImage->setTexture(item.imagePath);
    Size imageSize = itemInfoImage->getContentSize();
    Size infoSize = infoScrollView->getContentSize();
    float scaleX = (infoSize.width - 20) / imageSize.width;
    float scaleY = (infoSize.height / 2 - 20) / imageSize.height;
    float scale = std::min(scaleX, scaleY);
    itemInfoImage->setScale(scale);

    Size labelSize = introLabel->getContentSize();
    float newHeight = labelSize.height + itemInfoImage->getContentSize().height * scale + 20;
    if (newHeight > infoScrollView->getContentSize().height) {
        infoScrollView->setInnerContainerSize(Size(infoScrollView->getContentSize().width, newHeight + 20));
        itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, newHeight - labelSize.height - 10));
    }
    else {
        infoScrollView->setInnerContainerSize(infoScrollView->getContentSize());
        itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height - labelSize.height - 10));
    }
}


