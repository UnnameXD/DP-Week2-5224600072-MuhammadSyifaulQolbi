#include "RunSession.h"
#include "Card.h"
#include "PerkFactory.h"
#include "Shop.h"

#include <iostream>
#include <vector>

RunSession::RunSession(ScoringRule* scoring, InputProvider* input)
    : m_scoring(scoring), m_input(input)
{}

RunSession::~RunSession() {}

// ============================================================
//  INVARIANT CORE LOOP
//  Phase order: PlayerAction → SystemEvaluate → ApplyDamage → UpdateState
//  This sequence must NEVER change.
// ============================================================
void RunSession::run()
{
    int playerHP = 63;
    int round    = 1;
    int cash     = 0;

    std::vector<Perk*> perks;

    // --- Starting perk (mutable: factory decides which perk, or none) ---
    Perk* startPerk = PerkFactory::createStartingPerk();
    if (startPerk)
    {
        perks.push_back(startPerk);
        std::cout << "Starting Perk: " << startPerk->getName() << "\n";
    }
    else
    {
        std::cout << "Tidak ada starting perk\n";
    }

    // ---- Outer loop: one iteration = one round (boss fight) ----
    while (playerHP > 0)
    {
        int dealerHP = 12 + round * 5;  // Mutable: scaling formula bisa diubah

        std::cout << "\n====================\n";
        std::cout << "RONDE " << round << "\n";
        std::cout << "HP Dealer: " << dealerHP << "\n";

        // ---- Inner loop: one iteration = one blackjack hand ----
        while (playerHP > 0 && dealerHP > 0)
        {
            std::vector<Card> playerHand;
            std::vector<Card> dealerHand;

            // Initial deal
            playerHand.push_back(drawCard());
            playerHand.push_back(drawCard());
            dealerHand.push_back(drawCard());
            dealerHand.push_back(drawCard());

            std::cout << "\nHP Player: " << playerHP
                      << " | HP Dealer: " << dealerHP << "\n";

            // === PHASE 1: PlayerAction ===
            playerAction(playerHand);

            // === PHASE 2: SystemEvaluate (dealer draws) ===
            systemEvaluate(dealerHand);

            // Show results
            std::cout << "\n=== HASIL HAND ===\n";
            std::cout << "Kartu Dealer: ";
            showHand(dealerHand, false);

            int dealerTotal = calculateTotal(dealerHand);
            int playerTotal = calculateTotal(playerHand);

            std::cout << "Total Dealer: " << dealerTotal << "\n";
            std::cout << "Kartu Kamu: ";
            showHand(playerHand, false);
            std::cout << "Total Kamu: " << playerTotal << "\n";

            // === PHASE 3: ApplyDamage ===
            applyDamage(playerTotal, dealerTotal, playerHP, dealerHP, perks);

            // === PHASE 4: UpdateState — checked by while condition ===
        }

        // --- UpdateState: check overall win/loss ---
        if (playerHP <= 0) break;

        std::cout << "\nRonde " << round << " selesai!\n";

        // Post-round: heal + reward (mutable values)
        playerHP += 21;
        std::cout << "Sembuh 21 HP\n";
        std::cout << "HP saat ini: " << playerHP << "\n";

        int reward = 10 + round * 5;   // Mutable: reward formula
        cash += reward;
        std::cout << "Mendapat " << reward << " gold\n";
        std::cout << "Total gold: " << cash << "\n";

        // Mutable phase between rounds: shop can be swapped out entirely
        Shop::openShop(cash, perks, *m_input);

        round++;
    }

    std::cout << "\n===== GAME OVER =====\n";

    for (Perk* p : perks)
        delete p;
}

// ============================================================
//  Phase implementations
// ============================================================

void RunSession::playerAction(std::vector<Card>& playerHand)
{
    while (true)
    {
        std::cout << "\nKartu Dealer: ";
        // Show dealer's first card hidden (we only have playerHand here, dealer shown elsewhere)
        std::cout << "Kartu Kamu: ";
        showHand(playerHand, false);

        int total = calculateTotal(playerHand);
        std::cout << "Total: " << total << "\n";

        if (total > 21)
        {
            std::cout << "Bust!\n";
            break;
        }

        std::cout << "Hit(h) Stand(s): ";
        char choice = m_input->getHitOrStand();

        if (choice == 'h')
            playerHand.push_back(drawCard());
        else
            break;
    }
}

void RunSession::systemEvaluate(std::vector<Card>& dealerHand)
{
    while (calculateTotal(dealerHand) < 17)
        dealerHand.push_back(drawCard());
}

void RunSession::applyDamage(int playerTotal, int dealerTotal,
                              int& playerHP, int& dealerHP,
                              const std::vector<Perk*>& perks)
{
    // Scoring delegated to ScoringRule — RunSession doesn't know the formula
    int damage = m_scoring->calculateDamage(playerTotal, perks);

    if (playerTotal <= 21 && dealerTotal > 21)
    {
        dealerHP -= damage;
        std::cout << "Dealer Bust! Dealer menerima " << damage << " damage\n";
    }
    else if (playerTotal > 21)
    {
        playerHP -= dealerTotal;
        std::cout << "Kamu Bust! Kamu menerima " << dealerTotal << " damage\n";
    }
    else if (playerTotal > dealerTotal)
    {
        dealerHP -= damage;
        std::cout << "Kamu Menang Hand! Dealer menerima " << damage << " damage\n";
    }
    else if (playerTotal < dealerTotal)
    {
        playerHP -= dealerTotal;
        std::cout << "Dealer Menang Hand! Kamu menerima " << dealerTotal << " damage\n";
    }
    else
    {
        std::cout << "Seri\n";
    }
}

bool RunSession::isRoundOver(int playerHP, int dealerHP)
{
    return playerHP <= 0 || dealerHP <= 0;
}
