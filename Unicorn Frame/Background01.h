#ifndef __BACKGROUND01_H__
#define __BACKGROUND01_H__

class Background01: public Effect {
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	SpriteDrawer *spriteDrawer;
};

#endif