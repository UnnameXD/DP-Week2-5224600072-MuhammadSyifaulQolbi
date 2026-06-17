#include "ScoringRule.h"

// StandardScoringRule: applies each perk modifier in sequence.
// Mutable — can be replaced with a different rule (e.g. MultiplicativeScoringRule)
// without touching RunSession.
int StandardScoringRule::calculateDamage(int playerTotal, const std::vector<Perk*>& perks)
{
    int damage = playerTotal;
    for (Perk* p : perks)
        damage = p->modifyDamage(damage);
    return damage;
}
