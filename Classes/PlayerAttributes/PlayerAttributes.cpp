#include "PlayerAttributes.h"
#include <iostream>

// Constructor to initialize player's health
PlayerAttributes::PlayerAttributes(int initialHealth) : health(initialHealth) {}

// Method to reduce player's health
void PlayerAttributes::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;

    // Debug output
    std::cout << "Player took damage. Current health: " << health << std::endl;
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

// Method to set the player's health to a specific value
void PlayerAttributes::SetHealth(int newHealth)
{
    health = newHealth;  // Update health with the new value
    if (health < 0) health = 0;  // Ensure health doesn't go below 0

    // Debug output
    std::cout << "Player's health set. Current health: " << health << std::endl;
}
