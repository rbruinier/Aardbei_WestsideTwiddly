#ifndef __SMOKESYS_H__
#define __SMAKESYS_H__

class SmokeSys: public SpriteGroup
{
public:
	SmokeSys(int nrSprites, float size, dword color, float duration, UniVector &basePos, char * texture);

	void Update(dword timer);
	void Reset();

	UniVector basePos;
	UniVector defVel;
	UniVector acc;
	UniVector defRandVel;

private:
	float *time;
	UniVector *vel;
	UniVector *startPos;

	float duration;

	dword oldTimer;

	dword color;
};

#endif