#ifndef __LIQUID_H__
#define __LIQUID_H__

class Liquid: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
	Object * objOud;
};

#endif
