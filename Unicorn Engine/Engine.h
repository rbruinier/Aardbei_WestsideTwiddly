/*

	Pyramid DX8 Engine - Engine Header


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "..\Unicorn System\Unicorn.h"

#define _PI  3.1415926535f
#define _PI2 6.2831853070f

#include "EngSetup.h"

enum CULL_MODE {CULL_NONE = D3DCULL_NONE, CULL_CW = D3DCULL_CW, CULL_ACW = D3DCULL_CCW};

#define SHADE_NONE		0
#define SHADE_ENVMAP	1
#define	SHADE_PHONG		2
#define SHADE_FLAT		4

#define METHOD_NONE		0
#define METHOD_POINT	1
#define METHOD_LINE		2
#define METHOD_SOLID	4

struct Face {
	unsigned short a, b, c;
};

struct Vertex {
	float x,y,z;
	float nx,ny,nz;
	float tu1,tv1;
	float tu2,tv2;
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

//#define D3DFVF_VERTEX2DTL  (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//#define D3DFVF_VERTEX3DL   (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//#define D3DFVF_VERTEXPOINT (D3DFVF_XYZ | D3DFVF_DIFFUSE)


#define ENGINE_VERTEX      (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE2(1))
#define ENGINE_VERTEXPOINT (D3DFVF_XYZ | D3DFVF_DIFFUSE)


#include "Matrix.h"
#include "Vector.h"
#include "Object.h"
#include "Camera.h"
#include "Sprite.h"
#include "Light.h"
#include "Scene.h"
#include "Generate.h"

#endif