#include "InputProvider.h"
#include <iostream>

char ConsoleInputProvider::getHitOrStand()
{
    char c;
    std::cin >> c;
    return c;
}

int ConsoleInputProvider::getShopChoice()
{
    int n;
    std::cin >> n;
    return n;
}
