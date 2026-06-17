#include "PerkFactory.h"
#include <cstdlib>

Perk* PerkFactory::createStartingPerk()
{
    int roll = rand() % 2;
    if (roll == 0) return new Bonus21Damage();
    return nullptr;
}

Perk* PerkFactory::createShopPerk(int choice)
{
    switch (choice)
    {
        case 1: return new Bonus21Damage();
        case 2: return new FlatDamagePlus5();
        case 3: return new BlackjackDouble();
    }
    return nullptr;
}
