/*

	Pyramid DX8 System - Main System Header


	Remarks:
		-


	Updates:
		- 21-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __UNICORN_H__
#define __UNICORN_H__

#ifndef UNI_TITLE
#define UNI_TITLE "westside twiddly - aardbei"
#endif

#ifndef UNI_LOGNAME
#define UNI_LOGNAME "aardbei log file.html"
#endif

#define UNI_NONE		 0

#define UNI_ZBF			 1
#define UNI_BBF			 2

#define UNI_LOG			 1		// loggen?
#define UNI_TNL			 2		// logen te hardware tnl'en?
#define UNI_WVR			 4		// op vertical retrace wachten?
#define UNI_WIN			 8		// windowed?

#define UNI_DRAW		 0		// gewoon pixels fillen
#define UNI_ADD			 1		// pixels adden bij backbuffer pixels
#define UNI_ALPHA		 2		// alpha shaden

#define UNI_DYNAMIC		 1		// dynamische texture
#define UNI_MIPMAP		 2		// mipmappen tot zo klein als mogelijk
#define UNI_MIPMAP2		 4		// mipmappen met 2 levels
#define UNI_MIPMAP3		 8		// etc etc
#define UNI_MIPMAP4		 16
#define UNI_MIPMAP5		 32
#define UNI_MIPMAP6		 64
#define UNI_MIPMAP7		 128
#define UNI_MIPMAP8		 256
#define UNI_RENDERTARGET 512	// deze texture als rendertarget?

typedef unsigned int   dword;
typedef unsigned short word;

#include <Windows.h>
#include <Mmsystem.h>
#include <math.h>

#include "..\Include\D3d8.h"

#include "SysSetup.h"

#include "Adapter.h"
#include "System.h"
#include "Module.h"
#include "Loghtml.h"
#include "Files.h"
#include "VBuffer.h"
#include "IBuffer.h"
#include "Texture.h"
#include "Bitmap.h"
#include "Script.h"
#include "2DStuff.h"



/*
struct Vertex2DTL {
    float	x, y, z;
	float	rhw;
	dword	diffuse;   
    float   tu, tv;
};

struct Vertex3DL {
    float	x, y, z;
	dword	diffuse;   
    float   tu, tv;
};

struct VertexPoint {
public:
	float	x, y, z;
	dword	diffuse;

public:
	void setLoc(const float x, const float y, const float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void setDiffuse(const dword color) {
		this->diffuse = color;
	}
};

#define D3DFVF_VERTEX2DTL  (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_VERTEX3DL   (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_VERTEXPOINT (D3DFVF_XYZ | D3DFVF_DIFFUSE)

*/
#endif
