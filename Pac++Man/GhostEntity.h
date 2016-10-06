/****************************************************************************
File: GhostEntity.h
Author: fookenCode
****************************************************************************/
#ifndef _GHOST_ENTITY_H_
#define _GHOST_ENTITY_H_

#include "MovingEntity.h"

class GhostEntity : public MovingEntity {
private:
    enum GHOST_STATE {INVULNERABLE=0,VULNERABLE};
    int mVulnerableStatus,mColor, mRespawnTimer;
    double timeToSwitchDir;
    char mGhostIcon;
    bool mActive;

    Entity *mTarget;
public:
    GhostEntity();
    virtual ~GhostEntity() { }

    void setTarget(Entity *newTarget) { if (newTarget != nullptr) mTarget = newTarget; }
    const Entity *getTarget() { return mTarget; }

    void setActive(bool status) { this->mActive = status; }
    bool isActive() { return this->mActive; }

    void setGhostColor(int color) { this->mColor = color; }
    int getGhostColor() { return (mVulnerableStatus == VULNERABLE)? GHOST_BLUE:this->mColor; }

    void setGhostIcon(char icon) { this->mGhostIcon = icon; }
    char getGhostIcon() { return this->mGhostIcon; }

    void setVulnerable(bool status) { (status) ? mVulnerableStatus = VULNERABLE : mVulnerableStatus = INVULNERABLE; setInvalidated(true); }
    bool isVulnerable() { return (mVulnerableStatus == VULNERABLE); }

    void setRespawnTimer(int timer) { this->mRespawnTimer = timer; }
    int getRespawnTimer() { return this->mRespawnTimer; }

    void initializeGhost();
    virtual void Update(unsigned validDirections, double timeStep);
    virtual void Move(double timeStep);
    virtual void Render();
    virtual void Reset();
};
#endif // _GHOST_ENTITY_H_