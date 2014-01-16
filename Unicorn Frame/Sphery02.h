#ifndef __SPHERY02_H__
#define __SPHERY02_H__

class Sphery02: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
	Object  *oldObject;
};

#endif