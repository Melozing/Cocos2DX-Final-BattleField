#include "Library/ItemLibraryWindow.h"
#include "Scene/MainMenuScene.h"
#include "ui/UIWidget.h"
#include "Controller/SoundController.h"
USING_NS_CC;
using namespace ui;

namespace {
    const float MAIN_BG_SCALE = 0.8f;
    const float INTRO_SCALE_FACTOR = 0.7f;
    const float INTRO_OFFSET_Y = 50.0f;
    const float ITEM_PADDING_X = 20.0f;
    const float ITEM_PADDING_Y = 20.0f;
    const float EXIT_BUTTON_SCALE = 0.6f;
    const float EXIT_BUTTON_OFFSET_X = 40.0f;
    const float EXIT_BUTTON_OFFSET_Y = 35.0f;
    const float ITEM_LAYOUT_WIDTH = 120.0f;
    const float ITEM_LAYOUT_HEIGHT = 120.0f;
    const float ITEM_LABEL_OFFSET_Y = -10.0f;
    const float INFO_IMAGE_OFFSET_Y = 100.0f;
    const float INFO_LABEL_OFFSET_Y = 50.0f;
    const float INFO_LABEL_FONT_SIZE = 24.0f;
    const float INFO_NAME_LABEL_FONT_SIZE = 28.0f;
    const float INFO_SHORT_DESC_LABEL_FONT_SIZE = 20.0f;
    const float INFO_DESC_LABEL_FONT_SIZE = 20.0f;
    const float INFO_LABEL_PADDING_Y = 20.0f;
}

bool ItemLibraryWindow::init() {
    if (!Layer::init()) {
        return false;
    }

    if (!ItemLibrary::getInstance().loadItemsFromXml("xml/items_library.xml")) {
        CCLOG("Failed to load items from XML");
        return false;
    }

    createBackground();
    createTitleLabel();
    createItemSection();
    createInfoSection();
    createExitButton();

    return true;
}

void ItemLibraryWindow::createBackground() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    mainBg = Sprite::create("assets_game/UXUI/Collection/c_full_header.png");
    if (!mainBg) {
        return;
    }
    mainBg->setContentSize(Size(visibleSize.width * MAIN_BG_SCALE, visibleSize.height * MAIN_BG_SCALE));
    mainBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(mainBg);
}

void ItemLibraryWindow::createTitleLabel() {
    Size mainBgSize = mainBg->getContentSize();
    auto titleLabel = Label::createWithSystemFont("Library", "Arial", INFO_LABEL_FONT_SIZE);
    if (!titleLabel) {
        CCLOG("Failed to create titleLabel");
        return;
    }
    titleLabel->setTextColor(Color4B::GREEN);
    titleLabel->enableBold();
    titleLabel->setPosition(Vec2(mainBgSize.width / 2, mainBgSize.height - EXIT_BUTTON_OFFSET_Y));
    mainBg->addChild(titleLabel);

    // Thêm EventListener cho titleLabel
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this, titleLabel](Touch* touch, Event* event) {
        if (titleLabel->getBoundingBox().containsPoint(touch->getLocation())) {
            SoundController::getInstance()->playSoundEffect(Constants::Library);
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, titleLabel);
}



cocos2d::Rect ItemLibraryWindow::getMainBgBoundingBox() const {
    return mainBg->getBoundingBox();
}

