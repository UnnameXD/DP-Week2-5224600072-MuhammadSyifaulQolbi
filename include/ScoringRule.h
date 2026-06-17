#ifndef SCORINGRULE_H
#define SCORINGRULE_H

#include <vector>
#include "Perk.h"

// Abstract ScoringRule — Invariant interface
// Separates "how damage is calculated" from the core loop
class ScoringRule
{
public:
    // Returns final damage after applying all perks
    virtual int calculateDamage(int playerTotal, const std::vector<Perk*>& perks) = 0;
    virtual ~ScoringRule() {}
};

// Mutable: default implementation — applies each perk's modifier in sequence
class StandardScoringRule : public ScoringRule
{
public:
    int calculateDamage(int playerTotal, const std::vector<Perk*>& perks) override;
};

#endif
