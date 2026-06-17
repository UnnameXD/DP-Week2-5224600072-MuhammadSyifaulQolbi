#include "Shop.h"
#include "PerkFactory.h"
#include <iostream>
#include <cstdlib>

void Shop::openShop(int& cash, std::vector<Perk*>& perks, InputProvider& input)
{
    int option1 = rand() % 3 + 1;
    int option2 = rand() % 3 + 1;

    int prices[4] = { 0, 30, 25, 40 };
    int price1 = prices[option1];
    int price2 = prices[option2];

    Perk* preview1 = PerkFactory::createShopPerk(option1);
    Perk* preview2 = PerkFactory::createShopPerk(option2);

    std::cout << "\n===== SHOP =====\n";
    std::cout << "Cash: " << cash << "\n\n";
    std::cout << "1. " << preview1->getName() << " (" << price1 << " gold)\n";
    std::cout << "2. " << preview2->getName() << " (" << price2 << " gold)\n";
    std::cout << "0. Skip\n";
    std::cout << "Pilihan: ";

    int choice = input.getShopChoice();

    if (choice == 1 && cash >= price1)
    {
        perks.push_back(preview1);
        cash -= price1;
        std::cout << "Membeli " << preview1->getName() << "\n";
        delete preview2;
    }
    else if (choice == 2 && cash >= price2)
    {
        perks.push_back(preview2);
        cash -= price2;
        std::cout << "Membeli " << preview2->getName() << "\n";
        delete preview1;
    }
    else
    {
        std::cout << "Lewati toko\n";
        delete preview1;
        delete preview2;
    }
}
