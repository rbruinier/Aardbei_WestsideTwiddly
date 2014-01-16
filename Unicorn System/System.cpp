/*

	Pyramid DX8 System - System Code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "unicorn.h"

//#define UNI_SHOW_FPS

long CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef UNI_FULL_DEBUG	
	#include <crtdbg.h> 
	#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif

#ifdef  UNI_SHOW_FPS

float fpsCounter = 0;
float fpsTimer   = 0;
int fpsFPS = 0;

#endif


UniSystem * uniSystem;
/*
long uniTimer() {
	return timeGetTime() - uniSystem->oldTimer;
}

void uniTimerReset() {
	uniSystem->oldTimer = timeGetTime();
}
*/
void uniTimerReset()
{
	if (QueryPerformanceCounter(&uniSystem->timerHPStart))
	{
		uniSystem->timerHPSupported = true;

		QueryPerformanceFrequency(&uniSystem->timerHPFrequency);
	}
	else
	{
		uniSystem->timerHPSupported = false;

		uniSystem->timerLPStart = timeGetTime();
	}
}

long uniTimer()
{
	if (uniSystem->timerHPSupported)
	{
		QueryPerformanceCounter(&uniSystem->timerHPCounter);

		return ((uniSystem->timerHPCounter.QuadPart - uniSystem->timerHPStart.QuadPart) / (float)uniSystem->timerHPFrequency.QuadPart) * 1000.f;
	}
	else
	{
		return timeGetTime() - uniSystem->timerLPStart;
	}
}

float uniRand() {
	return (rand() / (float)RAND_MAX);
}

void uniSetMaterial(D3DMATERIAL8 & mtrl) {
    uniSystem->d3d8Device->SetMaterial(&mtrl);
}

void uniSetLightState(int index, bool on) {
	uniSystem->d3d8Device->LightEnable(index, on);
}

void uniSetLightInfo(int index, D3DLIGHT8 &light) {
	uniSystem->d3d8Device->SetLight(index, &light);
}

bool uniStart() {
	if (uniSystem->d3d8Device->BeginScene() != D3D_OK)
		return false;
	else
		return true;
}

bool uniStop() {
    if (uniSystem->d3d8Device->EndScene() != D3D_OK)
		return false;
	else
		return true;
}

void uniSetTexture(char * name, int stage) {
	if (name == NULL)
		uniSystem->d3d8Device->SetTexture(stage, NULL);
	else 
		uniSystem->d3d8Device->SetTexture(stage, uniTextureGet(name));
} 

void uniSetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD state) 
{
	uniSystem->d3d8Device->SetTextureStageState(stage, type, state);
}

void uniSetRenderState(D3DRENDERSTATETYPE type, DWORD state) 
{
	uniSystem->d3d8Device->SetRenderState(type, state);
}

void uniSetTransformView(D3DMATRIX & matrix) 
{
	uniSystem->d3d8Device->SetTransform(D3DTS_VIEW, &matrix);
}

void uniSetTransformWorld(D3DMATRIX & matrix) 
{
	uniSystem->d3d8Device->SetTransform(D3DTS_WORLD, &matrix);
}

void uniSetTransformProjection(D3DMATRIX & matrix) 
{
	uniSystem->d3d8Device->SetTransform(D3DTS_PROJECTION, &matrix);
}

void uniClearBuffers(dword flags, int color) 
{
	dword params = 0;
	
	if (flags & UNI_ZBF)
		params |= D3DCLEAR_ZBUFFER;
	if (flags & UNI_BBF)
		params |= D3DCLEAR_TARGET;

	if (D3D_OK != (uniSystem->d3d8Device->Clear(0, NULL, params, color, 1.0f, 0))) {
		uniQuit("Failed to clear backbuffer");
	}
}

