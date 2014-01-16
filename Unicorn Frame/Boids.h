#ifndef __BOIDS_H__
#define __BOIDS_H__

class Boids: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
};

#endif