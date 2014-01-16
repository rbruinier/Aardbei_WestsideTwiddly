#ifndef __BRICKS_H__
#define __BRICKS_H__

class Bricks: public Effect
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene *scene;
};

#endif