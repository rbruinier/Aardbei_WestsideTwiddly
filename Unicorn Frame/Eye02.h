#ifndef __EYE02_H__
#define __EYE02_H__

struct Greet
{
	char *name;
	int  x;
};

class Eye02: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
	void	Receive(dword Message, dword timer);

private:
	Scene	*scene;

	float	camTimer1;
};

#endif