/*

	Pyramid DX8 Engine - Scene code (source file)


	(c) 2001, Robert Jan Bruinier

*/


#include "Engine.h"

Scene::Scene() {
	nrLights  = 0;
	nrObjects = 0;
	//nrSprites = 0;
	nrSpriteGroups = 0;
	camera = new Camera();

	ambient = 0xffffffff;

	fog = false;
	fogPixel   = false;
	fogColor   = 0x000000;
	fogStart   = 0.0f;
	fogEnd	   = 3.0f;
	fogDensity = 0.5f;
	fogMode    = D3DFOG_LINEAR;

	fogSprites = false;
}

Scene::~Scene() {
	for (int i = 0; i < nrObjects; i++)
		delete object[i];
	if (nrObjects)
		delete [] object;

	for (i = 0; i < nrLights; i++)
		delete light[i];
	if (nrLights)
		delete [] light;
/*
	for (i = 0; i < nrSprites; i++)
		delete sprite[i];
	if (nrSprites)
		delete [] sprite;
*/
	for (i = 0; i < nrSpriteGroups; i++)
		delete spriteGroup[i];
	if (nrSpriteGroups)
		delete [] spriteGroup;

	delete camera;
}

void Scene::finish()
{
	for (int i = 0; i < nrObjects; i++)
	{
		object[i]->finish(false);
	}
}

void Scene::render() {
	int i;

	uniSetRenderState(D3DRS_AMBIENT, ambient);

	if (fog) {
		uniSetRenderState(D3DRS_FOGENABLE, true);
		uniSetRenderState(D3DRS_FOGCOLOR, fogColor);
		if (fogPixel) {
			if (fogMode == D3DFOG_LINEAR) {
				uniSetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
				uniSetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fogStart));
				uniSetRenderState(D3DRS_FOGEND,   *(DWORD *)(&fogEnd));
			} else {
				uniSetRenderState(D3DRS_FOGTABLEMODE, fogMode);
				uniSetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&fogDensity));
			}
		} else {
			uniSetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			uniSetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fogStart));
			uniSetRenderState(D3DRS_FOGEND,   *(DWORD *)(&fogEnd));
		}
	} else {
		uniSetRenderState(D3DRS_FOGENABLE, false);
	}

	for (i = 0; i < nrLights; i++)
		light[i]->activate();

	camera->update();

	uniStart();

	for (i = 0; i < nrObjects; i++) {
		object[i]->render();
	}

	for (i = 0; i < nrLights; i++)
		light[i]->deactivate();

	if (!fogSprites)
		uniSetRenderState(D3DRS_FOGENABLE, false);

//	if (nrSprites + nrSpriteGroups + nrLights)
	if (nrSpriteGroups + nrLights)
		renderSprites();

	if (fogSprites)
		uniSetRenderState(D3DRS_FOGENABLE, false);

	uniStop(); 
}

void Scene::renderSprites() {
	uniSetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE);

	uniSetRenderState(D3DRS_ALPHABLENDENABLE, true);
	uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
	uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	uniSetRenderState(D3DRS_LIGHTING, false);

    uniSetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
    uniSetRenderState(D3DRS_POINTSCALEENABLE,  TRUE);
    uniSetRenderState(D3DRS_POINTSIZE_MIN, uniFtoDW(0.0f));
    uniSetRenderState(D3DRS_POINTSCALE_A,  uniFtoDW(0.0f));
    uniSetRenderState(D3DRS_POINTSCALE_B,  uniFtoDW(0.0f));
    uniSetRenderState(D3DRS_POINTSCALE_C,  uniFtoDW(1.0f));


	uniSetRenderState(D3DRS_ZWRITEENABLE, false);

	uniSetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	D3DMATRIX matrix;
	uniMatrixIdentity(matrix);
	uniSetTransformWorld(matrix);

	for (int i = 0; i < nrLights; i++)
		light[i]->render();
/*
	for (i = 0; i < nrSprites; i++)
		sprite[i]->render();
*/
	for (i = 0; i < nrSpriteGroups; i++)
		spriteGroup[i]->render();

    uniSetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
    uniSetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

	uniSetRenderState(D3DRS_LIGHTING, true);
	uniSetRenderState(D3DRS_ALPHABLENDENABLE, false);
	uniSetRenderState(D3DRS_ZWRITEENABLE, true);
}

Object * Scene::findObject(char * naam) {
	for (int i = 0; i < nrObjects; i++) {
		if (lstrcmp(object[i]->name, naam) == 0)
			return object[i];
	}
	return NULL;
}

void Scene::addObject(Object * obj) {
	Object ** temp = new Object*[nrObjects + 1];

	for (int i = 0; i < nrObjects; i++) {
		temp[i] = object[i];
	}

	if (nrObjects > 0)
		delete object;

	object = temp;

	object[nrObjects] = obj;

	nrObjects++;
}

void Scene::addLight(Light * lht) {
	Light ** temp = new Light*[nrLights + 1];

	for (int i = 0; i < nrLights; i++) {
		temp[i] = light[i];
	}

	if (nrLights)
		delete light;

	light = temp;

	light[nrLights] = lht;

	nrLights++;
}
/*
void Scene::addSprite(Sprite * spt) {
	Sprite ** temp = new Sprite*[nrSprites + 1];

	for (int i = 0; i < nrSprites; i++) {
		temp[i] = sprite[i];
	}

	if (nrSprites)
		delete sprite;

	sprite = temp;

	sprite[nrSprites] = spt;

	nrSprites++;
}
*/
void Scene::addSpriteGroup(SpriteGroup * spt) {
	SpriteGroup ** temp = new SpriteGroup*[nrSpriteGroups + 1];

	for (int i = 0; i < nrSpriteGroups; i++) {
		temp[i] = spriteGroup[i];
	}

	if (nrSpriteGroups)
		delete spriteGroup;

	spriteGroup = temp;

	spriteGroup[nrSpriteGroups] = spt;

	nrSpriteGroups++;
}
