#ifndef __AMMO_ITEM_H__
#define __AMMO_ITEM_H__

#include "CollectibleItem.h"

class AmmoItem : public CollectibleItem {
public:
    static AmmoItem* create();
    virtual bool init() override;
    virtual void applyEffect() override;

private:
    float _spriteScale; // Add this line
};

#endif // __AMMO_ITEM_H__
