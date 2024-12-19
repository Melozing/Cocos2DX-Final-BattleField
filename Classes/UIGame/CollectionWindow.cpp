#include "UIGame/CollectionWindow.h"

USING_NS_CC;

CollectionWindow* CollectionWindow::create() {
    CollectionWindow* ret = new (std::nothrow) CollectionWindow();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

bool CollectionWindow::init() {
    if (!Layer::init()) {
        return false;
    }

    createMainWindow();
    createLayer1();
    createLayer2();

    return true;
}

void CollectionWindow::createMainWindow() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create main window background
    auto mainWindow = ui::Scale9Sprite::create("assets_game/UXUI/Collection/c_full_header.png");
    mainWindow->setContentSize(Size(visibleSize.width * 0.8, visibleSize.height * 0.8));
    mainWindow->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(mainWindow);

    // Create close button
    auto closeButton = ui::Button::create("assets_game/UXUI/Collection/c_header_close.png");
    closeButton->setPosition(Vec2(mainWindow->getContentSize().width - closeButton->getContentSize().width / 2, mainWindow->getContentSize().height - closeButton->getContentSize().height / 2));
    closeButton->addTouchEventListener(CC_CALLBACK_2(CollectionWindow::closeWindow, this));
    mainWindow->addChild(closeButton);

    // Create title label
    auto titleLabel = Label::createWithTTF("Library", "fonts/Marker Felt.ttf", 24);
    titleLabel->setPosition(Vec2(mainWindow->getContentSize().width / 2, mainWindow->getContentSize().height - titleLabel->getContentSize().height));
    mainWindow->addChild(titleLabel);
}

void CollectionWindow::createLayer1() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create scroll view for items
    scrollView = ui::ScrollView::create();
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(Size(visibleSize.width * 0.25, visibleSize.height * 0.7));
    scrollView->setInnerContainerSize(Size(visibleSize.width * 0.25, visibleSize.height * 1.4)); // Adjust inner container size as needed
    scrollView->setPosition(Vec2(visibleSize.width * 0.1 + origin.x, visibleSize.height * 0.1 + origin.y));
    this->addChild(scrollView);

    // Add item buttons to scroll view
    for (int i = 0; i < 10; ++i) { // Example with 10 items
        auto itemButton = ui::Button::create("assets_game/items/IncreaseBulletCountItem.png");
        itemButton->setPosition(Vec2(scrollView->getContentSize().width / 2, scrollView->getInnerContainerSize().height - (i + 1) * itemButton->getContentSize().height));
        itemButton->addTouchEventListener(CC_CALLBACK_2(CollectionWindow::onItemClicked, this));
        scrollView->addChild(itemButton);
    }
}

void CollectionWindow::createLayer2() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create label to display item information
    itemInfoLabel = Label::createWithTTF("Item Info", "fonts/Marker Felt.ttf", 24);
    itemInfoLabel->setPosition(Vec2(visibleSize.width * 0.65 + origin.x, visibleSize.height * 0.5 + origin.y));
    this->addChild(itemInfoLabel);
}

void CollectionWindow::closeWindow(Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        this->removeFromParentAndCleanup(true);
    }
}

void CollectionWindow::onItemClicked(Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        // Update item info label with the clicked item's information
        itemInfoLabel->setString("Item Info: " + std::to_string(rand() % 100)); // Example info
    }
}
