#include "Perk.h"

// --- Bonus21Damage ---
int Bonus21Damage::modifyDamage(int baseDamage)
{
    return static_cast<int>(baseDamage * 1.21);
}
std::string Bonus21Damage::getName() { return "+21% Damage"; }

// --- FlatDamagePlus5 ---
int FlatDamagePlus5::modifyDamage(int baseDamage)
{
    return baseDamage + 5;
}
std::string FlatDamagePlus5::getName() { return "+5 Flat Damage"; }

// --- BlackjackDouble ---
int BlackjackDouble::modifyDamage(int baseDamage)
{
    if (baseDamage == 21) return baseDamage * 2;
    return baseDamage;
}
std::string BlackjackDouble::getName() { return "Blackjack Double Damage"; }
