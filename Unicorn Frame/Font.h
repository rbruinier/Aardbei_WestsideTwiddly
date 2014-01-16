#ifndef __FONT_H__
#define __FONT_H__

class Font: public Effect
{
public:
	bool	Init();
	void	Do(char *tekst, float x, float y, dword color);
	void	DeInit();

private:
	SpriteDrawer *spriteDrawer;
};

#endif