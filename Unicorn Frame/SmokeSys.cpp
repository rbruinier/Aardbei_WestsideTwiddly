#include "Main.h"

SmokeSys::SmokeSys(int nrSprites, float size, dword color, float duration, UniVector &basePos, char * texture)
{
	this->nrSprites = nrSprites;
	this->size      = size;
	this->color     = color;
	this->basePos   = basePos;

	this->texture   = NULL;
	setTexture(texture);

	enabled = true;

	blend = UNI_ADD;

	sprite = new VertexPoint[nrSprites];

	for (int i = 0; i < nrSprites; i++) {
		sprite[i].setLoc(0, 0, 0);
		sprite[i].setDiffuse(color);
	}

	vertexBuffer = new UniVBuffer((dword*) sprite, sizeof(VertexPoint), nrSprites * sizeof(VertexPoint), ENGINE_VERTEXPOINT, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS);

	basePos.x = 0;
	basePos.y = 0;
	basePos.z = 0;

	oldTimer  = 0;

	time     = new float[nrSprites];
	vel      = new UniVector[nrSprites];
	startPos = new UniVector[nrSprites];

	defVel.x = 0;
	defVel.y = 2;
	defVel.z = 0;

	defRandVel.x = 4.f;
	defRandVel.y = 4.f;
	defRandVel.z = 4.f;

	acc.x = 0;
	acc.y = 0.8f;
	acc.z = 0;

	this->duration = duration;

	Reset();
}

void SmokeSys::Reset()
{
	for (int i = 0; i < nrSprites; i++)
	{
		startPos[i] = this->basePos;

		vel[i].x = defVel.x + ((uniRand() * defRandVel.x) - (defRandVel.x / 2.f));
		vel[i].y = defVel.y + ((uniRand() * defRandVel.y) - (defRandVel.y / 2.f));
		vel[i].z = defVel.z + ((uniRand() * defRandVel.z) - (defRandVel.z / 2.f));

		time[i] = (-uniRand() * duration) * 1000.f;
	}
}

void SmokeSys::Update(dword timer)
{
	int r = (color >> 16) & 255;
	int g = (color >>  8) & 255;
	int b = (color >>  0) & 255;
	for (int i = 0; i < nrSprites; i++)
	{
		float difTimer = (timer - time[i]) / 1000.f;

		if (difTimer > duration)
		{
			time[i]  = timer;
			difTimer = 0;
			vel[i].x = defVel.x + ((uniRand() * defRandVel.x) - (defRandVel.x / 2.f));
			vel[i].y = defVel.y + ((uniRand() * defRandVel.y) - (defRandVel.y / 2.f));
			vel[i].z = defVel.z + ((uniRand() * defRandVel.z) - (defRandVel.z / 2.f));
			startPos[i] = basePos;
		}

		sprite[i].x = startPos[i].x + (vel[i].x * difTimer) + (acc.x * pow(difTimer, 2));
		sprite[i].y = startPos[i].y + (vel[i].y * difTimer) + (acc.y * pow(difTimer, 2));
		sprite[i].z = startPos[i].z + (vel[i].z * difTimer) + (acc.z * pow(difTimer, 2));

		int fade = 255 - ((difTimer / duration) * 255);

		sprite[i].diffuse = (0xff << 24) | (((r * fade) >> 8) << 16) | (((g * fade) >> 8) << 8) | (((b * fade) >> 8));
	}
}