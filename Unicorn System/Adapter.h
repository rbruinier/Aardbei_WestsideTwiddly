/*

	Pyramid DX8 System - Adapter Code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __ADAPTER_H_
#define __ADAPTER_H_

struct UniDisplayMode {
	D3DDISPLAYMODE videoMode;
	D3DFORMAT textureFormat16;
	D3DFORMAT textureFormat32;
	D3DFORMAT bbufferFormat;
	D3DFORMAT zbufferFormat;
	dword bpp;
};

class UniAdapter {
public:
	dword			adapter;
	int				nrVideoModes;
	char			name[MAX_DEVICE_IDENTIFIER_STRING];
	UniDisplayMode	*videoMode;
	D3DCAPS8		caps;

	dword			texMaxWidth;
	dword			texMaxHeight;

public:
	bool			hwTnL;

public:
	UniAdapter(dword nummer, bool windowed);
	~UniAdapter();

public:
	void showInfo();
	void addVideoMode(D3DDISPLAYMODE vid, D3DFORMAT back, D3DFORMAT zbuf, D3DFORMAT text16, D3DFORMAT text32);
	UniDisplayMode * findVideoMode(dword width, dword height, dword bit);

};

extern dword uniGetBppFromFormat(const D3DFORMAT format);

#endif
