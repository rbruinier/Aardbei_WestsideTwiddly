#include "Main.h"

struct texture
{
	char *name;
	unsigned char *source;
	int	 flags;
	bool toMem;
};

extern unsigned char ATS_DISTORT5[];

extern bool precalcMusic;

dword *extraTextures[16];

#define TEXTURELOAD_NR 65

texture texturesToLoad[] =
{
	{"sprite02",	ATS_SPRITE_2,		UNI_MIPMAP,	true},		// eye, background1
		
	{"env3",		ATS_ENV3,			UNI_MIPMAP,	true},		// sphery03, sphery01

	{"water01",		ATS_WATER2,			UNI_MIPMAP,	true},		// liquid
	{"pelare",		ATS_PELARE,			UNI_MIPMAP,	true},		// liquid
	{"tak",			ATS_TAK,			UNI_MIPMAP,	true},		// liquid
	{"env2",		ATS_ENV2,			UNI_MIPMAP,	true},		// liquid
	{"raindrop1",	ATS_RAINDROP,		0,			true},		// liquid
	{"",			ATS_GROUND3,		0,			false},		// vulcan
	{"",			ATS_GROUNDNEDAN,	0,			false},		// liquid
	{"",			ATS_GROUNDOVAN,		0,			false},		// liquid
	{"",			ATS_WIT,			0,			false},		// liquid, vulcan
	{"sprite01",	ATS_SPRITE01,		0,			true},		// liquid, starfield	// !!!

	{"sand01",		ATS_KOKKELGROUND,	UNI_MIPMAP,	true},		// kokkels
	{"texture06",	ATS_TENTACLE01,		UNI_MIPMAP,	true},		// kokkels
	{"texture07",	ATS_SHELL01,		UNI_MIPMAP,	true},		// kokkels
	{"bubble01",	ATS_BUBBLE01,		0,			true},		// kokkels

	{"smoke01",		ATS_SMOKE01,		0,			true},		// kokkels, vulcan

	{"vulcansprite",ATS_VULCANSPRITE,	0,			true},		// vulcan				// !!!
	{"",			ATS_VULCANGROUND1,	0,			false},		// vulcan
	{"",			ATS_VULCANGROUND2,	0,			false},		// vulcan
	{"vulcanback",	ATS_VULCANBACK,		UNI_MIPMAP,	true},		// vulcan

	{"nicesand",	ATS_NICESAND,		0,			false},		// liquid

	{"stars01",		ATS_STARS02,		UNI_MIPMAP,	true},		// universe
	{"nevel01",		ATS_NEVEL2,			UNI_MIPMAP,	true},		// universe
	{"little",		ATS_LITTLE,			0,			true},		// universe				// !!!

	{"sweet01",		ATS_SWEET1,			UNI_MIPMAP,	true},		// sweet
	{"sweet02",		ATS_SWEET2,			UNI_MIPMAP,	true},		// sweet
	{"sweet04",		ATS_SWEET4,			UNI_MIPMAP,	true},		// sweet

	{"back01",		ATS_BACKGROUND1,	0,			true},		// background01

	{"sphery01",	ATS_SPHERYENV1,		UNI_MIPMAP,	true},		// sphery01

	{"title",		ATS_TITLE,			0,			true},		// title

	{"greet00",		ATS_ARRAY,			0,			true},		// greet
	{"greet01",		ATS_CONFINE,		0,			true},		// greet
	{"greet02",		ATS_CRYONICS,		0,			true},		// greet
	{"greet03",		ATS_EXCEED,			0,			true},		// greet
	{"greet04",		ATS_FARBRAUSCH,		0,			true},		// greet
	{"greet05",		ATS_HAUJOBB,		0,			true},		// greet
	{"greet06",		ATS_OUTBREAK,		0,			true},		// greet
	{"greet07",		ATS_REBELS,			0,			true},		// greet
	{"greet08",		ATS_REPLAY,			0,			true},		// greet
	{"greet09",		ATS_SUNFLOWER,		0,			true},		// greet
	{"greet10",		ATS_TBL,			0,			true},		// greet
	{"greet11",		ATS_TEASTATE,		0,			true},		// greet
	{"greet12",		ATS_TPOLM,			0,			true},		// greet
	{"greet13",		ATS_WRATH,			0,			true},		// greet

	{"credit00",	ATS_ROB,			0,			true},		// credits
	{"credit01",	ATS_OYISEER,		0,			true},		// credits
	{"credit02",	ATS_STEFFO,			0,			true},		// credits
	{"aardbei",		ATS_AARDBEI,		0,			true},		// credits

	{"sun",			ATS_SUN,			0,			true},		// liquid
	{"vulcantext1",	ATS_VULCANTEXT1,	0,			true},		// poem
	{"vulcantext2",	ATS_VULCANTEXT2,	0,			true},		// poem
	{"vulcantext3",	ATS_VULCANTEXT3,	0,			true},		// poem
	{"kokkeltext1",	ATS_KOKKELTEXT1,	0,			true},		// poem
	{"kokkeltext2",	ATS_KOKKELTEXT2,	0,			true},		// poem
	{"kokkeltext3",	ATS_KOKKELTEXT3,	0,			true},		// poem
	{"kokkeltext4",	ATS_KOKKELTEXT4,	0,			true},		// poem
	{"liquidtext1",	ATS_LIQUIDTEXT1,	0,			true},		// poem
	{"liquidtext2",	ATS_LIQUIDTEXT2,	0,			true},		// poem
	{"liquidtext3",	ATS_LIQUIDTEXT3,	0,			true},		// poem
	{"liquidtext4",	ATS_LIQUIDTEXT4,	0,			true},		// poem
	{"liquidtext5",	ATS_LIQUIDTEXT5,	0,			true},		// poem
	{"liquidtext6",	ATS_LIQUIDTEXT6,	0,			true},		// poem
	{"liquidtext7",	ATS_LIQUIDTEXT7,	0,			true},		// poem

	{"distort5",	ATS_DISTORT5,		0,			true},		// noise

	{NULL,			NULL,				NULL,		NULL}
};