void ItemLibraryWindow::createIntroSection() {
    auto introBg = Sprite::create("assets_game/UXUI/Collection/intro.png");
    if (!introBg) {
        CCLOG("Failed to create introBg sprite");
        return;
    }
    const Size mainBgSize = mainBg->getContentSize();
    const Size targetSize = Size(mainBgSize.width * INTRO_SCALE_FACTOR, introBg->getContentSize().height);
    float scaleX = targetSize.width / introBg->getContentSize().width;
    float scaleY = targetSize.height / introBg->getContentSize().height;
    introBg->setScale(scaleX, scaleY);
    introBg->setPosition(Vec2(mainBgSize.width / 2, mainBgSize.height - introBg->getContentSize().height * scaleY / 2 - INTRO_OFFSET_Y));
    mainBg->addChild(introBg);

    introLabel = Label::createWithSystemFont("Welcome to the Item Library", "Arial", INFO_LABEL_FONT_SIZE);
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
    exitButton->setScale(EXIT_BUTTON_SCALE);
    exitButton->setPosition(Vec2(mainBg->getContentSize().width - EXIT_BUTTON_OFFSET_X, mainBg->getContentSize().height - EXIT_BUTTON_OFFSET_Y));
    exitButton->addClickEventListener([=](Ref* sender) {
        auto mainMenuScene = dynamic_cast<MainMenu*>(Director::getInstance()->getRunningScene());
        if (mainMenuScene) {
			SoundController::getInstance()->playSoundEffect(Constants::Library);
            mainMenuScene->toggleItemLibraryWindow();
        }
        });
    mainBg->addChild(exitButton);
}

void ItemLibraryWindow::createItemSection() {
    auto itemsBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    if (!itemsBg) {
        CCLOG("Failed to create itemsBg sprite");
        return;
    }
    const Size mainBgSize = mainBg->getContentSize();
    itemsBg->setContentSize(Size(mainBgSize.width / 3 - 10, mainBgSize.height - 60));
    itemsBg->setAnchorPoint(Vec2(-0.04f, 0));
    itemsBg->setPosition(Vec2(5, 5));
    mainBg->addChild(itemsBg);

    scrollView = ScrollView::create();
    if (!scrollView) {
        CCLOG("Failed to create scrollView");
        return;
    }
    const Size itemsBgSize = itemsBg->getContentSize();
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(itemsBgSize);
    scrollView->setInnerContainerSize(itemsBgSize);
    scrollView->setBounceEnabled(true);
    scrollView->setScrollBarEnabled(false); // Disable default scroll bar
    scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    scrollView->setPosition(Vec2(itemsBgSize.width / 2, itemsBgSize.height / 2 - 15));
    itemsBg->addChild(scrollView);

    const auto& items = ItemLibrary::getInstance().getItems();
    int columns = 2;
    float itemWidth = (itemsBgSize.width - (columns + 1) * ITEM_PADDING_X) / columns;
    float itemHeight = itemWidth;
    int rows = (items.size() + columns - 1) / columns;
    float totalHeight = rows * (itemHeight + ITEM_PADDING_Y) + ITEM_PADDING_Y;
    scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, totalHeight));

    for (size_t i = 0; i < items.size(); ++i) {
        int row = i / columns;
        int col = i % columns;

        float posX = ITEM_PADDING_X + col * (itemWidth + ITEM_PADDING_X) + itemWidth / 2;
        float posY = totalHeight - ITEM_PADDING_Y - row * (itemHeight + ITEM_PADDING_Y) - itemHeight / 2;
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
        }
        };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, scrollView);
}

void ItemLibraryWindow::createInfoSection() {
    auto infoBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    if (!infoBg) {
        CCLOG("Failed to create infoBg sprite");
        return;
    }
    const Size mainBgSize = mainBg->getContentSize();
    infoBg->setContentSize(Size(mainBgSize.width * 2 / 3 - 10, mainBgSize.height - 60));
    infoBg->setAnchorPoint(Vec2(0, 0));
    infoBg->setPosition(Vec2(mainBgSize.width / 3, 5));
    mainBg->addChild(infoBg);

    infoScrollView = UILibrary::createScrollView(infoBg->getContentSize() - Size(20, 20));
    if (!infoScrollView) {
        return;
    }
    infoScrollView->setAnchorPoint(Vec2(0.5, 0.5));
    infoScrollView->setPosition(Vec2(infoBg->getContentSize().width / 2, infoBg->getContentSize().height / 2));
    infoBg->addChild(infoScrollView);

    createIntroSection();

    itemInfoImage = Sprite::create();
    if (!itemInfoImage) {
        CCLOG("Failed to create itemInfoImage");
        return;
    }
    itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height - INFO_IMAGE_OFFSET_Y));
    itemInfoImage->setAnchorPoint(Vec2(0.5, 1.0));
    infoScrollView->addChild(itemInfoImage);
}