void UniSystem::restoreSettings() 
{
	for (int i = 0; i < 8; i++) {
		uniSetTextureStageState(i, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		uniSetTextureStageState(i, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		uniSetTextureStageState(i, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
	}

	uniSetRenderState(D3DRS_FILLMODE,		D3DFILL_SOLID);
	uniSetRenderState(D3DRS_SHADEMODE,		D3DSHADE_GOURAUD);
	uniSetRenderState(D3DRS_LASTPIXEL,		true);

	uniSetRenderState(D3DRS_ZENABLE,		true);
	uniSetRenderState(D3DRS_ZFUNC,			D3DCMP_LESSEQUAL);
	uniSetRenderState(D3DRS_ZWRITEENABLE,	true);

	uniSetRenderState(D3DRS_CLIPPING,		true);
	uniSetRenderState(D3DRS_CULLMODE,		D3DCULL_CW);
	uniSetRenderState(D3DRS_LIGHTING,		true);
	uniSetRenderState(D3DRS_COLORVERTEX,	true);

	uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
	uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void UniSystem::restoreDirect3D() 
{
	if (surfaceBackBuffer != NULL) 
		surfaceBackBuffer->Release();

	if (surfaceDepthBuffer != NULL) 
		surfaceDepthBuffer->Release();

	surfaceBackBuffer  = NULL;
	surfaceDepthBuffer = NULL;

	uniVBufferReleaseDynamics();
	uniIBufferReleaseDynamics();
	uniTextureReleaseDynamics();

	d3d8Device->Reset(&d3d8PP);

	if (D3D_OK != d3d8Device->TestCooperativeLevel())
		uniQuit("Unable to recover");

	d3d8Device->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &surfaceBackBuffer);
	d3d8Device->GetDepthStencilSurface(&surfaceDepthBuffer);

	uniVBufferRecreateDynamics();
	uniIBufferRecreateDynamics();
	uniTextureRecreateDynamics();

	restoreSettings();

	//uniSetViewport(viewportX1, viewportY1, viewportX2, viewportY2);
}

void uniRenderTargetReset() 
{
	if (D3D_OK != uniSystem->d3d8Device->SetRenderTarget(uniSystem->surfaceBackBuffer, uniSystem->surfaceDepthBuffer))
		uniQuit("Failed to reset render target");
	uniSetViewport(uniSystem->viewportX1, uniSystem->viewportY1, 
		           uniSystem->viewportX2, uniSystem->viewportY2);

}

void uniSetViewport(int x1, int y1, int x2, int y2) 
{
	D3DVIEWPORT8 viewData = {x1, y1, x2 - x1, y2 - y1, 0.0f, 1.0f };
	uniSystem->d3d8Device->SetViewport(&viewData);
	uniSystem->viewportX1 = x1;
	uniSystem->viewportY1 = y1;
	uniSystem->viewportX2 = x2;
	uniSystem->viewportY2 = y2;
}

//#include <stdio.h>

void uniShowBuffers() 
{

	if (D3DERR_DEVICELOST == (uniSystem->d3d8Device->Present(NULL, NULL, NULL, NULL))) {
		HRESULT hr = uniSystem->d3d8Device->TestCooperativeLevel();
		if (hr == D3DERR_DEVICENOTRESET) {
			uniSystem->restoreDirect3D();
			uniSystem->pause = false;
		} else
			uniSystem->pause = true;
	}
/*
	// script info laten zien
	HDC hDC = GetDC(uniSystem->windowHandle);

    SetTextColor(hDC, RGB(255, 255, 255) );
	SetBkColor(hDC, RGB(0, 0, 0));

	char buffer[128];

	sprintf(buffer, "                     ");
    ExtTextOut(hDC, 1,  1, 0, NULL, buffer, lstrlen(buffer), NULL);
    ExtTextOut(hDC, 1,  15, 0, NULL, buffer, lstrlen(buffer), NULL);
	ExtTextOut(hDC, 1,  29, 0, NULL, buffer, lstrlen(buffer), NULL);
	ExtTextOut(hDC, 1,  43, 0, NULL, buffer, lstrlen(buffer), NULL);
	ExtTextOut(hDC, 1,  57, 0, NULL, buffer, lstrlen(buffer), NULL);


	int pos, row, sync;

	fmodGetInfo(pos, row, sync);

	float usage = 0;

	if (pos < 10)
		sprintf(buffer, "pos  : 0%i", pos);
	else
		sprintf(buffer, "pos  : %i", pos);
    ExtTextOut(hDC, 1,  1, 0, NULL, buffer, lstrlen(buffer), NULL);

	if (row < 10)
		sprintf(buffer, "row  : 0%i", row);
	else
		sprintf(buffer, "row  : %i", row);
    ExtTextOut(hDC, 1,  15, 0, NULL, buffer, lstrlen(buffer), NULL);

	sprintf(buffer, "time : %i", sync);
    ExtTextOut(hDC, 1,  29, 0, NULL, buffer, lstrlen(buffer), NULL);

	sprintf(buffer, "fps  : %i", fpsFPS);
    ExtTextOut(hDC, 1,  43, 0, NULL, buffer, lstrlen(buffer), NULL);

	sprintf(buffer, "cpu  : %f", usage);
    ExtTextOut(hDC, 1,  57, 0, NULL, buffer, lstrlen(buffer), NULL);

	ReleaseDC(uniSystem->windowHandle, hDC);
*/

}


void uniQuit(char * message) {
#ifdef UNI_LOGHTML
	uniLog("<font color='#ff0000'>   ERROR: %s</font>\n", message);
#endif
	ShowCursor(1);
	delete uniSystem;
#ifdef UNI_LOGHTML
	uniStopLog();
#endif
	MessageBox(NULL, message, "Fatal error", MB_OK | MB_TOPMOST);
	_exit(1);
}

void uniQuit() {
	delete uniSystem;
#ifdef UNI_LOGHTML
	uniStopLog();
#endif
	_exit(1);
}


void uniInitSystem(int adapter, int width, int height, int bit, int tbit, dword setup) {
	uniSystem->initParams(setup, bit, tbit);

#ifdef UNI_LOGHTML
	uniStartLog();
#endif

	uniSystem->initWindow(width, height);

#ifdef UNI_LOGHTML
	uniLog("\n<b>Initialising DirectX 8</b>\n");
	uniLog("   Searching for DirectX 8\n");
#endif

	uniSystem->initDirect3D(adapter);

#ifdef UNI_LOGHTML
	uniLog("<b>DirectX 8 initialised</b>\n");

	uniLog("\n<b>Initialising demo</b>\n");
#endif
}

void uniInitSystem(int width, int height, int bit, int tbit, dword setup) {
	uniInitSystem(D3DADAPTER_DEFAULT, width, height, bit, tbit, setup);
}

UniSystem::UniSystem(HINSTANCE hInstance, char * params) {
	this->hInstance = hInstance;
	this->params    = params;

	surfaceBackBuffer  = NULL;
	surfaceDepthBuffer = NULL;
	d3d8Device		   = NULL;
	d3d8			   = NULL;
	uniAdapter		   = NULL;

	pause = false;

	WNDCLASS wc;
	memset(&wc, 0, sizeof (wc));
	wc.style         = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wc.lpszClassName = "Unicorn3DSystem";
	RegisterClass( &wc );
}

UniSystem::~UniSystem() {
	deinitDirect3D();

	if (uniAdapter)
		delete uniAdapter;

	UnregisterClass("Unicorn3DSystem", hInstance);
}

void UniSystem::initParams(dword setup, dword bit, dword tbit) {
	reqFlags = setup;

	windowed = log = waitretrace = hwTnL = false;

	if (setup & UNI_WIN)
		windowed = true;

	if (setup & UNI_LOG)
		log = true;

	if (setup & UNI_WVR)
		waitretrace = true;

	if (setup & UNI_TNL)
		hwTnL = true;

	reqVideoBpp   = bit;
	reqTextureBpp = tbit;

	if (strstr(params, "v16b")) {
		reqVideoBpp = 16;
	} else
	if (strstr(params, "v32b")) {
		reqVideoBpp  = 32;
	}

	if (strstr(params, "t16b")) {
		reqTextureBpp = 16;
	} else
	if (strstr(params, "t32b")) {
		reqTextureBpp = 32;
	}

	if (strstr(params, "window")) {
		windowed = true;
	} else
	if (strstr(params, "fullscreen")) {
		windowed = false;
	}

	if (strstr(params, "notnl")) {
		hwTnL = false;
	} else
	if (strstr(params, "tnl")) {
		hwTnL = true;
	}

	if (strstr(params, "waitretrace")) {
		waitretrace = true;
	}

	if (strstr(params, "log")) {
		log = true;
	}

}

void UniSystem::initWindow(dword width, dword height) {
	this->reqWidth  = width;
	this->reqHeight = height;

	long winParam = WS_POPUP;

	if (windowed)
#ifdef UNI_RESIZABLE_WINDOW
		winParam |= WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_VISIBLE;
#else
		winParam |= WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
#endif

#ifdef UNI_LOGHTML
	uniLog("<b>Creating window (%ix%i)</b>\n", reqWidth, reqHeight);
#endif

	int winWidth  = reqWidth;
	int winHeight = reqHeight;


	if (windowed) {
#ifdef UNI_RESIZABLE_WINDOW
		winWidth  += 8;
		winHeight += 27;
#else
		winWidth  += 6;
		winHeight += 25;
#endif
	}

	windowHandle = CreateWindowEx(0,
							      "Unicorn3DSystem",
								  UNI_TITLE,
								  winParam,
								  0,
								  0,
								  winWidth,
								  winHeight,
								  0,
								  0,
								  hInstance,
								  0);

	if (!windowHandle)
		uniQuit("Unable to create window");
			
	if (!windowed)
		ShowCursor(false);
    
	ShowWindow(windowHandle, SW_SHOW);
	UpdateWindow(windowHandle);
}

void UniSystem::initDirect3D(dword adapter) {
	if (NULL == (d3d8 = Direct3DCreate8(D3D_SDK_VERSION))) {
		uniQuit("Please install DirectX 8.0 or better");
	}

#ifdef UNI_LOGHTML
	uniLog("   Retrieving information about display adapter(s)\n");
#endif
	
	uniAdapter = new UniAdapter(adapter, windowed);

	ZeroMemory(&d3d8PP, sizeof(d3d8PP));
	
	if (windowed) 
	{
		displayMode = &uniAdapter[adapter].videoMode[0];
		d3d8PP.Windowed                        = TRUE;
		d3d8PP.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		d3d8PP.BackBufferFormat                = displayMode->videoMode.Format;
		d3d8PP.FullScreen_RefreshRateInHz      = 0;
	} else 
	{
		displayMode = uniAdapter[adapter].findVideoMode(reqWidth, reqHeight, reqVideoBpp);
		if (displayMode == NULL)
			uniQuit("Unable to find correct video mode");
		d3d8PP.Windowed                        = FALSE;
		d3d8PP.BackBufferWidth				   = reqWidth;
		d3d8PP.BackBufferHeight				   = reqHeight;
		d3d8PP.BackBufferFormat                = displayMode->bbufferFormat;
		if (waitretrace) 
			d3d8PP.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		else
			d3d8PP.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3d8PP.FullScreen_RefreshRateInHz      = D3DPRESENT_RATE_DEFAULT;
	}

	d3d8PP.EnableAutoDepthStencil          = TRUE;    
	d3d8PP.AutoDepthStencilFormat          = displayMode->zbufferFormat;
	d3d8PP.BackBufferCount                 = 1;
	d3d8PP.SwapEffect                      = D3DSWAPEFFECT_DISCARD;
	d3d8PP.MultiSampleType                 = D3DMULTISAMPLE_NONE;

	if (reqTextureBpp == 32)
		if (displayMode->textureFormat32 != D3DFMT_UNKNOWN)
			textureFormat = displayMode->textureFormat32;
		else
			textureFormat = displayMode->textureFormat16;
	else
		if (displayMode->textureFormat16 != D3DFMT_UNKNOWN)
			textureFormat = displayMode->textureFormat16;
		else
			textureFormat = displayMode->textureFormat32;	

	dword behavior = 0;
	if (uniAdapter->hwTnL && (hwTnL)) 
	{
//		if (uniAdapter->caps.DevCaps & D3DDEVCAPS_PUREDEVICE)
//		{
//			behavior |= D3DCREATE_HARDWARE_VERTEXPROCESSING |
//                                        D3DCREATE_PUREDEVICE;
//		}
//		else
//		{
			behavior |= D3DCREATE_MIXED_VERTEXPROCESSING;
//		}
	} else 
	{
		behavior |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	usedTextureBpp = uniGetBppFromFormat(textureFormat);
	usedVideoBpp   = uniGetBppFromFormat(displayMode->bbufferFormat);
	usedWidth	   = reqWidth;
	usedHeight	   = reqHeight;

#ifdef UNI_LOGHTML
	uniLog("   Creating Direct3D 8 Device, using %s\n", uniAdapter->name);	
	uniLog("   Used formats: - Backbuffer : %d\n", displayMode->bbufferFormat);
	uniLog("                 - ZBuffer    : %d\n", displayMode->zbufferFormat);
	uniLog("                 - Textures   : %d\n", textureFormat);
	uniLog("   Requested %ix%ix%ix%i, got %ix%ix%ix%i\n", 
		   reqWidth, reqHeight, reqVideoBpp, reqTextureBpp,
		   usedWidth, usedHeight, usedVideoBpp, usedTextureBpp);
#endif
	
	if (D3D_OK != (d3d8->CreateDevice(adapter, D3DDEVTYPE_HAL, windowHandle,
				   behavior, &d3d8PP, &d3d8Device)))
		uniQuit("UNI Says: Problems creating Direct3D 8 Device");

	
	d3d8Device->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &surfaceBackBuffer);
	d3d8Device->GetDepthStencilSurface(&surfaceDepthBuffer);

	maxTextureWidth  = uniAdapter->texMaxWidth;
	maxTextureHeight = uniAdapter->texMaxHeight;

	uniSetViewport(0, 0, reqWidth, reqHeight);
	uniSystem->restoreSettings();
	uniClearBuffers(UNI_BBF, 0);
	uniShowBuffers();
	uniClearBuffers(UNI_BBF, 0);
	uniShowBuffers();
	uniClearBuffers(UNI_BBF, 0);

	spriteDrawer = new SpriteDrawer();
}

void UniSystem::deinitDirect3D() {
#ifdef UNI_LOGHTML
	uniLog("\n<b>Destroying system</b>\n");
#endif

	if (surfaceBackBuffer != NULL)
		surfaceBackBuffer->Release();
	if (surfaceDepthBuffer != NULL)
		surfaceDepthBuffer->Release();


#ifdef UNI_LOGHTML
	uniLog("   Destroying textures\n");
#endif
	uniTextureDestroyAll();
#ifdef UNI_BITMAP_SUPPORT
	uniBitmapDestroyAll();
#endif

#ifdef UNI_LOGHTML
	uniLog("   Destroying vertex buffers\n");
#endif
	uniVBufferDestroy();

#ifdef UNI_LOGHTML
	uniLog("   Destroying index buffers\n");
#endif
	uniIBufferDestroy();

#ifdef UNI_LOGHTML
	uniLog("   Releasing DirectX 8\n");
#endif

	if (d3d8Device != NULL)
		d3d8Device->Release();
	if (d3d8 != NULL)
		d3d8->Release();	

#ifdef UNI_LOGHTML
	uniLog("<b>System destroyed</b>\n\n");
#endif

}

void UniSystem::initDemo() {
	mainInit();
#ifdef UNI_LOGHTML
	uniLog("<b>Demo Initialised</b>\n");
#endif
	uniTimerReset();
}

void UniSystem::renderDemo() {
	mainRender(uniTimer());
}

void UniSystem::exitDemo() {
	mainExit();
}


long CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
/*
    POINT ptCursor;
	GetCursorPos(&ptCursor);
    ScreenToClient(uniSystem->->windowHandle, &ptCursor);

	mouseInput.posX = ptCursor.x;
	mouseInput.posY = ptCursor.y;
*/
	switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
        break;
        case WM_KEYDOWN:
            switch(wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
                break;
             }
//            keyInput[wParam] = TRUE;
        break;
#ifdef UNI_RESIZABLE_WINDOW
        case WM_SIZE:
			if (uniSystem->windowed) {
				RECT m_rcWindowClient;

				GetClientRect(uniSystem->windowHandle, &m_rcWindowClient );
				uniLog("blah : %d, %d\n", m_rcWindowClient.right  - m_rcWindowClient.left,
					m_rcWindowClient.bottom - m_rcWindowClient.top);


				int left = m_rcWindowClient.left;
				int right = m_rcWindowClient.right;
				int top = m_rcWindowClient.top;
				int bottom = m_rcWindowClient.bottom;
				int width = right - left;
				int height = top - bottom;
				if (((width) < 5) || ((height) < 5)) {
					break;
				}

				uniSystem->restoreDirect3D();

				int x1 = (uniSystem->viewportX1 / (uniSystem->viewportX2 - uniSystem->viewportX1)) * width;
				int y1 = (uniSystem->viewportY1 / (uniSystem->viewportY2 - uniSystem->viewportY1)) * height;
				int x2 = (uniSystem->viewportX2 / (uniSystem->viewportX2 - uniSystem->viewportX1)) * width;
				int y2 = (uniSystem->viewportY2 / (uniSystem->viewportY2 - uniSystem->viewportY1)) * height;

				D3DVIEWPORT8 viewData = {x1, y1, x2 - x1, y2 - y1, 0.0f, 1.0f};
				uniSystem->d3d8Device->SetViewport(&viewData);

				uniClearBuffers(UNI_BBF, 0);
				uniShowBuffers();
				uniClearBuffers(UNI_BBF, 0);
			}
		break;
#endif
		/*
        case WM_KEYUP:
            keyInput[wParam] = FALSE;
		break;
        case WM_RBUTTONDOWN:
			mouseInput.buttonRight = true;
		break;
        case WM_MBUTTONDOWN:
			mouseInput.buttonMiddle = true;
		break;
        case WM_LBUTTONDOWN:
			mouseInput.buttonLeft = true;
		break;
        case WM_RBUTTONUP:
			mouseInput.buttonRight = false;
		break;
        case WM_MBUTTONUP:
			mouseInput.buttonMiddle = false;
		break;
        case WM_LBUTTONUP:
			mouseInput.buttonLeft = false;
		break;
*/
	}
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//#endif
/*
<stefojoxa> BOOL SetThreadPriority(
<stefojoxa>   HANDLE hThread, // handle to the thread
<stefojoxa>   int nPriority   // thread priority level
<stefojoxa> );
<stefojoxa> nPriority 
<stefojoxa> Specifies the priority value for the thread. This parameter can be one of the following values: Priority Meaning 
<stefojoxa> THREAD_PRIORITY_ABOVE_NORMAL Indicates 1 point above normal priority for the priority class. 
<stefojoxa> THREAD_PRIORITY_BELOW_NORMAL Indicates 1 point below normal priority for the priority class. 
<stefojoxa> THREAD_PRIORITY_HIGHEST Indicates 2 points above normal priority for the priority class. 
<stefojoxa> THREAD_PRIORITY_IDLE Indicates a base priority level of 1 for IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS
<oysrob> hmmm
<oysrob> i dont have a thread or something
<oysrob> yes one
<oysrob> but no handle to it
<oysrob> the app it self
<stefojoxa> hold
<stefojoxa> THREAD_PRIORITY_LOWEST Indicates 2 points below normal priority for the priority class. 
<stefojoxa> THREAD_PRIORITY_NORMAL Indicates normal priority for the priority class. 
<stefojoxa> THREAD_PRIORITY_TIME_CRITICAL Indicates a base priority level of 15 for IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a base priority level of 31 for REALTIME_PRIORITY_CLASS processes. 
<stefojoxa> one should be able to get the handle
<stefojoxa> hold
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {

#ifdef UNI_FULL_DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#endif

	HANDLE thread = GetCurrentThread();

	SetThreadPriority(thread, THREAD_PRIORITY_NORMAL);

	uniSystem = new UniSystem(hInstance, lpCmdLine);

	uniSystem->initDemo();
	
  	MSG  msg;
	do {		
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		if (!uniSystem->pause)
			uniSystem->renderDemo();

		uniShowBuffers();

#ifdef  UNI_SHOW_FPS
		fpsCounter++;

		float ihhi = uniTimer();
		if ((ihhi - fpsTimer) > 50)
		{
			fpsFPS = ((float)fpsCounter / (float)(ihhi - fpsTimer)) * 1000.f;
			fpsTimer = ihhi;
			fpsCounter = 0;
		}
#endif
		if (steffoPlayer != NULL)
		{
			steffoPlayer->process();
		}

		//_sleep(1);
    } while (msg.message != WM_QUIT);

	uniSystem->exitDemo();

	uniQuit();

    return 0;
}
