#ifndef PLAYERATTRIBUTES_H
#define PLAYERATTRIBUTES_H

class PlayerAttributes
{
private:
    int health;

public:
    PlayerAttributes(int initialHealth);  // Constructor
    void TakeDamage(int damage);          // Reduces player's health
    bool IsDead() const;                  // Checks if player is dead
    int GetHealth() const;                // Returns the current health
};

#endif // PLAYERATTRIBUTES_H
