#include "Library/ItemLibraryWindow.h"

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

void ItemLibraryWindow::createExitButton() {
    auto exitButton = Button::create("assets_game/UXUI/Collection/c_header_close.png", "assets_game/UXUI/Collection/c_header_close-PUSH.png");
    if (!exitButton) {
        CCLOG("Failed to create exitButton");
        return;
    }
    exitButton->setScale(0.6f);
    exitButton->setPosition(Vec2(mainBg->getContentSize().width - 40, mainBg->getContentSize().height - 35));
    exitButton->addClickEventListener([=](Ref* sender) {
        this->removeFromParent();
        });
    mainBg->addChild(exitButton);
}

void ItemLibraryWindow::createItemSection() {
    auto itemsBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    if (!itemsBg) {
        return;
    }
    itemsBg->setContentSize(Size(mainBg->getContentSize().width / 3 - 10, mainBg->getContentSize().height - 60));
    itemsBg->setAnchorPoint(Vec2(0, 0));
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
    scrollView->setPosition(Vec2(itemsBg->getContentSize().width / 2, itemsBg->getContentSize().height / 2));
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
    infoBg->setPosition(Vec2(mainBg->getContentSize().width / 3 + 5, 5));
    mainBg->addChild(infoBg);

    infoScrollView = UILibrary::createInfoScrollView(infoBg->getContentSize() - Size(20, 20));
    if (!infoScrollView) {
        return;
    }
    infoScrollView->setPosition(Vec2(10, 10));
    infoBg->addChild(infoScrollView);

    itemInfoLabel = Label::createWithSystemFont("Chon item de xem", "Arial", 20, Size(infoScrollView->getContentSize().width - 20, 0), TextHAlignment::LEFT, TextVAlignment::TOP);
    if (!itemInfoLabel) {
        return;
    }
    itemInfoLabel->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height - 10));
    itemInfoLabel->setAnchorPoint(Vec2(0.5, 1.0));
    infoScrollView->addChild(itemInfoLabel);

    itemInfoImage = Sprite::create();
    if (!itemInfoImage) {
        CCLOG("Failed to create itemInfoImage");
        return;
    }
    itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height - 100));
    itemInfoImage->setAnchorPoint(Vec2(0.5, 1.0));
    infoScrollView->addChild(itemInfoImage);
}

void ItemLibraryWindow::updateItemInfo(const ItemData& item) {
    std::string info = "Name: " + item.name + "\n\n" + item.description;
    itemInfoLabel->setString(info);
    itemInfoImage->setTexture(item.imagePath);
    Size labelSize = itemInfoLabel->getContentSize();
    float newHeight = itemInfoLabel->getContentSize().height + itemInfoImage->getContentSize().height + 20;
    if (newHeight > infoScrollView->getContentSize().height) {
        infoScrollView->setInnerContainerSize(Size(infoScrollView->getContentSize().width, newHeight + 20));
        itemInfoLabel->setPosition(Vec2(infoScrollView->getContentSize().width / 2, newHeight));
        itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, newHeight - itemInfoLabel->getContentSize().height - 10));
    }
    else {
        infoScrollView->setInnerContainerSize(infoScrollView->getContentSize());
        itemInfoLabel->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height));
        itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height - itemInfoLabel->getContentSize().height - 10));
    }
}
