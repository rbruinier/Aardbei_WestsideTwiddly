#ifndef __SCAPE03_H__
#define __SCAPE03_H__

class Scape03: public ScapeEffect
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
	void	Receive(dword Message, dword timer);

private:
	int		currentCam;
	float	camTimer1;
	float	camTimer2;
	float   camTimer3;
	float	camTimer4;
	float	camTimer5;

	Scene	*scene2;
};

#endif