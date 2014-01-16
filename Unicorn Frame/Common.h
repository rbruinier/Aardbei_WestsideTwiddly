#ifndef __COMMON_H_
#define __COMMON_H_

extern dword *extraTextures[];

extern bool TGALoadAndSave;

class ClearBuffers: public Effect 
{
public:
	void	Do(dword);
	void    Receive(dword message, dword timer);
};

class Noise01: public Effect 
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	dword	*noise;
};

class Noise02: public Effect 
{
public:
	bool	Init();
	void	Do(dword);
	void	DeInit();

private:
	dword	*noise;
};

class FlashWhite: public Effect {
	public:
		void	Do(dword);
};

class FlashBlack: public Effect {
	public:
		void	Do(dword);
};

class FadeWhite: public Effect {
	public:
		void	Do(dword);
};

class FadeBlack: public Effect {
	public:
		void	Do(dword);
};

class FlashWhiteFast: public Effect {
	public:
		bool	Init();
		void	Do(dword);
		void	Receive(dword message, dword timer);

		int		mode;
};

class FlashBlackFast: public Effect {
	public:
		void	Do(dword);
};

class FadeWhiteFast: public Effect {
	public:
		void	Do(dword);
};

class FadeBlackFast: public Effect {
	public:
		void	Do(dword);
};


void LoadAllTextures();
void RenderLoader(float percent);
void ShowLoader(float percent);

extern void WINAPI SoundPrecalc(void *context, float progress);

extern dword *loadTGAFromFile(char *filename, unsigned short &width, unsigned short &height);
extern void saveTGAToFile(char *filename, dword *source, unsigned short width, unsigned short height);

extern dword *loadATSFromFile(char *filename, int &width, int &height);
extern dword *loadATSFromMem(byte *atsMem, int &width, int &height);

#endif
