#include "LibraryWindow.h"
#include "Controller/SpriteController.h"
#include "Constants/Constants.h"
#include "json/document.h"

USING_NS_CC;

bool LibraryWindow::init()
{
    if (!Node::init())
    {
        return false;
    }

    initBackground();
	loadItemData("json/items_library.json");
    initCloseButton();
    addLeftInfoImage();
    addRightInfoImage();
    addTopCenterLabel();
    setupOverlay();

    return true;
}

void LibraryWindow::initBackground() {
    // Add a background image
    backgroundImage = Sprite::create("assets_game/UXUI/Collection/library_bg.png");
    if (backgroundImage)
    {
        backgroundImage->setScale(SpriteController::updateSpriteScale(backgroundImage, 0.8f));
        backgroundImage->setPosition(Vec2::ZERO);
        backgroundImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        this->addChild(backgroundImage, Constants::ORDER_LAYER_LAYOUT_UI);

    }
}

void LibraryWindow::setupOverlay() {
    // Initialize overlay layer
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    overlayLayer = Sprite::create("assets_game/UXUI/Collection/overlay.png");
    overlayLayer->setContentSize(visibleSize);
    overlayLayer->setPosition(Vec2::ZERO);
    overlayLayer->setVisible(false);
    this->addChild(overlayLayer, Constants::ORDER_LAYER_LAYOUT_UI-2); // Ensure overlay is on top

    //// Add touch listener to intercept touch events
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true); // Ensure touches are not propagated to layers below
    touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (overlayLayer->isVisible())
        {
            return true; // Return true to indicate that the touch event has been handled
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, overlayLayer);
}

void LibraryWindow::addTopCenterLabel() {
    // Add a label at the top center of the backgroundImage
    float paddingHight = SpriteController::calculateScreenHeightRatio(0.04);
    auto fontSize = 50;
    auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
    if (platform == cocos2d::Application::Platform::OS_ANDROID ||
        platform == cocos2d::Application::Platform::OS_IPHONE) {
        paddingHight = SpriteController::calculateScreenHeightRatio(0.008);
        fontSize = 24;
    }
    topCenterLabel = Label::createWithTTF("Thư Viện", Constants::FONT_GAME, fontSize);
    if (topCenterLabel)
    {
        topCenterLabel->setPosition(Vec2(backgroundImage->getContentSize().width / 2,
            backgroundImage->getContentSize().height - topCenterLabel->getContentSize().height / 2 - paddingHight));
        topCenterLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        backgroundImage->addChild(topCenterLabel, Constants::ORDER_LAYER_LAYOUT_UI + 1);
    }
}

void LibraryWindow::initCloseButton() {
    // Create and position the close button
    closeButton = ui::Button::create("assets_game/UXUI/Collection/close_btn.png");
    closeButton->setScale(SpriteController::updateSpriteScale(closeButton, 0.135f));
    closeButton->setPosition(Vec2(SpriteController::GetContentSize(backgroundImage).width,
        SpriteController::GetContentSize(backgroundImage).height)/2);
    closeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            this->toggleItemLibraryWindow(this->getPosition());
            NotificationCenter::getInstance()->postNotification("CloseLibraryWindow");
        }
        });
    this->addChild(closeButton, Constants::ORDER_LAYER_LAYOUT_UI - 1); 
}

void LibraryWindow::addLeftInfoImage() {
    // Add a left information image
    leftInfoImage = Sprite::create("assets_game/UXUI/Collection/library_left.png");
    if (leftInfoImage)
    {
        leftInfoImage->setScale(SpriteController::updateSpriteScale(leftInfoImage, 0.625f));
   /*     leftInfoImage->setPosition(Vec2(SpriteController::GetContentSize(backgroundImage).width * 0.363f,
            SpriteController::GetContentSize(backgroundImage).height * 0.57));*/
        float paddingHight = SpriteController::calculateScreenHeightRatio(0.06);
        float paddingWeight = SpriteController::calculateScreenRatio(0.175);
        auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
        if (platform == cocos2d::Application::Platform::OS_ANDROID ||
            platform == cocos2d::Application::Platform::OS_IPHONE) {
            paddingHight = SpriteController::calculateScreenHeightRatio(0.03);
        }
        leftInfoImage->setPosition(backgroundImage->getPosition().x - paddingWeight, backgroundImage->getPosition().y - paddingHight);
        leftInfoImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        this->addChild(leftInfoImage, Constants::ORDER_LAYER_LAYOUT_UI + 1);
        this->setupScollItems();
    }
}

