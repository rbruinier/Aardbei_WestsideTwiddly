#ifndef __bsPlayer_h
#define __bsPlayer_h

//#include "isplayer.h"
//#include "issynth.h"

class isPlayer;
class isSynth;
struct GTK;

#include <windows.h>

class bsTimer
{
public:
	inline bsTimer() { 
		QueryPerformanceFrequency(&bsTimerFreq); 
		QueryPerformanceCounter(&TimeStart); 
	}
	inline ~bsTimer() {}

	inline float getTime() {
		QueryPerformanceCounter(&TimeCur);
		return ((float)(TimeCur.QuadPart-TimeStart.QuadPart)/(float)bsTimerFreq.QuadPart);
	}
	inline void reset() {
		QueryPerformanceCounter(&TimeStart);
	}

private:
	LARGE_INTEGER bsTimerFreq;	// bsTimer Frequency.
	LARGE_INTEGER TimeStart;	// Time of start.
	LARGE_INTEGER TimeCur;		// Current time.

};

class bsPlayer
{
public:
	// memory loader, with support for precalc
	bsPlayer(void *buff, HWND hWnd, bool precalc,int precalcEndPattern);
	// no music (but with full sync support, therefor the bpm has to be known)
	bsPlayer(float bpm);

	~bsPlayer();
	void Start(int startPos=0);
	int Row();
	int Pos();
	float Time(); // in seconds
	float GetCPUUsage(); // in percentage

private:

	// for "no music" support
	float rowPerSample;
	bool noMusic;
	float startTime;

	bsTimer timer;
	
	// for music
	isSynth *syn;
	GTK *tune;
	isPlayer *pl;
};

#endif
