#ifndef __SPHERY01_H__
#define __SPHERY01_H__

class Sphery01: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
	void	Receive(dword Message, dword timer);

private:
	Scene	*scene;
	Object  *oldObject;

	int currentCam;
	float camTimer1;
};

#endif