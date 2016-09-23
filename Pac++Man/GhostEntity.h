/****************************************************************************
File: GhostEntity.h
Author: fookenCode
****************************************************************************/
#ifndef _GHOST_ENTITY_H_
#define _GHOST_ENTITY_H_

#include "Entity.h"

class GhostEntity : public Entity {
private:
	enum GHOST_STATE {INVULNERABLE=0,VULNERABLE};
	int mVulnerableStatus,mColor, mRespawnTimer;
	char mGhostIcon;
	bool mActive;

public:
	GhostEntity();
	virtual ~GhostEntity() { }

	void setActive(bool status) { this->mActive = status; }
	bool isActive() { return this->mActive; }

	void setGhostColor(int color) { this->mColor = color; }
	int getGhostColor() { return (mVulnerableStatus == VULNERABLE)? GHOST_BLUE:this->mColor; }

	void setGhostIcon(char icon) { this->mGhostIcon = icon; }
	char getGhostIcon() { return this->mGhostIcon; }

	void setVulnerable(bool status) { (status) ? mVulnerableStatus = VULNERABLE : mVulnerableStatus = INVULNERABLE; }
	bool isVulnerable() { return (mVulnerableStatus == VULNERABLE); }

	void setRespawnTimer(int timer) { this->mRespawnTimer = timer; }
	int getRespawnTimer() { return this->mRespawnTimer; }

	virtual void render();
	virtual void reset();
};
#endif // _GHOST_ENTITY_H_