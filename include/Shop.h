#ifndef SHOP_H
#define SHOP_H

#include <vector>
#include "Perk.h"
#include "InputProvider.h"

// Shop — Mutable phase (called between rounds, can be replaced or extended)
class Shop
{
public:
    static void openShop(int& cash, std::vector<Perk*>& perks, InputProvider& input);
};

#endif
