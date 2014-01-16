/*

	Pyramid DX8 System - 2D Sprite Code (source file)


	(c) 2001, Robert Jan Bruinier

*/

 
#include "Unicorn.h"

SpriteDrawer::SpriteDrawer()
{
	vertexBuffer = new UniVBuffer(NULL, sizeof(Vertex2DTL), 4 * sizeof(Vertex2DTL), 
								  D3DFVF_VERTEX2DTL, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC);

	okToRender = false;
}

SpriteDrawer::~SpriteDrawer()
{
}

void SpriteDrawer::Start(dword flags)
{
	if (okToRender)
	{
		uniQuit("Error with rendering 2D sprites");
	}

	uniSystem->d3d8Device->GetRenderState(D3DRS_ZFUNC, &stateZFUNC);

	uniSetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	uniSetRenderState(D3DRS_ZWRITEENABLE, false);

	uniSetRenderState(D3DRS_LIGHTING, false);

	uniSetRenderState(D3DRS_CULLMODE, 1);

	uniSetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	if (flags & UNI_ADD) {
		uniSetRenderState(D3DRS_ALPHABLENDENABLE, true);
		uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
		uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		uniSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}  else
	if (flags & UNI_ALPHA) {
		uniSetRenderState(D3DRS_ALPHABLENDENABLE, true);
		uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		uniSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	} else
		uniSetRenderState(D3DRS_ALPHABLENDENABLE, false);

	if (uniStart())
	{
		okToRender = true;
	}
}

void SpriteDrawer::Stop()
{
	if (okToRender)
	{
		uniStop();
	}

	uniSetRenderState(D3DRS_LIGHTING, true);
	uniSetRenderState(D3DRS_ZFUNC, stateZFUNC);
	uniSetRenderState(D3DRS_ALPHABLENDENABLE, false);
	uniSetRenderState(D3DRS_ZWRITEENABLE, true);
	uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
	uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	uniSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	okToRender = false;
}

void SpriteDrawer::Draw2D(float x1, float y1, float tu1, float tv1, dword color1,
			              float x2, float y2, float tu2, float tv2, dword color2,
			              float x3, float y3, float tu3, float tv3, dword color3,
			              float x4, float y4, float tu4, float tv4, dword color4)
{

    Vertex2DTL v[4];

	v[0].x       = x1;
	v[0].y       = y1;
	v[0].z       = 0.5f;
	v[0].tu		 = tu1;
	v[0].tv		 = tv1;
	v[0].rhw     = 0.5f;
	v[0].diffuse = color1;

	v[1].x       = x2;
	v[1].y       = y2;
	v[1].z       = 0.5f;
	v[1].tu		 = tu2;
	v[1].tv		 = tv2;
	v[1].rhw     = 0.5f;
	v[1].diffuse = color2;

	v[2].x       = x3;
	v[2].y       = y3;
	v[2].z       = 0.5f;
	v[2].tu		 = tu3;
	v[2].tv		 = tv3;
	v[2].rhw     = 0.5f;
	v[2].diffuse = color3;

	v[3].x       = x4;
	v[3].y       = y4;
	v[3].z       = 0.5f;
	v[3].tu		 = tu4;
	v[3].tv		 = tv4;
	v[3].rhw     = 0.5f;
	v[3].diffuse = color4;

	if (okToRender)
	{
		vertexBuffer->fill((dword*) v);
		vertexBuffer->set();
		uniSystem->d3d8Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
