/*

	Pyramid DX8 System - 2D Sprite Code (header file)


	Remarks:
		-


	Updates:
		- 09-11-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/
 
#ifndef __2DStuff_h__
#define __2DStuff_h__

#define D3DFVF_VERTEX2DTL  (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct Vertex2DTL {
    float	x, y, z;
	float	rhw;
	dword	diffuse;   
    float   tu, tv;
};

class SpriteDrawer
{
public:
	SpriteDrawer();
	~SpriteDrawer();

	void Start(dword flags);
	void Stop();
	void Draw2D(float x1, float y1, float tu1, float tv1, dword color1,
		        float x2, float y2, float tu2, float tv2, dword color2,
		        float x3, float y3, float tu3, float tv3, dword color3,
		        float x4, float y4, float tu4, float tv4, dword color4);

private:
	UniVBuffer * vertexBuffer;
	DWORD stateZFUNC;
	bool okToRender;
};

#endif
