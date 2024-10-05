#ifndef __AMMO_ITEM_H__
#define __AMMO_ITEM_H__

#include "CollectibleItem.h"

class AmmoItem : public CollectibleItem {
public:
    static AmmoItem* create();
    virtual bool init() override;
    virtual void applyEffect() override;
    int getAmmoValue() const { return _ammoValue; }
private:
    float _spriteScale; // Add this line
    int _ammoValue; // Ensure this member variable exists and is set appropriately
};

#endif // __AMMO_ITEM_H__