dword loadingBar[20] =
{
	0xffc600,
	0xe6b200,
	0xcc9e00,
	0xb38a00,
	0x997600,
	0x806200,
	0x664F00,
	0x4D3C00,
	0x332800,
	0x1A1400,
	0x1A1400,
	0x332800,
	0x4D3C00,
	0x664F00,
	0x806200,
	0x997600,
	0xB38A00,
	0xCC9E00,
	0xE6B200,
	0xFFC600
}; 

void RenderLoader(float percent)
{
	SpriteDrawer *sd = uniSystem->spriteDrawer;
	sd->Start(0);

	int pos = floor(percent * 20.f);
	for (int i = 0; i < pos; i++)
	{
		int x1 = i * 40;
		int x2 = x1 + 40;
		sd->Draw2D(x1, 290, 0, 0, loadingBar[i],
			      x2, 290, 0, 0, loadingBar[i],
				  x1, 310, 0, 0, loadingBar[i],
				  x2, 310, 0, 0, loadingBar[i]);

	}
	if (pos < 20)
	{
		int x1 = pos * 40;
		int x2 = x1 + ((percent * 20.f) - pos) * 40;
		sd->Draw2D(x1, 290, 0, 0, loadingBar[pos],
			      x2, 290, 0, 0, loadingBar[pos],
				  x1, 310, 0, 0, loadingBar[pos],
				  x2, 310, 0, 0, loadingBar[pos]);
	}
	sd->Stop();
} 

void ShowLoader(float percent)
{
	RenderLoader(percent);
	uniShowBuffers();
	_sleep(20);
}

void WINAPI SoundPrecalc(void *context, float progress)
{
	ShowLoader(progress * 0.5f);
}

void LoadAllTextures()
{
	uniClearBuffers(UNI_BBF | UNI_ZBF, 0);

	unsigned char *currentTexture;
	int width, height;
	int index = 0;
	int extraIndex = 0;
	while ((currentTexture = texturesToLoad[index].source) != NULL)
	{
  		MSG  msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		if (msg.message == WM_QUIT)
		{
			uniQuit();
		}

#ifdef TEXTURELOAD_NR
		if (precalcMusic)
		{
			ShowLoader(((index * 0.7f) / TEXTURELOAD_NR) * 0.5f + 0.5f);
		}
		else
		{
			ShowLoader(((index * 0.7f) / TEXTURELOAD_NR) * 1.0f);
		}
#endif
		dword *data = loadATSFromMem(currentTexture, width, height);
		if (texturesToLoad[index].toMem)
		{
			uniTextureCreate(texturesToLoad[index].name, data, width, height, texturesToLoad[index].flags);
		}
		else
		{
			extraTextures[extraIndex++] = data;
		}
		index++;

		_sleep(20); 
	}

	// shake textures
	/*
	for (int i = 0; i < index; i++)
	{
		uniSetTexture(texturesToLoad[index].name);

		SpriteDrawer sd;

		float x1 = uniSystem->viewportX1;
		float y1 = uniSystem->viewportY1;
		float x2 = uniSystem->viewportX2;
		float y2 = uniSystem->viewportY2;

		sd.Start(UNI_ALPHA);
		sd.Draw2D(x1, y1, 0, 0, 0xffffff,
				  x2, y1, 1, 0, 0xffffff,
				  x1, y2, 0, 1, 0xffffff,
				  x2, y2, 1, 1, 0xffffff);
		sd.Stop();
	}
	*/
	
}

