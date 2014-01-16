#ifndef __BLURS_H__
#define __BLURS_H__

class BlurMotion: public Effect {
public:
	bool	Init();
	void    Open();
	void    Close();
	void	Do(dword);
	void	DeInit();
	void	Clear(dword color);

private:
	char	*textureName;
	int		nrLayers;
	int		activeLayer;
	int     oldTime;
};


class BlurRadial: public Effect {
public:
	bool	Init();
	void    Open();
	void    Close();
	void	Do(dword);
	void	DeInit();
	void	SetMode(int iMode);

private:
	char	*textureName;
	int		nrLayers;
	int		mode;
};


class BlurHorizontal: public Effect {
public:
	bool	Init();
	void    Open();
	void    Close();
	void	Do(dword);
	void	DeInit();

private:
	char	*textureName;
	int		nrLayers;
};


#endif