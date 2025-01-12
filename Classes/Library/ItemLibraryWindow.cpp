#include "Library/ItemLibraryWindow.h"
#include "Scene/MainMenuScene.h"
#include "ui/UIWidget.h"
#include "Controller/SoundController.h"
#include "Controller/SpriteController.h"

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
    createTitleLabel();
    createItemSection();
    createInfoSection();
    createExitButton();

    return true;
}

void ItemLibraryWindow::createBackground() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    backgroundSprite = Sprite::create("assets_game/UXUI/Collection/c_full_header.png");
    if (!backgroundSprite) {
        return;
    }
    backgroundSprite->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
    backgroundSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(backgroundSprite);
}

void ItemLibraryWindow::createTitleLabel() {
    Size mainBgSize = backgroundSprite->getContentSize();
    auto titleLabel = Label::createWithSystemFont("Library", Constants::FONT_GAME, 23);
    if (!titleLabel) {
        CCLOG("Failed to create titleLabel");
        return;
    }
    titleLabel->setTextColor(Color4B::GREEN);
    titleLabel->enableBold();
    titleLabel->setPosition(Vec2(mainBgSize.width / 2, mainBgSize.height / 2));
    backgroundSprite->addChild(titleLabel);
}

cocos2d::Rect ItemLibraryWindow::getMainBgBoundingBox() const {
    return backgroundSprite->getBoundingBox();
}

void ItemLibraryWindow::createIntroSection() {
    /*auto introBg = Sprite::create("assets_game/UXUI/Collection/intro.png");
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

    introLabel = Label::createWithSystemFont("Welcome to the Item Library", Constants::FONT_GAME, INFO_LABEL_FONT_SIZE);
    if (!introLabel) {
        CCLOG("Failed to create introLabel");
        return;
    }

    introLabel->setPosition(Vec2(introBg->getContentSize().width / 2, introBg->getContentSize().height / 2));
    introBg->addChild(introLabel);*/
}

void ItemLibraryWindow::createExitButton() {
    auto exitButton = Button::create("assets_game/UXUI/Collection/c_header_close.png", "assets_game/UXUI/Collection/c_header_close-PUSH.png");
    if (!exitButton) {
        CCLOG("Failed to create exitButton");
        return;
    }
    exitButton->setScale(SpriteController::updateSpriteScale(exitButton,0.1f));
    exitButton->setPosition(Vec2(backgroundSprite->getContentSize().width , backgroundSprite->getContentSize().height));
    exitButton->addClickEventListener([=](Ref* sender) {
        auto mainMenuScene = dynamic_cast<MainMenu*>(Director::getInstance()->getRunningScene());
        if (mainMenuScene) {
			SoundController::getInstance()->playSoundEffect(Constants::ButtonClickSFX);
            mainMenuScene->toggleItemLibraryWindow();
        }
        });
    backgroundSprite->addChild(exitButton);
}

