#ifndef RUNSESSION_H
#define RUNSESSION_H

#include <vector>
#include "Card.h"
#include "Perk.h"
#include "ScoringRule.h"
#include "InputProvider.h"

// RunSession — owns the INVARIANT core loop.
// The phase order: PlayerAction → SystemEvaluate → ApplyDamage → UpdateState
// must NEVER change. Only the collaborators (ScoringRule, InputProvider) are mutable.
class RunSession
{
public:
    // Dependencies injected — RunSession doesn't construct them itself
    RunSession(ScoringRule* scoring, InputProvider* input);
    ~RunSession();

    void run();

private:
    ScoringRule*        m_scoring;
    InputProvider*      m_input;

    // --- Invariant phases (private — external code must not reorder them) ---

    // Phase 1: deal cards, player hits or stands
    void playerAction(std::vector<Card>& playerHand);

    // Phase 2: dealer draws to 17+
    void systemEvaluate(std::vector<Card>& dealerHand);

    // Phase 3: compare totals, apply damage via ScoringRule
    void applyDamage(int playerTotal, int dealerTotal,
                     int& playerHP, int& dealerHP,
                     const std::vector<Perk*>& perks);

    // Phase 4: check win/loss condition
    bool isRoundOver(int playerHP, int dealerHP);
};

#endif
