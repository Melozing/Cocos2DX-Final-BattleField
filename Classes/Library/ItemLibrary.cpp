#include "Library/ItemLibrary.h"
#include "external/tinyxml2/tinyxml2.h"
#include "cocos2d.h"

using namespace cocos2d;

ItemLibrary& ItemLibrary::getInstance() {
    static ItemLibrary instance;
    return instance;
}

bool ItemLibrary::loadItemsFromXml(const std::string& filePath) {

    if (isDataLoaded) {
        CCLOG("Data already loaded, skipping loading from XML");
        return true;
    }
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    CCLOG("Full path to XML file: %s", fullPath.c_str());

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(fullPath.c_str());
    if (result != tinyxml2::XML_SUCCESS) {
        CCLOG("Failed to load XML file: %s", fullPath.c_str());
        return false;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        CCLOG("Failed to get root element in XML file: %s", fullPath.c_str());
        return false;
    }

    for (tinyxml2::XMLElement* itemElement = root->FirstChildElement("item"); itemElement; itemElement = itemElement->NextSiblingElement("item")) {
        ItemData data;
        tinyxml2::XMLElement* idElement = itemElement->FirstChildElement("id");
        tinyxml2::XMLElement* imagePathElement = itemElement->FirstChildElement("imagePath");
        tinyxml2::XMLElement* nameElement = itemElement->FirstChildElement("name");
        tinyxml2::XMLElement* descriptionElement = itemElement->FirstChildElement("description");

        if (idElement && idElement->GetText()) {
            data.id = std::stoi(idElement->GetText());
        }
        else {
            CCLOG("Item missing 'id' or 'id' is not an int");
            continue;
        }

        if (imagePathElement && imagePathElement->GetText()) {
            data.imagePath = imagePathElement->GetText();
        }
        else {
            CCLOG("Item missing 'imagePath' or 'imagePath' is not a string");
            continue;
        }

        if (nameElement && nameElement->GetText()) {
            data.name = nameElement->GetText();
        }
        else {
            CCLOG("Item missing 'name' or 'name' is not a string");
            continue;
        }

        if (descriptionElement && descriptionElement->GetText()) {
            data.description = descriptionElement->GetText();
        }
        else {
            CCLOG("Item missing 'description' or 'description' is not a string");
            continue;
        }

        items.push_back(data);
    }
    isDataLoaded = true;
    CCLOG("Successfully loaded %zu items from XML", items.size());
    return true;
}

const std::vector<ItemData>& ItemLibrary::getItems() const {
    return items;
}

