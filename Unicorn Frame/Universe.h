#ifndef __UNIVERSE_H__
#define __UNIVERSE_H__

class Universe: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
	Scene	*scene2;
};

#endif
