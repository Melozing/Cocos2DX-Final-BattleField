#include "UIGame/LibraryWindow.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "json/document.h"
#include "json/filereadstream.h"

USING_NS_CC;
using namespace ui;
using namespace rapidjson;

bool LibraryWindow::init() {
    if (!Layer::init()) {
        return false;
    }
    createBackground();
    createItemSection();
    createInfoSection();
    createExitButton();
    loadItemsFromJson();

    return true;
}

void LibraryWindow::createBackground() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    mainBg = Sprite::create("assets_game/UXUI/Collection/c_full_header.png");
    mainBg->setContentSize(Size(visibleSize.width * 0.8, visibleSize.height * 0.8));
    mainBg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(mainBg);
}

void LibraryWindow::createExitButton() {
    auto exitButton = Button::create("assets_game/UXUI/Collection/c_header_close.png", "assets_game/UXUI/Collection/c_header_close-PUSH.png");
    exitButton->setScale(0.6);
    exitButton->setPosition(Vec2(mainBg->getContentSize().width - 40, mainBg->getContentSize().height - 30));
    exitButton->addClickEventListener([=](Ref* sender) {
        this->removeFromParent();
        });
    mainBg->addChild(exitButton);
}

void LibraryWindow::createItemSection() {
    auto itemsBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    itemsBg->setContentSize(Size(mainBg->getContentSize().width / 3 - 10, mainBg->getContentSize().height - 60));
    itemsBg->setAnchorPoint(Vec2(0, 0));
    itemsBg->setPosition(Vec2(5, 5));
    mainBg->addChild(itemsBg);

    scrollView = ScrollView::create();
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(itemsBg->getContentSize());
    scrollView->setInnerContainerSize(Size(itemsBg->getContentSize().width, 600));
    scrollView->setBounceEnabled(true);
    itemsBg->addChild(scrollView);
}

void LibraryWindow::createInfoSection() {
    auto infoBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    infoBg->setContentSize(Size(mainBg->getContentSize().width * 2 / 3 - 10, mainBg->getContentSize().height - 60));
    infoBg->setAnchorPoint(Vec2(0, 0));
    infoBg->setPosition(Vec2(mainBg->getContentSize().width / 3 + 5, 5));
    mainBg->addChild(infoBg);

    infoScrollView = ScrollView::create();
    infoScrollView->setDirection(ScrollView::Direction::VERTICAL);
    infoScrollView->setContentSize(infoBg->getContentSize() - Size(20, 20));
    infoScrollView->setInnerContainerSize(infoBg->getContentSize() - Size(20, 20));
    infoScrollView->setPosition(Vec2(10, 10));
    infoScrollView->setBounceEnabled(true);
    infoBg->addChild(infoScrollView);

    itemInfoLabel = Label::createWithSystemFont("Chon item de xem", "Arial", 20, Size(infoScrollView->getContentSize().width - 20, 0), TextHAlignment::LEFT, TextVAlignment::TOP);
    itemInfoLabel->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height - 10));
    itemInfoLabel->setAnchorPoint(Vec2(0.5, 1.0));
    infoScrollView->addChild(itemInfoLabel);
}

void LibraryWindow::updateItemInfo(const ItemData& item) {
    std::string info = "Name: " + item.name + "\n\n" + item.description;
    itemInfoLabel->setString(info);
    Size labelSize = itemInfoLabel->getContentSize();
    float newHeight = itemInfoLabel->getContentSize().height;
    if (newHeight > infoScrollView->getContentSize().height) {
        infoScrollView->setInnerContainerSize(Size(infoScrollView->getContentSize().width, newHeight + 20));
        itemInfoLabel->setPosition(Vec2(infoScrollView->getContentSize().width / 2, newHeight));
    }
    else {
        infoScrollView->setInnerContainerSize(infoScrollView->getContentSize());
        itemInfoLabel->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height));
    }
}

void LibraryWindow::loadItemsFromJson() {
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("json/items.json");
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(filePath);
    Document document;
    document.Parse(jsonContent.c_str());

    if (document.HasParseError() || !document.IsArray()) {
        CCLOG("Error parsing JSON or JSON is not an array");
        return;
    }

    for (auto& item : document.GetArray()) {
        ItemData data;
        data.id = item["id"].GetInt();
        data.imagePath = item["imagePath"].GetString();
        data.name = item["name"].GetString();
        data.description = item["description"].GetString();
        items.push_back(data);
    }

    int columns = 2;
    float paddingX = 20.0f;
    float paddingY = 20.0f;
    float itemWidth = (scrollView->getContentSize().width - (columns + 1) * paddingX) / columns;
    float itemHeight = itemWidth;
    int rows = (items.size() + columns - 1) / columns;
    float totalHeight = rows * (itemHeight + paddingY) + paddingY;
    scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, totalHeight));

    for (size_t i = 0; i < items.size(); ++i) {
        int row = i / columns;
        int col = i % columns;

        float posX = paddingX + col * (itemWidth + paddingX) + itemWidth / 2;
        float posY = totalHeight - (paddingY + row * (itemHeight + paddingY) + itemHeight / 2); // Điều chỉnh vị trí Y
        auto itemWidget = ItemWidget::create(items[i]);
        itemWidget->setPosition(Vec2(posX, posY));
        scrollView->addChild(itemWidget);
    }
}

// Implementation of ItemWidget
ItemWidget* ItemWidget::create(const ItemData& itemData) {
    ItemWidget* widget = new (std::nothrow) ItemWidget();
    if (widget && widget->init(itemData)) {
        widget->autorelease();
        return widget;
    }
    delete widget;
    return nullptr;
}

bool ItemWidget::init(const ItemData& itemData) {
    if (!Widget::init()) {
        return false;
    }

    itemBg = Sprite::create("assets_game/UXUI/Collection/item_frame.png");
    itemBg->setContentSize(Size(100, 100));
    this->addChild(itemBg);

    itemButton = Button::create(itemData.imagePath, itemData.imagePath);
    itemButton->setScale9Enabled(true);
    itemButton->setContentSize(Size(80, 80));
    itemButton->setPosition(Vec2(itemBg->getContentSize().width / 2, itemBg->getContentSize().height / 2));
    itemButton->addClickEventListener([=](Ref* sender) {
        // Handle item click
        LibraryWindow* libraryWindow = dynamic_cast<LibraryWindow*>(this->getParent()->getParent()->getParent());
        if (libraryWindow) {
            libraryWindow->updateItemInfo(itemData);
        }
        });
    itemBg->addChild(itemButton);

    itemNameLabel = Label::createWithSystemFont(itemData.name, "Arial", 14);
    itemNameLabel->setPosition(Vec2(itemBg->getContentSize().width / 2, -10));
    itemBg->addChild(itemNameLabel);

    return true;
}

void ItemWidget::updateItem(const ItemData& itemData) {
    itemButton->loadTextures(itemData.imagePath, itemData.imagePath);
    itemNameLabel->setString(itemData.name);
}