void ItemLibraryWindow::createItemSection() {
    auto glView = Director::getInstance()->getOpenGLView();
    glView->setScissorInPoints(backgroundSprite->getPositionX() - backgroundSprite->getContentSize().width / 2,
        backgroundSprite->getPositionY() - backgroundSprite->getContentSize().height / 2,
        backgroundSprite->getContentSize().width,
        backgroundSprite->getContentSize().height);
    glEnable(GL_SCISSOR_TEST);

    // Tạo ScrollView bên trong ClippingNode
    auto scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(backgroundSprite->getContentSize().width, backgroundSprite->getContentSize().height- SpriteController::calculateScreenHeightRatio(0.03f))); // Kích thước hiển thị bằng background
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setInnerContainerSize(Size(300, 600)); // Nội dung dài hơn
    scrollView->setBounceEnabled(true);
    scrollView->setPosition(Vec2::ZERO); // Vị trí trong ClippingNode
    backgroundSprite->addChild(scrollView);

    // Thêm các item vào ListView
    for (int i = 0; i < 20; ++i) {
        auto item = ui::Button::create("assets_game/items/HealthRecoveryItem.png");
		item->setScale(0.01f);
        item->setPosition(Vec2(150, 600 - 50 - i * 100)); // Sắp xếp các item
        scrollView->addChild(item);
    }

    glDisable(GL_SCISSOR_TEST);

    //auto itemsBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    //if (!itemsBg) {
    //    CCLOG("Failed to create itemsBg sprite");
    //    return;
    //}
    //const Size mainBgSize = mainBg->getContentSize();
    //itemsBg->setContentSize(Size(mainBgSize.width / 3 - 10, mainBgSize.height - 60));
    //itemsBg->setAnchorPoint(Vec2(-0.04f, 0));
    //itemsBg->setPosition(Vec2(5, 5));
    //mainBg->addChild(itemsBg);

    //scrollView = ScrollView::create();
    //if (!scrollView) {
    //    CCLOG("Failed to create scrollView");
    //    return;
    //}
    //const Size itemsBgSize = itemsBg->getContentSize();
    //scrollView->setDirection(ScrollView::Direction::VERTICAL);
    //scrollView->setContentSize(itemsBgSize);
    //scrollView->setInnerContainerSize(itemsBgSize);
    //scrollView->setBounceEnabled(true);
    //scrollView->setScrollBarEnabled(false); // Disable default scroll bar
    //scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    //scrollView->setPosition(Vec2(itemsBgSize.width / 2, itemsBgSize.height / 2 - 15));
    //itemsBg->addChild(scrollView);

    //const auto& items = ItemLibrary::getInstance().getItems();
    //int columns = 2;
    //float itemWidth = (itemsBgSize.width - (columns + 1) * ITEM_PADDING_X) / columns;
    //float itemHeight = itemWidth;
    //int rows = (items.size() + columns - 1) / columns;
    //float totalHeight = rows * (itemHeight + ITEM_PADDING_Y) + ITEM_PADDING_Y;
    //scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, totalHeight));

    //for (size_t i = 0; i < items.size(); ++i) {
    //    int row = i / columns;
    //    int col = i % columns;

    //    float posX = ITEM_PADDING_X + col * (itemWidth + ITEM_PADDING_X) + itemWidth / 2;
    //    float posY = totalHeight - ITEM_PADDING_Y - row * (itemHeight + ITEM_PADDING_Y) - itemHeight / 2;
    //    auto itemLayout = UILibrary::createItemLayout(items[i], [=]() {
    //        updateItemInfo(items[i]);
    //        });
    //    if (!itemLayout) {
    //        CCLOG("Failed to create itemLayout for item %d", items[i].id);
    //        continue;
    //    }
    //    itemLayout->setAnchorPoint(Vec2(0.5, 0.5));
    //    itemLayout->setPosition(Vec2(posX, posY));
    //    scrollView->addChild(itemLayout);
    //}

    //// Add mouse scroll event listener to scrollView
    //auto mouseListener = EventListenerMouse::create();
    //mouseListener->onMouseScroll = [=](Event* event) {
    //    EventMouse* e = (EventMouse*)event;
    //    if (scrollView->getBoundingBox().containsPoint(e->getLocationInView())) {
    //        float delta = e->getScrollY() * 10; // Adjust scroll speed
    //        auto newPos = scrollView->getInnerContainerPosition().y + delta;
    //        newPos = std::max(newPos, scrollView->getContentSize().height - scrollView->getInnerContainerSize().height);
    //        newPos = std::min(newPos, 0.0f);
    //        scrollView->setInnerContainerPosition(Vec2(0, newPos));
    //    }
    //    };
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, scrollView);
}

void ItemLibraryWindow::createInfoSection() {
    auto infoBg = Sprite::create("assets_game/UXUI/Collection/c_full.png");
    if (!infoBg) {
        CCLOG("Failed to create infoBg sprite");
        return;
    }
    const Size mainBgSize = backgroundSprite->getContentSize();
    infoBg->setContentSize(Size(mainBgSize.width * 2 / 3 - 10, mainBgSize.height - 60));
    infoBg->setAnchorPoint(Vec2(0, 0));
    infoBg->setPosition(Vec2(mainBgSize.width / 3, 5));
    backgroundSprite->addChild(infoBg);

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
    itemInfoImage->setPosition(Vec2(infoScrollView->getContentSize().width / 2, infoScrollView->getContentSize().height));
    itemInfoImage->setAnchorPoint(Vec2(0.5, 1.0));
    infoScrollView->addChild(itemInfoImage);
}

void ItemLibraryWindow::updateItemInfo(const ItemData& item) {
    Label* nameLabel = dynamic_cast<Label*>(infoScrollView->getChildByName("nameLabel"));
    if (!nameLabel) {
        nameLabel = Label::createWithSystemFont(item.name, Constants::FONT_GAME, 23);
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
        shortDescriptionLabel = Label::createWithSystemFont(item.shortDescription, Constants::FONT_GAME, 23);
        shortDescriptionLabel->setAnchorPoint(Vec2(0, 1));
        shortDescriptionLabel->setPosition(Vec2(10, nameLabel->getPositionY() - nameLabel->getContentSize().height - 10));
        shortDescriptionLabel->setTextColor(Color4B::WHITE);
        shortDescriptionLabel->setName("shortDescriptionLabel");
        infoScrollView->addChild(shortDescriptionLabel);
    }
    else {
        shortDescriptionLabel->setString(item.shortDescription);
    }
    float contentStartY = shortDescriptionLabel->getPositionY() - shortDescriptionLabel->getContentSize().height ;

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

        contentStartY -= (imageSize.height * scale );
    }
    else {
        itemInfoImage->setVisible(false);
    }

    Label* descriptionLabel = dynamic_cast<Label*>(infoScrollView->getChildByName("descriptionLabel"));
    if (item.imageInfo.empty()) {
        if (!descriptionLabel) {
            descriptionLabel = Label::createWithSystemFont(item.description, Constants::FONT_GAME, 23);
            descriptionLabel->setAnchorPoint(Vec2(0, 1));
            descriptionLabel->setPosition(Vec2(10, contentStartY));
            descriptionLabel->setTextColor(Color4B::WHITE);
            descriptionLabel->setName("descriptionLabel");
            infoScrollView->addChild(descriptionLabel);
        }
        else {
            descriptionLabel->setString(item.description);
        }
        contentStartY -= (descriptionLabel->getContentSize().height);
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