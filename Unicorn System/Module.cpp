/*

	Pyramid DX8 System - HTML Log Code (source file)


	(c) 2001, Robert Jan Bruinier

*/


#include "Unicorn.h"

#ifdef UNI_TINY_FMOD


// for new lib
bsPlayer *steffoPlayer = NULL;
int steffoPlayerStartPos = 0;


void fmodLoadMod(void *data, int length) 
{
//	steffoPlayer = new bsPlayer(data, uniSystem->windowHandle, false, 0x2d, precalcum);
	steffoPlayer = new bsPlayer(data, uniSystem->windowHandle, false, 0x5);
}

void fmodLoadTrack(void *data, bool precalc, BS_PRECALC_PROC callback) 
{
//	steffoPlayer = new bsPlayer(data, uniSystem->windowHandle, false, 0x2d, precalcum);
	steffoPlayer = new bsPlayer(data, uniSystem->windowHandle, precalc, 0x2f, callback);
}

void fmodPlay(int iOrder) 
{
	steffoPlayerStartPos = iOrder;
	steffoPlayer->Start(iOrder);
}

void fmodStop() 
{
	delete steffoPlayer;
}

void fmodGetInfo(int &order, int &row, int &sync) 
{
	if (steffoPlayer != 0)
	{
		order = steffoPlayer->Pos() + steffoPlayerStartPos;
		row   = steffoPlayer->Row() - (steffoPlayer->Pos() * 48);
		sync  = (int) steffoPlayer->Time();
	}
	else
	{
		order = row = sync = 0;
	}
}

/*
// for lib from 03-07
bsPlayer *steffoPlayer = NULL;
int steffoPlayerStartPos = 0;

void fmodLoadMod(void *data, int length) 
{
	steffoPlayer = new bsPlayer(data, uniSystem->windowHandle, false, 0x2d);
}

void fmodPlay(int iOrder) 
{
	steffoPlayerStartPos = iOrder;
	steffoPlayer->Start(iOrder);
}

void fmodStop() 
{
	delete steffoPlayer;
}

void fmodGetInfo(int &order, int &row, int &sync) 
{
	if (steffoPlayer != 0)
	{
		order = steffoPlayer->Pos() + steffoPlayerStartPos;
		row   = steffoPlayer->Row() - (steffoPlayer->Pos() * 48);
		sync  = (int) steffoPlayer->Time();
	}
	else
	{
		order = row = sync = 0;
	}
}
*/



/*


// for no music (but timing)
unsigned int fakeMusicTimer;

void fmodLoadMod(void *data, int length) 
{
}

void fmodPlay(int iOrder) 
{
	fakeMusicTimer = timeGetTime();
	steffoPlayerStartPos = iOrder;
}

void fmodStop() 
{
}

void fmodGetInfo(int &order, int &row, int &sync) 
{
	float sec = (timeGetTime() - fakeMusicTimer) / 1000.f;

	order  = (int)floor(sec / 8);
	row    = ((sec - (order * 8)) / 8.f) * 48.f;
	sync   = sec;
	order += steffoPlayerStartPos;
}
*/

#endif