void LibraryWindow::setupScollItems() {
    // Create and configure the ScrollView
    itemScrollView = ui::ScrollView::create();
    itemScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    itemScrollView->setContentSize(leftInfoImage->getContentSize());
    itemScrollView->setPosition(Vec2::ZERO);
    itemScrollView->setBounceEnabled(true);
    leftInfoImage->addChild(itemScrollView);

    // Define the scale factor for the items
    float scaleFactor = 0.29f; // Adjust this value as needed
    float scaleFactorBackground = 0.55f; // Adjust this value as needed

    // Calculate the number of columns and the padding between items
    const int numColumns = 2;
    float padding = SpriteController::calculateScreenHeightRatio(0.02);

    // Calculate the size of each item
    float itemWidth = (itemScrollView->getContentSize().width - (numColumns + 1) * padding) / numColumns;
    float itemHeight = itemWidth; // Assuming square items

    // Adjust the inner container size of the ScrollView
    int numRows = (items.size() + numColumns - 1) / numColumns; // Calculate the number of rows
    float innerHeight = numRows * (itemHeight + padding) + padding;
    itemScrollView->setInnerContainerSize(Size(itemScrollView->getContentSize().width, innerHeight));

    // Populate the ScrollView with items
    for (size_t i = 0; i < items.size(); ++i) {
        // Create the item button
        auto itemButton = ui::Button::create(items[i].iconPath);
        itemButton->setScale(SpriteController::updateSpriteScale(itemButton, scaleFactor)); // Scale the item button

        // Create active and inactive backgrounds
        auto activeBackground = Sprite::create("assets_game/UXUI/Collection/item_frame_active.png");
        activeBackground->setScale(SpriteController::updateSpriteScale(activeBackground, scaleFactorBackground));
        activeBackground->setVisible(false); // Initially hidden

        auto inactiveBackground = Sprite::create("assets_game/UXUI/Collection/item_frame.png");
        inactiveBackground->setScale(SpriteController::updateSpriteScale(inactiveBackground, scaleFactorBackground));

        // Calculate the position of the item
        int row = i / numColumns;
        int col = i % numColumns;
        float x = padding + col * (itemWidth + padding) + itemWidth / 2;
        float y = innerHeight - (padding + row * (itemHeight + padding) + itemHeight / 2);

        activeBackground->setPosition(Vec2(x, y));
        inactiveBackground->setPosition(Vec2(x, y));
        itemButton->setPosition(Vec2(x, y));

        // Add touch event listener for itemButton
        itemButton->addTouchEventListener([this, i](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
                this->displayItemDetails(i);
                this->items[i].isActive = true;
                this->updateItemStates();
            }
            });

        // Add touch event listener for inactiveBackground
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [this, i](Touch* touch, Event* event) {
            auto target = static_cast<Sprite*>(event->getCurrentTarget());
            Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
            Size size = target->getContentSize();
            Rect rect = Rect(0, 0, size.width, size.height);

            if (rect.containsPoint(locationInNode)) {
                this->displayItemDetails(i);
                this->items[i].isActive = true;
                this->updateItemStates();
                return true;
            }
            return false;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, inactiveBackground);

        itemScrollView->addChild(activeBackground);
        itemScrollView->addChild(inactiveBackground);
        itemScrollView->addChild(itemButton);

        // Store the backgrounds in the item data
        items[i].activeBackground = activeBackground;
        items[i].inactiveBackground = inactiveBackground;

        itemButtons.push_back(itemButton); // Store the item button
    }

    // Ensure the ScrollView starts at the top
    itemScrollView->jumpToTop();
}

