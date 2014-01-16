#ifndef __SPIKEY_H__
#define __SPIKEY_H__

class Spikey: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	Scene	*scene;
	Object  *oldPlane;
};

#endif