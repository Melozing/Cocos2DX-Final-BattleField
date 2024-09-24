#include "PlayerAttributes.h"

// Constructor to initialize player's health
PlayerAttributes::PlayerAttributes(int initialHealth) : health(initialHealth) {}

// Method to reduce player's health
void PlayerAttributes::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

// Method to check if player is dead (health <= 0)
bool PlayerAttributes::IsDead() const
{
    return health <= 0;
}

// Method to get the current health of the player
int PlayerAttributes::GetHealth() const
{
    return health;
}
