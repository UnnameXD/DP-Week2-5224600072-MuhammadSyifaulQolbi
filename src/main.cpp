#include "RunSession.h"
#include "ScoringRule.h"
#include "InputProvider.h"
#include <ctime>
#include <cstdlib>

int main()
{
    srand(time(0));

    // Inject mutable dependencies — swap these without touching RunSession
    StandardScoringRule  scoring;
    ConsoleInputProvider input;

    RunSession game(&scoring, &input);
    game.run();

    return 0;
}
