#ifndef __HEALTH_ITEM_H__
#define __HEALTH_ITEM_H__

#include "CollectibleItem.h"

class HealthItem : public CollectibleItem {
public:
    static HealthItem* create();
    virtual bool init() override;
    virtual void applyEffect() override;
private:
    float _spriteScale; // Add this line
};

#endif // __HEALTH_ITEM_H__