void LibraryWindow::updateItemStates() {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].isActive) {
            items[i].activeBackground->setVisible(true);
            items[i].inactiveBackground->setVisible(false);
        }
        else {
            items[i].activeBackground->setVisible(false);
            items[i].inactiveBackground->setVisible(true);
        }
    }
}

void LibraryWindow::loadItemData(const std::string& filePath) {
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(filePath);
    if (fileContent.empty()) {
        CCLOG("Failed to load file: %s", filePath.c_str());
        return;
    }

    rapidjson::Document document;
    document.Parse(fileContent.c_str());
    if (document.HasParseError()) {
        CCLOG("Failed to parse JSON file: %s", filePath.c_str());
        return;
    }

    for (auto& item : document.GetArray()) {
        if (!item.HasMember("iconPath") || !item.HasMember("name") || !item.HasMember("imagePath") || !item.HasMember("content")) {
            CCLOG("Invalid item data in file: %s", filePath.c_str());
            continue;
        }

        ItemData itemData;
        itemData.iconPath = item["iconPath"].GetString();
        itemData.name = item["name"].GetString();
        itemData.imagePath = item["imagePath"].GetString();
        itemData.content = item["content"].GetString();
        items.push_back(itemData);
    }
}

void LibraryWindow::displayItemDetails(size_t index) {
    if (index >= items.size()) return;

    // Set all items to inactive
    for (auto& item : items) {
        item.isActive = false;
    }

    // Set the selected item to active
    items[index].isActive = true;

    // Update item states
    updateItemStates();

    rightInfoImage->removeAllChildrenWithCleanup(true);
    paperSpriteFront->removeAllChildrenWithCleanup(true);

    // Update rightInfoImage with the item's image
    auto itemsImg = Sprite::create(items[index].imagePath);
    itemsImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto paddingHPosImg = SpriteController::calculateScreenRatio(0.23);
    auto paddingWPosImg = SpriteController::calculateScreenRatio(0.22);
    auto paddingWPosText = SpriteController::calculateScreenRatio(0.4);
    itemsImg->setScale(SpriteController::updateSpriteScale(itemsImg, 0.7f));
    itemsImg->setPosition(Vec2(rightInfoImage->getContentSize().width / 2 - paddingWPosImg, rightInfoImage->getContentSize().height - paddingHPosImg));
    rightInfoImage->addChild(itemsImg);

    // Create a Label for the item's name
    auto paddingHNameImg = SpriteController::calculateScreenRatio(0.13);
    auto itemNameLabel = Label::createWithTTF(items[index].name, Constants::FONT_GAME, 60);
    itemNameLabel->setTextColor(Color4B(0xFC, 0xFF, 0xA2, 0xFF));
    itemNameLabel->setContentSize(rightInfoImage->getContentSize() / 2);
    itemNameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    itemNameLabel->setPosition(Vec2(itemsImg->getPosition().x / 2 + paddingWPosText, itemsImg->getPosition().y + paddingHNameImg));
    rightInfoImage->addChild(itemNameLabel);

    // Create a ScrollView for the content
    auto contentScrollView = ui::ScrollView::create();
    contentScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    contentScrollView->setContentSize(paperSpriteFront->getContentSize() * 0.8f); // Make the content smaller
    contentScrollView->setPosition(Vec2((paperSpriteFront->getContentSize().width - contentScrollView->getContentSize().width) / 2,
        (paperSpriteFront->getContentSize().height - contentScrollView->getContentSize().height) / 2)); // Center the content
    contentScrollView->setBounceEnabled(true);
    paperSpriteFront->addChild(contentScrollView);

    // Define padding values
    float padding = 20.0f; // Adjust this value as needed

    // Create a Label for the content
    auto contentLabel = Label::createWithTTF(items[index].content, Constants::FONT_GAME_PARAGRAPH, 45);
    contentLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    contentLabel->setTextColor(Color4B(0x3C, 0x2C, 0x23, 0xFF));
    contentLabel->setWidth(contentScrollView->getContentSize().width - 2 * padding);
    contentLabel->setPosition(Vec2(padding, contentScrollView->getInnerContainerSize().height - padding));

    // Adjust the inner container size of the ScrollView based on the content size
    float contentHeight = contentLabel->getContentSize().height + 2 * padding;
    contentScrollView->setInnerContainerSize(Size(contentScrollView->getContentSize().width, contentHeight));
    contentLabel->setPosition(Vec2(padding, contentScrollView->getInnerContainerSize().height - padding));

    contentScrollView->addChild(contentLabel);
}


