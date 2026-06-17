#include "Card.h"
#include <cstdlib>

Card drawCard()
{
    Card c;
    c.rank = rand() % 13 + 1;

    std::string suits[4] = { "Spade", "Heart", "Club", "Diamond" };
    c.suit = suits[rand() % 4];

    return c;
}

int cardValue(int rank)
{
    if (rank > 10) return 10;
    if (rank == 1)  return 11;
    return rank;
}

std::string cardName(int rank)
{
    if (rank == 1)  return "A";
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    return std::to_string(rank);
}

int calculateTotal(const std::vector<Card>& hand)
{
    int total    = 0;
    int aceCount = 0;

    for (const Card& c : hand)
    {
        total += cardValue(c.rank);
        if (c.rank == 1) aceCount++;
    }

    while (total > 21 && aceCount > 0)
    {
        total    -= 10;
        aceCount -= 1;
    }

    return total;
}

void showHand(const std::vector<Card>& hand, bool hideFirst)
{
    for (size_t i = 0; i < hand.size(); i++)
    {
        if (i == 0 && hideFirst)
            std::cout << "[Hidden] ";
        else
            std::cout << cardName(hand[i].rank) << "(" << hand[i].suit << ") ";
    }
    std::cout << "\n";
}
