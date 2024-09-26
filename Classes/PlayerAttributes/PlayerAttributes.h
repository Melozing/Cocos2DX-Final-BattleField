#ifndef PLAYERATTRIBUTES_H
#define PLAYERATTRIBUTES_H

class PlayerAttributes
{
private:
    int health;  // Player's health

public:
    PlayerAttributes(int initialHealth);  // Constructor
    void TakeDamage(int damage);          // Reduces player's health
    bool IsDead() const;                  // Checks if player is dead
    int GetHealth() const;                // Returns the current health
    void SetHealth(int newHealth);        // Sets the player's health to a specific value
};

#endif // PLAYERATTRIBUTES_H
