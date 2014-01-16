#ifndef __POEM_H__
#define __POEM_H__

class Poem: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();
	void	Receive(dword Message, dword timer);

private:
	float	camTimer1;
	int		curText;
};

#endif