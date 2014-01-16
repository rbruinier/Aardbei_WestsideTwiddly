#ifndef __FASTROAD_H__
#define __FASTROAD_H__

class FastRoad: public Effect 
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
};

#endif