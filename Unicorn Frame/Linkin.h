#ifndef __LINKIN_H__
#define __LINKIN_H__

class Linkin: public Effect 
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
	void	Receive(dword Message, dword timer);

private:
	void	bendTentacle(Vertex *vertex, int nrSegmentsX, int nrSegmentsY, float radius, float yScale, int timer);

	Scene	*scene;
	Scene	*scene2;
	D3DVECTOR *source;

	float	camTimer1;
	float	camTimer2;
	float	camTimer3;
	float	camTimer4;

	int		currentCam;

	bool	ugh;
};

#endif
