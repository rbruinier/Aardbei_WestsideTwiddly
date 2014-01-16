#ifndef __SCAPE04_H__
#define __SCAPE04_H__

class Scape04: public ScapeEffect
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

};

extern	void Scape04CreateFlatSurface(float *heightMap);

#endif