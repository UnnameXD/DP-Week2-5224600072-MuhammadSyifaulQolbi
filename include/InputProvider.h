#ifndef INPUTPROVIDER_H
#define INPUTPROVIDER_H

#include <string>

// Abstract InputProvider — Invariant interface
// RunSession only calls getHitOrStand() and getShopChoice(); it never reads cin directly.
// Mutable: can be swapped to AI input, test input, network input without touching RunSession.
class InputProvider
{
public:
    virtual char getHitOrStand() = 0;
    virtual int  getShopChoice() = 0;
    virtual ~InputProvider() {}
};

// Default: reads from standard input
class ConsoleInputProvider : public InputProvider
{
public:
    char getHitOrStand() override;
    int  getShopChoice() override;
};

#endif
