#ifndef __SHIELD_SKILL_ITEM_POOL_H__
#define __SHIELD_SKILL_ITEM_POOL_H__

#include "ShieldSkill.h"
#include <queue>

class ShieldSkillItemPool {
public:
    static ShieldSkillItemPool* getInstance();
    void initPool(int poolSize);
    ShieldSkill* getItem();
    void returnItem(ShieldSkill* item);
    void resetPool();
private:
    ShieldSkillItemPool() = default;
    std::queue<ShieldSkill*> _availableItems;
};

#endif // __SHIELD_SKILL_ITEM_POOL_H__
