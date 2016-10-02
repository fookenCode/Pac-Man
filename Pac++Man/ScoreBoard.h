#ifndef _SCORE_BOARD_H_
#define _SCORE_BOARD_H_

#include "Entity.h"
#include "Constants.h"

class ScoreBoard : public Entity {
private:
    long scoreTotal;
public:
    ScoreBoard();
    virtual ~ScoreBoard();

    long getScoreTotal() { return scoreTotal; }

    void setScoreTotal(int newScoreTotal) { scoreTotal = newScoreTotal; }
    void addScoreTotal(int scoreTotalToAdd) { scoreTotal += scoreTotalToAdd; }
    void addPointsForPickup(char mapCharacter) { (mapCharacter == POWER_PELLET_CHARACTER) ? scoreTotal += POWER_PELLET_SCORE_AMOUNT : scoreTotal += NORML_PELLET_SCORE_AMOUNT; }

    virtual void Render();
    virtual void Reset();
};
#endif //_SCORE_BOARD_H_