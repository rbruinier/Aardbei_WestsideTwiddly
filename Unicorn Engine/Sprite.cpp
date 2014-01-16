/*

	Pyramid DX8 Engine - Sprite code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "Engine.h"
/*
Sprite::Sprite(float size, dword color, char * texture) {
	this->size		= size;
	this->color     = color;

	this->texture = NULL;

	setTexture(texture);

	xLoc = yLoc = zLoc = 0;
}

Sprite::~Sprite() {
	setTexture(NULL);
}

void Sprite::render() {
	uniSetTexture(texture);

    VertexPoint v;
	v.x = xLoc;
	v.y = yLoc;
	v.z = zLoc;
	v.diffuse = color;

    uniSetRenderState(D3DRS_POINTSIZE, *(DWORD*)&size);

	vertexBufferPoint->fill((dword*) &v);

	vertexBufferPoint->set();
	unicorn->d3d8Device->DrawPrimitive(D3DPT_POINTLIST, 0, 1);
}


void Sprite::setSize(float size) {
	this->size = size;
}

void Sprite::setLoc(float x, float y, float z) {
	xLoc = x;
	yLoc = y;
	zLoc = z;
}

void Sprite::setColor(dword color) {
	this->color = color;
}

void Sprite::setTexture(char * naam) {
	if (texture != NULL) 
		delete [] texture;
	if (naam != NULL) {
		texture = new char[lstrlen(naam) + 1];
		lstrcpy(texture, naam);
	} else {
		texture = NULL;
	}
}
*/

SpriteGroup::SpriteGroup()
{
}

SpriteGroup::SpriteGroup(int nrSprites, float size, dword color, char * texture) {
	this->nrSprites = nrSprites;
	this->size      = size;

	this->texture   = NULL;
	setTexture(texture);

	enabled = true;

	blend = UNI_ADD;

	sprite = new VertexPoint[nrSprites];

	for (int i = 0; i < nrSprites; i++) {
		sprite[i].setLoc(0, 0, 0);
		sprite[i].setDiffuse(color);
	}

	vertexBuffer = new UniVBuffer((dword*) sprite, sizeof(VertexPoint), nrSprites * sizeof(VertexPoint), ENGINE_VERTEXPOINT, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS);
}


SpriteGroup::~SpriteGroup() 
{
	if (nrSprites)
		delete [] sprite;
	setTexture(NULL);
}

void SpriteGroup::render() 
{
	if (!enabled)
	{
		return;
	}

	if (blend & UNI_ADD) 
	{
		uniSetRenderState(D3DRS_ALPHABLENDENABLE, true);
		uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
		uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		uniSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}  
	else if (blend & UNI_ALPHA) 
	{
		uniSetRenderState(D3DRS_ALPHABLENDENABLE, true);
		uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		uniSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	} 
	else
	{
		uniSetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	uniSetTexture(texture);

    uniSetRenderState(D3DRS_POINTSIZE, *(DWORD*)&size);

	vertexBuffer->fill((dword*) sprite);

	vertexBuffer->set();

	uniSystem->d3d8Device->DrawPrimitive(D3DPT_POINTLIST, 0, nrSprites);
}

void SpriteGroup::setSize(float size) {
	this->size = size;
}

void SpriteGroup::setTexture(char * naam) {
	if (texture != NULL) 
		delete [] texture;
	if (naam != NULL) {
		texture = new char[lstrlen(naam) + 1];
		lstrcpy(texture, naam);
	} else {
		texture = NULL;
	}
}