void ItemLibraryWindow::updateItemInfo(const ItemData& item) {
    Label* nameLabel = dynamic_cast<Label*>(infoScrollView->getChildByName("nameLabel"));
    if (!nameLabel) {
        nameLabel = Label::createWithSystemFont(item.name, "Arial", INFO_NAME_LABEL_FONT_SIZE);
        nameLabel->setAnchorPoint(Vec2(0, 1));
        nameLabel->setPosition(Vec2(10, infoScrollView->getContentSize().height - 10));
        nameLabel->setTextColor(Color4B::WHITE);
        nameLabel->enableBold();
        nameLabel->setName("nameLabel");
        infoScrollView->addChild(nameLabel);
    }
    else {
        nameLabel->setString(item.name);
    }

    Label* shortDescriptionLabel = dynamic_cast<Label*>(infoScrollView->getChildByName("shortDescriptionLabel"));
    if (!shortDescriptionLabel) {
        shortDescriptionLabel = Label::createWithSystemFont(item.shortDescription, "Arial", INFO_SHORT_DESC_LABEL_FONT_SIZE);
        shortDescriptionLabel->setAnchorPoint(Vec2(0, 1));
        shortDescriptionLabel->setPosition(Vec2(10, nameLabel->getPositionY() - nameLabel->getContentSize().height - 10));
        shortDescriptionLabel->setTextColor(Color4B::WHITE);
        shortDescriptionLabel->setName("shortDescriptionLabel");
        infoScrollView->addChild(shortDescriptionLabel);
    }
    else {
        shortDescriptionLabel->setString(item.shortDescription);
    }
    float contentStartY = shortDescriptionLabel->getPositionY() - shortDescriptionLabel->getContentSize().height - INFO_LABEL_PADDING_Y;

    if (!item.imageInfo.empty()) {
        itemInfoImage->setTexture(item.imageInfo);

        // Scale ảnh
        Size imageSize = itemInfoImage->getContentSize();
        float scale = (infoScrollView->getContentSize().width * 0.8) / imageSize.width;
        itemInfoImage->setScale(scale);
        itemInfoImage->setVisible(true);

        // Cập nhật vị trí ảnh
        itemInfoImage->setAnchorPoint(Vec2(0.5, 1));
        itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, contentStartY));

        contentStartY -= (imageSize.height * scale + INFO_LABEL_PADDING_Y);
    }
    else {
        itemInfoImage->setVisible(false);
    }

    Label* descriptionLabel = dynamic_cast<Label*>(infoScrollView->getChildByName("descriptionLabel"));
    if (item.imageInfo.empty()) {
        if (!descriptionLabel) {
            descriptionLabel = Label::createWithSystemFont(item.description, "Arial", INFO_DESC_LABEL_FONT_SIZE);
            descriptionLabel->setAnchorPoint(Vec2(0, 1));
            descriptionLabel->setPosition(Vec2(10, contentStartY));
            descriptionLabel->setTextColor(Color4B::WHITE);
            descriptionLabel->setName("descriptionLabel");
            infoScrollView->addChild(descriptionLabel);
        }
        else {
            descriptionLabel->setString(item.description);
        }
        contentStartY -= (descriptionLabel->getContentSize().height + INFO_LABEL_PADDING_Y);
    }
    else if (descriptionLabel) {
        descriptionLabel->removeFromParent();
    }

    float totalHeight = infoScrollView->getContentSize().height - contentStartY;
    if (totalHeight > infoScrollView->getContentSize().height) {
        infoScrollView->setInnerContainerSize(Size(infoScrollView->getContentSize().width, totalHeight + 20));
    }
    else {
        infoScrollView->setInnerContainerSize(infoScrollView->getContentSize());
    }
}


