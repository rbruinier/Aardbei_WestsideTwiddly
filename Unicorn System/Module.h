/*

	Pyramid DX8 System - HTML Log Code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/


#ifndef __MODULE_H__
#define __MODULE_H__

#ifdef UNI_TINY_FMOD


class isPlayer;
class isSynth;
struct GTK;
typedef void (WINAPI *BS_PRECALC_PROC)(LPVOID context, float progress);

extern void fmodLoadTrack(void *data, bool precalc = false, BS_PRECALC_PROC callback = 0);
extern void fmodLoadMod(void *data, int length);
extern void fmodPlay(int iOrder);
extern void fmodStop();
extern void fmodGetInfo(int &order, int &row, int &sync);

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
	bsPlayer(void *buff, HWND hWnd, bool precalc,int precalcEndPattern, BS_PRECALC_PROC callback=0, void *context=0);
	// no music (but with full sync support, therefor the bpm has to be known)
	bsPlayer(float bpm);

	~bsPlayer();
	void Start(int startPos=0);
	int Row();
	int Pos();
	float Time(); // in seconds
	float GetCPUUsage(); // in percentage
	void process(void);

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

extern bsPlayer *steffoPlayer;

/*
// for lib from 0307
class isPlayer;
class isSynth;
struct GTK;

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


extern bsPlayer *steffoPlayer;
*/

#endif
#endif

