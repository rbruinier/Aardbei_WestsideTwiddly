/*

	Pyramid DX8 System - System Code (header file)


	Remarks:
		-


	Updates:
		- 20-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

class SpriteDrawer;

class UniSystem {
public:

	dword				reqVideoBpp;			// aantal bits per pixel voor video gevraagd
	dword				reqTextureBpp;			// aantal bits per pixel voor textures gevraagd
	dword				usedVideoBpp;			// aantal bits per pixel voor video gebruikt
	dword				usedTextureBpp;			// aantal bits per pixel voor textures gebruikt

	dword				reqWidth;				// gevraagde resolutie
	dword				reqHeight;				// gevraagde resolutie
	dword				usedWidth;				// gebruikte resolutie
	dword				usedHeight;				// gebruikte resolutie

	dword				reqFlags;				// gevraagde flags

	bool				windowed;
	bool				log;
	bool				hwTnL;
	bool				waitretrace;

	dword				maxTextureWidth;
	dword				maxTextureHeight;

	IDirect3D8			*d3d8;
	IDirect3DDevice8	*d3d8Device;

	IDirect3DSurface8	*surfaceBackBuffer;
	IDirect3DSurface8	*surfaceDepthBuffer;

	D3DPRESENT_PARAMETERS d3d8PP;

	D3DFORMAT			textureFormat;			// gebruikte format voor textures

	HWND				windowHandle;
	HINSTANCE			hInstance;
	char				*params;

	UniAdapter			*uniAdapter;			// alle info over gebruikte video adapter

	UniDisplayMode		*displayMode;			// gebruikte video mode

	dword				oldTimer;

	dword				viewportX1;
	dword				viewportY1;
	dword				viewportX2;
	dword				viewportY2;

	SpriteDrawer		*spriteDrawer;

	bool				pause;					// hebben we de focus?

	bool timerHPSupported;

	DWORD timerLPStart;
	LARGE_INTEGER timerHPCounter;
	LARGE_INTEGER timerHPFrequency;
	LARGE_INTEGER timerHPStart;


public:
	UniSystem(HINSTANCE hInstance, char * params);
	~UniSystem();


public:
	void initParams(dword setup, dword bit, dword tbit);
	void initWindow(dword width, dword height);
	void initDirect3D(dword adapter);

public:
	void deinitDirect3D();

public:
	void restoreSettings();
	void restoreDirect3D();

public:
	void initDemo();
	void renderDemo();
	void exitDemo();
};


inline DWORD uniFtoDW( FLOAT f ) { return *((DWORD*)&f); }

long uniTimer();
void uniTimerReset();

float uniRand();

void uniSetMaterial(D3DMATERIAL8 & mtrl);

void uniSetLightState(int index, bool on);
void uniSetLightInfo(int index, D3DLIGHT8 &light);

bool uniStart();
bool uniStop();

void uniSetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD state);
void uniSetRenderState(D3DRENDERSTATETYPE type, DWORD state);
void uniSetTexture(char * name, int stage = 0);

void uniSetTransformView(D3DMATRIX & matrix);
void uniSetTransformWorld(D3DMATRIX & matrix);
void uniSetTransformProjection(D3DMATRIX & matrix);

void uniSetViewport(int x1, int y1, int x2, int y2);

void uniClearBuffers(dword flags, int color = 0);

void uniShowBuffers();

void uniRenderTargetReset();

void uniInitSystem(int width, int height, int bit, int tbit, dword setup);
void uniInitSystem(int adapter, int width, int height, int bit, int tbit, dword setup);

void uniQuit();
void uniQuit(char * message);

extern UniSystem * uniSystem;

extern void mainInit();
extern void mainRender(dword timer);
extern void mainExit();


#endif
