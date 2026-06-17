#ifndef PERK_H
#define PERK_H

#include <string>

// Abstract base class — Invariant interface for all perks
class Perk
{
public:
    virtual int modifyDamage(int baseDamage) = 0;
    virtual std::string getName() = 0;
    virtual ~Perk() {}
};

// Mutable: concrete perk implementations can be added/removed freely
class Bonus21Damage : public Perk
{
public:
    int modifyDamage(int baseDamage) override;
    std::string getName() override;
};

class FlatDamagePlus5 : public Perk
{
public:
    int modifyDamage(int baseDamage) override;
    std::string getName() override;
};

class BlackjackDouble : public Perk
{
public:
    int modifyDamage(int baseDamage) override;
    std::string getName() override;
};

#endif
