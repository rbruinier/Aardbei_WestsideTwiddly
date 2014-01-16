/*

	Pyramid DX8 System - Adapter Code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "Unicorn.h"

dword uniGetBppFromFormat(const D3DFORMAT format) {
	switch (format) {
		case D3DFMT_A4R4G4B4:
			return 16;
		case D3DFMT_X4R4G4B4:
			return 16;
		case D3DFMT_X1R5G5B5:
			return 16;
		case D3DFMT_A1R5G5B5:
			return 16;
		case D3DFMT_R5G6B5:
			return 16;
		case D3DFMT_A8R3G3B2:
			return 16;
		case D3DFMT_X8R8G8B8:
			return 32;
		case D3DFMT_A8R8G8B8:
			return 32;
		case D3DFMT_R8G8B8:
			return 32;
	}
	return -1;
}

bool uniCheckTextureFormat(dword adapter, D3DFORMAT adapterFormat, D3DFORMAT textureFormat) {
	return (D3D_OK == uniSystem->d3d8->CheckDeviceFormat(adapter, D3DDEVTYPE_HAL, adapterFormat, 0,
							                         D3DRTYPE_TEXTURE, textureFormat));
}

bool uniCheckZbufferFormat(dword adapter, D3DFORMAT adapterFormat, D3DFORMAT zbufferFormat) {
	return (D3D_OK == uniSystem->d3d8->CheckDeviceFormat(adapter, D3DDEVTYPE_HAL, adapterFormat, 
		                                                 D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, 
												         zbufferFormat));
}

bool uniCheckBackBufferFormat(dword adapter, D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat) {
	return (D3D_OK == uniSystem->d3d8->CheckDeviceFormat(adapter, D3DDEVTYPE_HAL, adapterFormat, 
   		                                                 D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, 
												         backBufferFormat));
}


bool uniFindFormats(dword nummer, D3DFORMAT videoFormat, D3DFORMAT &bbufferFormat, D3DFORMAT &zbufferFormat, 
								  D3DFORMAT &textureFormat16, D3DFORMAT &textureFormat32) {
	bbufferFormat = D3DFMT_UNKNOWN;
	zbufferFormat = D3DFMT_UNKNOWN;
	textureFormat16 = D3DFMT_UNKNOWN;
	textureFormat32 = D3DFMT_UNKNOWN;

	if (uniCheckTextureFormat(nummer, videoFormat, D3DFMT_A1R5G5B5))
		textureFormat16 = D3DFMT_A1R5G5B5;
	else
	if (uniCheckTextureFormat(nummer, videoFormat, D3DFMT_A4R4G4B4))
		textureFormat16 = D3DFMT_A4R4G4B4;
	else
	if (uniCheckTextureFormat(nummer, videoFormat, D3DFMT_A8R3G3B2))
		textureFormat16 = D3DFMT_A8R3G3B2;
	else
	if (uniCheckTextureFormat(nummer, videoFormat, D3DFMT_R5G6B5))
		textureFormat16 = D3DFMT_R5G6B5;

	if (uniCheckTextureFormat(nummer, videoFormat, D3DFMT_A8R8G8B8))
		textureFormat32 = D3DFMT_A8R8G8B8;
	else
	if (uniCheckTextureFormat(nummer, videoFormat, D3DFMT_X8R8G8B8))
		textureFormat32 = D3DFMT_X8R8G8B8;

	if (uniCheckZbufferFormat(nummer, videoFormat, D3DFMT_D16))
		zbufferFormat = D3DFMT_D16;
	else
	if (uniCheckZbufferFormat(nummer, videoFormat, D3DFMT_D32))
		zbufferFormat = D3DFMT_D32;

	if (uniGetBppFromFormat(videoFormat) == 16) {
		if (uniCheckBackBufferFormat(nummer, videoFormat, D3DFMT_A1R5G5B5))
			bbufferFormat = D3DFMT_A1R5G5B5;
		else
		if (uniCheckBackBufferFormat(nummer, videoFormat, D3DFMT_A4R4G4B4))
			bbufferFormat = D3DFMT_A4R4G4B4;
		else
		if (uniCheckBackBufferFormat(nummer, videoFormat, D3DFMT_A8R3G3B2))
			bbufferFormat = D3DFMT_A8R3G3B2;
		else
		if (uniCheckBackBufferFormat(nummer, videoFormat, D3DFMT_R5G6B5))
			bbufferFormat = D3DFMT_R5G6B5;
	} else {
		if (uniCheckBackBufferFormat(nummer, videoFormat, D3DFMT_A8R8G8B8))
			bbufferFormat = D3DFMT_A8R8G8B8;
		else
		if (uniCheckBackBufferFormat(nummer, videoFormat, D3DFMT_X8R8G8B8))
			bbufferFormat = D3DFMT_X8R8G8B8;
		else
		if (uniCheckBackBufferFormat(nummer, videoFormat, D3DFMT_R8G8B8))
			bbufferFormat = D3DFMT_R8G8B8;
	}

	if ((zbufferFormat != D3DFMT_UNKNOWN) && ((textureFormat16 != D3DFMT_UNKNOWN) ||
		(textureFormat32 != D3DFMT_UNKNOWN)) && (bbufferFormat != D3DFMT_UNKNOWN))
		return true;
	else 
		return false;
}


UniAdapter::UniAdapter(dword adapter, bool windowed) {
	D3DDISPLAYMODE tempMode;
	D3DFORMAT textureFormat16, textureFormat32;
	D3DFORMAT bbufferFormat;
	D3DFORMAT zbufferFormat;

	this->adapter = adapter;
	
	D3DADAPTER_IDENTIFIER8 adapterIdentifier;

	if (D3D_OK != uniSystem->d3d8->GetAdapterIdentifier(adapter, NULL, &adapterIdentifier))
		uniQuit("Unable to get information about display adaptor");

	strcpy(name, adapterIdentifier.Description);

	nrVideoModes = 0;

	if (windowed) {
		videoMode = new UniDisplayMode[1];

		if (D3D_OK != uniSystem->d3d8->GetAdapterDisplayMode(adapter, &tempMode))
			uniQuit("Unable to get information about display adaptor");

		if (uniFindFormats(adapter, tempMode.Format, bbufferFormat, zbufferFormat, textureFormat16, textureFormat32))
			if (D3D_OK == uniSystem->d3d8->CheckDeviceType(adapter, D3DDEVTYPE_HAL, tempMode.Format, tempMode.Format, windowed))
				addVideoMode(tempMode, bbufferFormat, zbufferFormat, textureFormat16, textureFormat32);
			else
				uniQuit("No suitable 3d harware found");

	} else {
		int videoModes = uniSystem->d3d8->GetAdapterModeCount(adapter);

		videoMode = new UniDisplayMode[videoModes];
 
		for (int j = 0; j < videoModes; j++) {
			if (D3D_OK != uniSystem->d3d8->EnumAdapterModes(adapter, j, &tempMode))
				uniQuit("Unable to get information about available videomodes");

			if (uniFindFormats(adapter, tempMode.Format, bbufferFormat, zbufferFormat, textureFormat16, textureFormat32))
				if (D3D_OK == uniSystem->d3d8->CheckDeviceType(adapter, D3DDEVTYPE_HAL, tempMode.Format, tempMode.Format, windowed))
					addVideoMode(tempMode, bbufferFormat, zbufferFormat, textureFormat16, textureFormat32);
		}
	}

	if (D3D_OK != uniSystem->d3d8->GetDeviceCaps(adapter, D3DDEVTYPE_HAL, &caps))
		uniQuit("Unable to get information about device capabilities");

	hwTnL = false;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		if (caps.MaxPointSize <= 0.0f)
		{
			hwTnL = false;
		}
		else
		{
			hwTnL = true;
		}
	}

	texMaxWidth  = caps.MaxTextureWidth;
	texMaxHeight = caps.MaxTextureHeight;

	showInfo();	

}

UniAdapter::~UniAdapter() {
	if (nrVideoModes)
		delete [] videoMode;
}

// zoekt d3d video mode bij gevraagde gegevens, als niet gevonden bij 32 bit probeert
// het 16 bit mode te vinden
UniDisplayMode * UniAdapter::findVideoMode(dword width, dword height, dword bit) {
	int fastest = -1;

	for (int i = 0; i < nrVideoModes; i++) {
		if ((videoMode[i].videoMode.Width  == width) &&
			(videoMode[i].videoMode.Height == height) &&
			(videoMode[i].bpp == bit))
		{
			if (fastest == -1)
			{
				fastest = i;
			}
			else
			{
				if (videoMode[i].videoMode.RefreshRate > videoMode[fastest].videoMode.RefreshRate)
				{
					fastest = i;
				}
			}

		}
	}

	if (fastest != -1)
	{
		return &videoMode[fastest];
	}

	if (bit == 32) 
	{
		return findVideoMode(width, height, 16);
	}

	return NULL;
}

void UniAdapter::addVideoMode(D3DDISPLAYMODE vid, D3DFORMAT back, D3DFORMAT zbuf, D3DFORMAT text16, D3DFORMAT text32) 
{
	videoMode[nrVideoModes].bbufferFormat   = back;
	videoMode[nrVideoModes].textureFormat16 = text16;
	videoMode[nrVideoModes].textureFormat32 = text32;
	videoMode[nrVideoModes].zbufferFormat   = zbuf;
	videoMode[nrVideoModes].videoMode       = vid;
	videoMode[nrVideoModes].bpp             = uniGetBppFromFormat(vid.Format);
	nrVideoModes++;
}

void UniAdapter::showInfo() {
#ifdef UNI_LOGHTML
	uniLog("      -> <i><b>%s</b></i>\n", name);
	uniLog("         Available videomodes (%d):\n", nrVideoModes);

	// kijk of er dubbele videomodes in lijst voorkomen, zoja, kijk naar hoogste refreshrate
	for (int i = 0; i < nrVideoModes; i++) {
		bool bestaatal = false;
		for (int j = 0; j < nrVideoModes; j++) {
			if ((videoMode[i].videoMode.Width  == videoMode[j].videoMode.Width) &&
			    (videoMode[i].videoMode.Height == videoMode[j].videoMode.Height) &&
			    (videoMode[i].videoMode.Format == videoMode[j].videoMode.Format)) {
				if (videoMode[j].videoMode.RefreshRate > videoMode[i].videoMode.RefreshRate) {
					bestaatal = true;
					break;
				}
			}
		}
		// dit is een videomode die we kunnen kiezen
		if (!bestaatal) {
			uniLog("            -> %2d: %dx%dx%d	(bb:%d, zb:%d, t16:%d, t32:%d)\n", i, videoMode[i].videoMode.Width, 
								                        videoMode[i].videoMode.Height, videoMode[i].bpp, videoMode[i].bbufferFormat, videoMode[i].zbufferFormat, videoMode[i].textureFormat16, videoMode[i].textureFormat32); 
		}
	}

	if (hwTnL)
		uniLog("         Hardware TnL support: YES\n");
	else
		uniLog("         Hardware TnL support: NO\n");
#endif
}

/*
void enumerateAdapters() {
	dword nrAdapters;
	nrAdapters = unicorn->d3d8->GetAdapterCount();
	uniLog("   Number of available physical 3D adapters: %d\n", nrAdapters);

	if (nrAdapters == 0)
		uniQuit("UNI Says: no 3D hardware found");

	if (!unicorn->window) {
		unicorn->adapter = new uniAdapter*[nrAdapters];
		for (int i = 0; i < nrAdapters; i++) {
			unicorn->adapter[i] = new uniAdapter(i, false);
		}
	} else {
			unicorn->adapter = new uniAdapter*[1];
			unicorn->adapter[0] = new uniAdapter(D3DADAPTER_DEFAULT, true);
	}
}
*/