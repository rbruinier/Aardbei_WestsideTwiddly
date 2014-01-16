#ifndef __SCAPE05_H__
#define __SCAPE05_H__

extern Object *genPilar(int nrSegmentsX, int nrSegmentsY, float radius, float length, char *texture);

class Scape05: public ScapeEffect
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
	void	Receive(dword Message, dword timer);

private:
	Object  *oldSphere;

	int		currentCam;

	float	camTimer1;
	float	transTimer;
	float   morphTimer;

	bool	morphing;
	bool	transition;

	bool	credits;
	float	creditTimer;

	bool	blue;
	float	blueTimer;
};

#endif