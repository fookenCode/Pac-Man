#ifndef _LIVES_BOARD_H_
#define _LIVES_BOARD_H_

#include "Entity.h"

class LivesBoard : public Entity {
private:
    int livesLeft, maxLives;
public:
    LivesBoard();
    virtual ~LivesBoard();

    int getLivesLeft() { return livesLeft; }
    int getMaxLives() { return maxLives; }

    void setLivesLeft(int newLivesTotal) { livesLeft = newLivesTotal; setInvalidated(true); }
    inline void decLives() { livesLeft--; setInvalidated(true); }
    inline void incLives() { livesLeft++; setInvalidated(true); }

    void setMaxLives(int newMaxLivesTotal) { maxLives = newMaxLivesTotal; }

    virtual void Render();
    virtual void Reset();
};

#endif // _LIVES_BOARD_H_