// COMMON EFFECTS

void ClearBuffers::Do(dword timer) 
{
	uniClearBuffers(UNI_BBF | UNI_ZBF, clearColor);
}

void ClearBuffers::Receive(dword message, dword timer) 
{
	clearColor = message; 
}


bool Noise01::Init() 
{
	noise = new dword[256 * 256];
	for (int i = 0; i < 256 * 256; i++)
	{
		int fade = rand() % 255;
//		noise[i] = (fade << 16) | (fade << 8) | (fade) | (0xff << 24);
		noise[i] = fade << 24;
//		noise[i] = ((rand() % 255) << 16) | ((rand() % 255) << 8) | ((rand() % 255)) | (0xff << 24);
	}

	uniTextureCreate("noise01", noise, 256, 256, 0);

	return true;
}

void Noise01::Do(dword timer) 
{
	uniSetTexture("noise01");

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
///*
    int c = 0xffffff;
	int a = 0xff - (timer / 7);
	if (a >= 0) 
	{
		float distu = uniRand() * 2.f;
		float distv = uniRand() * 2.f;
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0 + distu, 0 + distv, c,
				  x2, y1, 2 + distu, 0 + distv, c,
  				  x1, y2, 0 + distu, 2 + distv, c,
				  x2, y2, 2 + distu, 2 + distv, c);
		sd->Stop();
	}
//*/

/*
    int c = 0xffffff;
	int a = 127 - (sin(timer / 50.f) * cos(timer / 60.f) * sin(timer / 20.f) * 40.f);
	float dist = uniRand() * 2.f;
	c += a << 24;
	sd.Start(UNI_ALPHA);
	sd.Draw2D(x1, y1, 0 + dist, 0 + dist, c,
			  x2, y1, 1 + dist, 0 + dist, c,
			  x1, y2, 0 + dist, 1 + dist, c,
			  x2, y2, 1 + dist, 1 + dist, c);
	sd.Stop();
*/
}

void Noise01::DeInit() 
{
	delete [] noise;
}

bool Noise02::Init() 
{
	noise = new dword[512 * 256];
	for (int i = 0; i < 512 * 256; i++)
	{
		int fade = rand() % 255;
		noise[i] = (fade << 16) | (fade << 8) | (fade) | (0xff << 24);
//		noise[i] = fade << 24;
//		noise[i] = ((rand() % 255) << 16) | ((rand() % 255) << 8) | ((rand() % 255)) | (0xff << 24);
	}

	uniTextureCreate("noise02", noise, 512, 256, 0);

	return true;
}

void Noise02::Do(dword timer) 
{
	uniSetTexture("noise02");

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;

    int c = 0xffffff;
	int a = 0xff - (timer / 3);
	if (a >= 0) 
	{
		float dist = uniRand() * 2.f;
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0 + dist, 0 + dist, c,
				  x2, y1, 1 + dist, 0 + dist, c,
  				  x1, y2, 0 + dist, 1 + dist, c,
				  x2, y2, 1 + dist, 1 + dist, c);
		sd->Stop();
	}
}

void Noise02::DeInit() 
{
	delete [] noise;
}


void FlashWhite::Do(dword timer) {
//	uniSetTexture(NULL);
//	uniSetTexture(NULL, 1);
	uniSetTexture("null");
	uniSetTexture("null", 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0xffffff;
	int a = 0xff - (timer / 5);
	if (a >= 0) {
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0, 0, c,
				  x2, y1, 0, 0, c,
  				  x1, y2, 0, 0, c,
				  x2, y2, 0, 0, c);
		sd->Stop();
	}
}

