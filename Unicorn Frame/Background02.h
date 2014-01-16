#ifndef __BACKGROUND02_H__
#define __BACKGROUND02_H__

class Background02: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
};

#endif