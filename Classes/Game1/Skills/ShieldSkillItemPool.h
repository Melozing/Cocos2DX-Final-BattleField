#ifndef __SHIELD_SKILL_ITEM_POOL_H__
#define __SHIELD_SKILL_ITEM_POOL_H__

#include "ShieldSkillItem.h"
#include <queue>

class ShieldSkillItemPool {
public:
    static ShieldSkillItemPool* getInstance();
    void initPool(int poolSize);
    ShieldSkillItem* getItem();
    void returnItem(ShieldSkillItem* item);
    void resetPool();
private:
    ShieldSkillItemPool() = default;
    std::queue<ShieldSkillItem*> _availableItems;
};

#endif // __SHIELD_SKILL_ITEM_POOL_H__
