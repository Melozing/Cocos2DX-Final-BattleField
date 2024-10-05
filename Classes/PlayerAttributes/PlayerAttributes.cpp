#include "PlayerAttributes/PlayerAttributes.h"
#include "Constants/Constants.h"
#include "cocos2d.h"

// Private constructor to initialize player's attributes
PlayerAttributes::PlayerAttributes(int initialHealth, int initialAmmo) : health(initialHealth), ammo(initialAmmo) {}

// Static method to get the singleton instance
PlayerAttributes& PlayerAttributes::getInstance()
{
    static PlayerAttributes instance(Constants::PLAYER_HEALTH, 0); // Example initial values
    return instance;
}

void PlayerAttributes::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
    CCLOG("Player took damage. Current health: %d", health);
}

bool PlayerAttributes::IsDead() const
{
    return health <= 0;
}

int PlayerAttributes::GetHealth() const
{
    return health;
}

void PlayerAttributes::SetHealth(int newHealth)
{
    health = newHealth;
    if (health < 0) health = 0;
    CCLOG("Player's health set. Current health: %d", health);
}

void PlayerAttributes::IncreaseHealth(int amount)
{
    health += amount;
    if (health >= Constants::PLAYER_HEALTH) health = Constants::PLAYER_HEALTH; // Ensure health does not exceed max value

    CCLOG("Player's health increased. Current health: %d", health);
}