void LibraryWindow::addRightInfoImage() {
    // Add a right information image
    rightInfoImage = Sprite::create("assets_game/UXUI/Collection/library_right.png");
    if (rightInfoImage)
    {
        rightInfoImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        rightInfoImage->setScale(SpriteController::updateSpriteScale(rightInfoImage, 0.625f));
     /*   rightInfoImage->setPosition(Vec2(SpriteController::GetContentSize(backgroundImage).width * 0.923f,
            SpriteController::GetContentSize(backgroundImage).height * 0.57));*/
        float paddingHight = SpriteController::calculateScreenHeightRatio(0.06);
        float paddingWeight = SpriteController::calculateScreenRatio(0.163);
        auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
        if (platform == cocos2d::Application::Platform::OS_ANDROID ||
            platform == cocos2d::Application::Platform::OS_IPHONE) {
            paddingHight = SpriteController::calculateScreenHeightRatio(0.03);
        }

        rightInfoImage->setPosition(backgroundImage->getPosition().x + paddingWeight, backgroundImage->getPosition().y - paddingHight);
        rightInfoImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        this->addChild(rightInfoImage, Constants::ORDER_LAYER_LAYOUT_UI + 1);

        // Add paper sprite below and centered relative to rightInfoImage
        paperSprite = Sprite::create("assets_game/UXUI/Collection/paper.png");
        paperSpriteFront = Sprite::create("assets_game/UXUI/Collection/paper_front.png");
        if (paperSprite)
        {
            float paddingHightPaper = SpriteController::calculateScreenHeightRatio(0.17);
            auto platform = cocos2d::Application::getInstance()->getTargetPlatform();
            if (platform == cocos2d::Application::Platform::OS_ANDROID ||
                platform == cocos2d::Application::Platform::OS_IPHONE) {
                paddingHightPaper = SpriteController::calculateScreenHeightRatio(0.07);
            }
            paperSprite->setScale(SpriteController::updateSpriteScale(paperSprite, 0.35f));
            paperSprite->setPosition(Vec2(rightInfoImage->getPosition().x, rightInfoImage->getPosition().y - paddingHightPaper));
            paperSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            this->addChild(paperSprite, Constants::ORDER_LAYER_LAYOUT_UI + 2);
            paperSpriteFront->setScale(SpriteController::updateSpriteScale(paperSprite, 0.35f));
            paperSpriteFront->setPosition(Vec2(rightInfoImage->getPosition().x, rightInfoImage->getPosition().y - paddingHightPaper));
            paperSpriteFront->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            this->addChild(paperSpriteFront, Constants::ORDER_LAYER_LAYOUT_UI + 2);
        }
    }
}

void LibraryWindow::toggleItemLibraryWindow(const Vec2& position)
{
    if (isItemLibraryVisible) {
        // Hide ItemLibraryWindow
        overlayLayer->setVisible(false);
        this->runAction(Sequence::create(
            Spawn::create(
                MoveTo::create(0.5, position),
                ScaleTo::create(0.5, 0.1),
                nullptr
            ),
            CallFunc::create([this]() {
                this->setVisible(false);
                }),
            nullptr
        ));
    }
    else {
        // Show ItemLibraryWindow
        overlayLayer->setVisible(true);
        this->setVisible(true);
        this->setScale(0.1);
        this->setPosition(position);
        this->runAction(Spawn::create(
            ScaleTo::create(0.5, 1.0f),
            MoveTo::create(0.5, Director::getInstance()->getVisibleSize() / 2),
            nullptr
        ));
    }
    isItemLibraryVisible = !isItemLibraryVisible;
}
