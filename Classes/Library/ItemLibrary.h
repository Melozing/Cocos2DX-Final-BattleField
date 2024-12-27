#ifndef __ITEM_LIBRARY_H__
#define __ITEM_LIBRARY_H__

#include <vector>
#include <string>
#include "cocos2d.h"
#include "external/tinyxml2/tinyxml2.h"
struct ItemData {
    int id;
    std::string imagePath;
    std::string name;
    std::string description;
};

class ItemLibrary {
public:
    static ItemLibrary& getInstance();
    bool loadItemsFromXml(const std::string& filePath);
    const std::vector<ItemData>& getItems() const;

private:
    ItemLibrary() = default;
    std::vector<ItemData> items;
    bool isDataLoaded;
};

#endif // __ITEM_LIBRARY_H__