void FlashBlack::Do(dword timer) {
//	uniSetTexture(NULL);
//	uniSetTexture(NULL, 1);
	uniSetTexture("null");
	uniSetTexture("null", 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0x0;
	int a = 0xff - (timer / 7);
	if (a >= 0) {
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0, 0, c,
				  x2, y1, 0, 0, c,
  				  x1, y2, 0, 0, c,
				  x2, y2, 0, 0, c);
		sd->Stop();
	}
}

void FadeWhite::Do(dword timer) {
//	uniSetTexture(NULL);
//	uniSetTexture(NULL, 1);
	uniSetTexture("null");
	uniSetTexture("null", 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0xffffff;
	int a = 0x0 + (timer / 7);
	if (a > 255)
		a = 255;
	c += a << 24;

	sd->Start(UNI_ALPHA);
	sd->Draw2D(x1, y1, 0, 0, c,
			  x2, y1, 0, 0, c,
			  x1, y2, 0, 0, c,
			  x2, y2, 0, 0, c);
	sd->Stop();
}


void FadeBlack::Do(dword timer) {
//	uniSetTexture(NULL);
//	uniSetTexture(NULL, 1);
	uniSetTexture("null");
	uniSetTexture("null", 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0x0;
	int a = 0x0 + (timer / 7);
	if (a > 255) 
		a = 255;
	c += a << 24;

	sd->Start(UNI_ALPHA);
	sd->Draw2D(x1, y1, 0, 0, c,
			  x2, y1, 0, 0, c,
			  x1, y2, 0, 0, c,
			  x2, y2, 0, 0, c);
	sd->Stop();
}

bool FlashWhiteFast::Init()
{
	mode = 1;
	return true;
}

void FlashWhiteFast::Do(dword timer) {
//	uniSetTexture(NULL);
//	uniSetTexture(NULL, 1);
	uniSetTexture("null");
	uniSetTexture("null", 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0xffffff;
	int a = 0xff - (timer / 2);
	if (mode == 2)
	{
		a = 0x77 - (timer / 1);
	}
	if (a >= 0) {
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0, 0, c,
				  x2, y1, 0, 0, c,
  				  x1, y2, 0, 0, c,
				  x2, y2, 0, 0, c);
		sd->Stop();
	}
}

void FlashWhiteFast::Receive(dword message, dword timer) 
{
	mode = message & 255; 
}

void FlashBlackFast::Do(dword timer) {
//	uniSetTexture(NULL);
//	uniSetTexture(NULL, 1);
	uniSetTexture("null");
	uniSetTexture("null", 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0x0;
	int a = 0xff - (timer / 1.5);
	if (a >= 0) {
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0, 0, c,
				  x2, y1, 0, 0, c,
  				  x1, y2, 0, 0, c,
				  x2, y2, 0, 0, c);
		sd->Stop();
	}
}

void FadeWhiteFast::Do(dword timer) {
//	uniSetTexture(NULL);
//	uniSetTexture(NULL, 1);
	uniSetTexture("null");
	uniSetTexture("null", 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0xffffff;
	int a = 0x0 + (timer / 2);
	if (a > 255)
		a = 255;
	c += a << 24;

	sd->Start(UNI_ALPHA);
	sd->Draw2D(x1, y1, 0, 0, c,
			  x2, y1, 0, 0, c,
			  x1, y2, 0, 0, c,
			  x2, y2, 0, 0, c);
	sd->Stop();
}

void FadeBlackFast::Do(dword timer) {
//	uniSetTexture(NULL);
//	uniSetTexture(NULL, 1);
	uniSetTexture("null");
	uniSetTexture("null", 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0x0;
	int a = 0x0 + (timer / 1.5);
	if (a > 255) 
		a = 255;
	c += a << 24;

	sd->Start(UNI_ALPHA);
	sd->Draw2D(x1, y1, 0, 0, c,
			  x2, y1, 0, 0, c,
			  x1, y2, 0, 0, c,
			  x2, y2, 0, 0, c);
	sd->Stop();
}

 

// ATS CRAP

#define ATS_FILLALPHA

bool TGALoadAndSave = false;
//char *TGAFilename = "texmap000.tga";
char TGAFilename[] = {'t', 'e', 'x', 'm', 'a', 'p', '0', '0', '0', '.', 't', 'g', 'a', '\0'};


class MemFileSys
{
public:
	MemFileSys(byte *pData)
	{
		data = pData;
		pos  = 0;
	}
	~MemFileSys()
	{
	}

	void Read(void *target, int size)
	{
		memcpy(target, data + pos, size);
		pos += size;
	}

	int GetOffset()
	{
		return pos;
	}

	void SetOffset(int iPos)
	{
		pos = iPos;
	}

private:	
	byte *data;
	int pos;
};

#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>

dword *loadTGAFromFile(char *filename, unsigned short &width, unsigned short &height)
{
    int handle;

    if ((handle = open(filename, O_RDONLY | O_BINARY, S_IREAD)) == -1)
    {
		return NULL;
    }

    unsigned short bpp;

    char h[12];

    read(handle, h, 12);

    read(handle, &width, 2);
    read(handle, &height, 2);
    read(handle, &bpp, 2);

	dword *target = new dword[width * height];

	unsigned char *tempmap = new unsigned char[width * height * 3];
	read(handle, tempmap, width * height * 3);

    int index = 0;
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
			unsigned char b = tempmap[index++];
			unsigned char g = tempmap[index++];
			unsigned char r = tempmap[index++];
            target[x + (y * width)] = ((int)r << 16) | ((int)g << 8) | (int)b;
        }
    }

    close(handle);

	delete [] tempmap;

#ifdef ATS_FILLALPHA
	for (int i = 0; i < width * height; i++)
	{
		target[i] |= 0xff << 24;
	}
#endif

	return target;
}

void saveTGAToFile(char *filename, dword *source, unsigned short width, unsigned short height)
{
    int handle;

    if ((handle = open(filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IWRITE)) == -1)
    {
        uniQuit("Error saving TGA");
    }

    unsigned short bpp = 24;

    char h[] = "\0\0\2\0\0\0\0\0\0\0\0\0";

    write(handle, h, 12);

    write(handle, &width, 2);
    write(handle, &height, 2);
    write(handle, &bpp, 2);

    char *tempmap = new char[width * height * 3];
    int index = 0;
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            int col = source[x + (y * width)];
            char r = (col >> 16) & 255;
            char g = (col >> 8) & 255;
            char b = (col) & 255;
            tempmap[index++] = b;
            tempmap[index++] = g;
            tempmap[index++] = r;
        }
    }

    write(handle, tempmap, width * height * 3);

    close(handle);

	delete [] tempmap;
}

