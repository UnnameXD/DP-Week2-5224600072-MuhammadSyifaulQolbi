#ifndef PERKFACTORY_H
#define PERKFACTORY_H

#include "Perk.h"

// Factory Pattern — decouples perk creation from RunSession
// Mutable: new perk types can be added without touching RunSession
class PerkFactory
{
public:
    static Perk* createStartingPerk();
    static Perk* createShopPerk(int choice);
};

#endif
