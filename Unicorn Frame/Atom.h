#ifndef __ATOM_H__
#define __ATOM_H__

class Atom: public Effect 
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
};

#endif