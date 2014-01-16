#ifndef __STARFIELD_H__
#define __STARFIELD_H__

class Starfield: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
	dword	oldTimer;

};

#endif