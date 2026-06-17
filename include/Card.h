#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

struct Card
{
    int rank;
    std::string suit;
};

Card drawCard();
int cardValue(int rank);
std::string cardName(int rank);
int calculateTotal(const std::vector<Card>& hand);
void showHand(const std::vector<Card>& hand, bool hideFirst);

#endif
