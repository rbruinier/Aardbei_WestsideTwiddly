#ifndef __EYE_H__
#define __EYE_H__

class Eye: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
	void	Receive(dword Message, dword timer);

private:
	Scene	*scene;

	float	camTimer1;
	float	camTimer2;
	float	camTimer3;
	float	camTimer4;
};

#endif