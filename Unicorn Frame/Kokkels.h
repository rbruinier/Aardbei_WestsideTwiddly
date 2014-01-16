#ifndef __KOKKELS_H__
#define __KOKKELS_H__

class Kokkels: public Effect
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
	void	Receive(dword Message, dword timer);

private:
	Scene   *scene;
	Object	*tube;

	int		currentCam;

	float	camTimer1;
	float	camTimer2;
	float   morphTimer;

	bool	morphing;
};

#endif