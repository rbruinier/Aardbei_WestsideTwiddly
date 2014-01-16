#ifndef __TENTACLES_H__
#define __TENTACLES_H__

class Tentacles: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
	Object  *oldObject;
};

#endif