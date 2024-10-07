#ifndef __HEALTH_ITEM_H__
#define __HEALTH_ITEM_H__

#include "CollectibleItem.h"
#include "Constants/Constants.h"

class HealthItem : public CollectibleItem {
public:
    static HealthItem* create();
    virtual bool init() override;
    virtual void applyEffect() override;
    int getHealthValue() const override {
        return Constants::PLAYER_HEALTH; // Example health value
    }
private:
    float _spriteScale; // Add this line
};

#endif // __HEALTH_ITEM_H__
