#ifndef _CREDITS_BOARD_H_
#define _CREDITS_BOARD_H_

#include "Entity.h"

class CreditsBoard : public Entity {
private:
    int creditTotal;
public:
    CreditsBoard();
    virtual ~CreditsBoard();

    int getCreditTotal() { return creditTotal; }
    void addCredits(int creditsToAdd) { creditTotal += creditsToAdd; }
    void setCredits(int newCreditTotal) { creditTotal = newCreditTotal; }
    inline void decCredits() { creditTotal--; }
    inline void incCredits() { creditTotal++; }
    virtual void Render();
    virtual void Reset();
};
#endif //_CREDITS_BOARD_H_