dword *loadATSFromMem(byte *atsMem, int &width, int &height)
{
	unsigned short nrFilters;
	unsigned short textureWidth;
	unsigned short textureHeight;

	if (TGALoadAndSave)
	{
		dword *target = loadTGAFromFile(TGAFilename, textureWidth, textureHeight);

		if (target != NULL)
		{
			TGAFilename[8]++;
			if (TGAFilename[8] == ':') 
			{
				TGAFilename[8] = '0';
				TGAFilename[7]++;
			}

			width  = textureWidth;
			height = textureHeight;

			return target;
		}
	}

	MemFileSys fileSys(atsMem);

    char header[5] = {'0','0','0','0','\0'};

	fileSys.Read(header, 4);
    
    if (strcmp(header, TEXGEN_HEADER1) == 0)
    {
		uniQuit("Sorry, cant read this old ATS file");
    }
    else
    if (strcmp(header, TEXGEN_HEADER2) != 0)
    {
		uniQuit("Not a valid Aardbei Texture Studio file");
        return NULL;
    }


    fileSys.Read(&nrFilters, 2);

    fileSys.Read(&textureWidth, 2);
    fileSys.Read(&textureHeight, 2);

    TextureGenerator *generator = new TextureGenerator(textureWidth, textureHeight);

	generator->AddLayer();

    for (int i = 0; i < nrFilters; i++)
    {
        unsigned char command;
        long offset = fileSys.GetOffset();
		fileSys.Read(&command, 1);
		fileSys.SetOffset(offset);
        if (command == TEXGEN_NEWLAYER)
        {
            Filter *filter = new Filter();
            fileSys.Read(filter, sizeof(Filter));
			generator->AddLayer();
        }
        if (command == TEXGEN_CHANGELAYER)
        {
            FilterChangeLayer *filter = new FilterChangeLayer();
            fileSys.Read(filter, sizeof(FilterChangeLayer));
			generator->layerActive = generator->layers[filter->layerNr];
        }
        if (command == TEXGEN_COPY)
        {
            FilterCopy *filter = new FilterCopy();
            fileSys.Read(filter, sizeof(FilterCopy));
            generator->FilterCopy(generator->layerActive,
                                  generator->layers[filter->sourceNr],
                                  filter->channel);
        }
        if (command == TEXGEN_ADD)
        {
            FilterAdd *filter = new FilterAdd();
            fileSys.Read(filter, sizeof(FilterAdd));
            generator->FilterAdd(generator->layerActive,
                                 generator->layers[filter->sourceNr],
                                 filter->channel);
        }
        if (command == TEXGEN_SUB)
        {
            FilterSub *filter = new FilterSub();
            fileSys.Read(filter, sizeof(FilterSub));
            generator->FilterSub(generator->layerActive,
                                 generator->layers[filter->sourceNr],
                                 filter->channel);
        }
        if (command == TEXGEN_MUL)
        {
            FilterMul *filter = new FilterMul();
            fileSys.Read(filter, sizeof(FilterMul));
            generator->FilterMul(generator->layerActive,
                                 generator->layers[filter->sourceNr],
                                 filter->channel);
        }
        if (command == TEXGEN_NOTNULLCOPY)
        {
            FilterNotNull *filter = new FilterNotNull();
            fileSys.Read(filter, sizeof(FilterNotNull));
            generator->FilterNotNullCopy(generator->layerActive,
                                         generator->layers[filter->sourceNr],
                                         filter->channel);
        }
        if (command == TEXGEN_MIX)
        {
            FilterMix *filter = new FilterMix();
            fileSys.Read(filter, sizeof(FilterMix));
            generator->FilterMix(generator->layerActive,
                                 generator->layers[filter->sourceNr],
                                 filter->alpha,
                                 filter->channel);
        }
        if (command == TEXGEN_SHADE)
        {
            FilterShade *filter = new FilterShade();
            fileSys.Read(filter, sizeof(FilterShade));
            generator->FilterShade(generator->layerActive,
                                   generator->layers[filter->sourceNr],
                                   filter->lightning,
                                   filter->channel);
        }
        if (command == TEXGEN_PLASMA)
        {
            FilterPlasma *filter = new FilterPlasma();
            fileSys.Read(filter, sizeof(FilterPlasma));
            generator->FilterPlasma(filter->aantalX,
                                    filter->aantalY,
                                    filter->shiftX,
                                    filter->shiftY,
                                    filter->channel,
                                    filter->method);
        }
        if (command == TEXGEN_FRACTALPLASMA)
        {
            FilterFractalPlasma *filter = new FilterFractalPlasma();
            fileSys.Read(filter, sizeof(FilterFractalPlasma));
            generator->FilterFractalPlasma(filter->factor,
                                           filter->seed,
                                           filter->channel,
                                           filter->method);
        }
        if (command == TEXGEN_CELLS)
        {
            FilterCells *filter = new FilterCells();
            fileSys.Read(filter, sizeof(FilterCells));
            generator->FilterCells(filter->density,
                                   filter->strength,
                                   filter->seed,
                                   filter->channel,
                                   filter->method);
        }
        if (command == TEXGEN_TEXT)
        {
            FilterText *filter = new FilterText();
            fileSys.Read(filter, sizeof(FilterText));
            generator->FilterText(filter->text,
                                  filter->font,
                                  filter->color,
                                  filter->size,
                                  filter->x,
                                  filter->y,
                                  filter->aa,
                                  filter->style,
                                  filter->clip,
                                  filter->method);
        }
        if (command == TEXGEN_BRUSH)
        {
            FilterBrush *filter = new FilterBrush();
            fileSys.Read(filter, sizeof(FilterBrush));
            generator->FilterBrush(filter->x,
                                    filter->y,
                                    filter->size,
                                    filter->density,
                                    filter->hardness,
                                    filter->opacity,
                                    filter->color);
        }
        if (command == TEXGEN_NOISE)
        {
            FilterNoise *filter = new FilterNoise();
            fileSys.Read(filter, sizeof(FilterNoise));
            generator->FilterNoise(filter->strength,
                                   filter->uniform,
                                   filter->multi,
                                   filter->seed,
                                   filter->channel,
                                   filter->method);
        }
        if (command == TEXGEN_RECTANGLE)
        {
            FilterRectangle *filter = new FilterRectangle();
            fileSys.Read(filter, sizeof(FilterRectangle));
            generator->FilterRectangle(filter->x1,
                                       filter->y1,
                                       filter->x2,
                                       filter->y2,
                                       filter->color,
                                       filter->method);
        }
        if (command == TEXGEN_GAUSSIANBLUR)
        {
            FilterGaussianBlur *filter = new FilterGaussianBlur();
            fileSys.Read(filter, sizeof(FilterGaussianBlur));
            generator->FilterGaussianBlur(filter->radius);
        }
        if (command == TEXGEN_MOTIONBLUR)
        {
            FilterMotionBlur *filter = new FilterMotionBlur();
            fileSys.Read(filter, sizeof(FilterMotionBlur));
            generator->FilterMotionBlur(filter->angle,
                                        filter->radius);
        }
        if (command == TEXGEN_DIRBLUR)
        {
            FilterDirBlur *filter = new FilterDirBlur();
            fileSys.Read(filter, sizeof(FilterDirBlur));
            generator->FilterDirBlur(generator->layers[filter->sourceNr],
                                     filter->aantal,
                                     filter->channel);
        }
        if (command == TEXGEN_BOXBLUR)
        {
            Filter *filter = new Filter();
            fileSys.Read(filter, sizeof(Filter));
            generator->FilterBoxBlur();
        }
        if (command == TEXGEN_EMBOSS)
        {
            Filter *filter = new Filter();
            fileSys.Read(filter, sizeof(Filter));
            generator->FilterEmboss();
        }
        if (command == TEXGEN_SOBEL)
        {
            Filter *filter = new Filter();
            fileSys.Read(filter, sizeof(Filter));
            generator->FilterSobel();
        }
        if (command == TEXGEN_EDGE)
        {
            Filter *filter = new Filter();
            fileSys.Read(filter, sizeof(Filter));
            generator->FilterEdge();
        }
        if (command == TEXGEN_TOPLEFT)
        {
            Filter *filter = new Filter();
            fileSys.Read(filter, sizeof(Filter));
            generator->FilterTopLeft();
        }
        if (command == TEXGEN_HSV)
        {
            FilterHsv *filter = new FilterHsv();
            fileSys.Read(filter, sizeof(FilterHsv));
            generator->FilterHsv(filter->rotation,
                                 filter->saternation);
        }
        if (command == TEXGEN_INVERT)
        {
            Filter *filter = new Filter();
            fileSys.Read(filter, sizeof(Filter));
            generator->FilterInvert();
        }
        if (command == TEXGEN_CONTRAST)
        {
            FilterContrast *filter = new FilterContrast();
            fileSys.Read(filter, sizeof(FilterContrast));
            generator->FilterContrast(filter->strength);
        }
        if (command == TEXGEN_BRIGHTNESS)
        {
            FilterBrightness *filter = new FilterBrightness();
            fileSys.Read(filter, sizeof(FilterBrightness));
            generator->FilterBrightness(filter->strength);
        }
        if (command == TEXGEN_SINEDISTORT)
        {
            FilterSineDistort *filter = new FilterSineDistort();
            fileSys.Read(filter, sizeof(FilterSineDistort));
            generator->FilterSineDistort(filter->nrX,
                                         filter->nrY,
                                         filter->ampX,
                                         filter->ampY,
                                         filter->shiftX,
                                         filter->shiftY);
        }
        if (command == TEXGEN_TILE)
        {
            FilterTile *filter = new FilterTile();
            fileSys.Read(filter, sizeof(FilterTile));
            generator->FilterTile(filter->repX,
                                  filter->repY);
        }
    }

	dword *target = new dword[textureWidth * textureHeight];

	memcpy(target, generator->layerActive, textureWidth * textureHeight * 4);

	width  = textureWidth;
	height = textureHeight;

#ifdef ATS_FILLALPHA
	for (i = 0; i < width * height; i++)
	{
		target[i] |= 0xff << 24;
	}
#endif

	if (TGALoadAndSave)
	{
		saveTGAToFile(TGAFilename, target, width, height);

		TGAFilename[8]++;
		if (TGAFilename[8] == ':') 
		{
			TGAFilename[8] = '0';
			TGAFilename[7]++;
		}
	}

	delete generator;

	return target;
}