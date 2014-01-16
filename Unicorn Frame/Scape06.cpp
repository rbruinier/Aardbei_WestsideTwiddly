#include "Main.h"

bool Scape06::Init() 
{
	scene = new Scene();

	scene->addObject(genPlane(39, 300, "texture05"));
	scene->object[0]->finish(true);

	return true;
}   


void Scape06::Do(dword timer) 
{
	timer += 0;

	uniSetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

	float divver = timer / 400.f;
	for (int i = 0; i < scene->object[0]->nrVertices; i++) 
	{
		float x = scene->object[0]->vertex[i].x;
		float z = scene->object[0]->vertex[i].z;
		float y = sin((x * 1.5f) + (z * 1.5f) + divver) * 0.4f;
		float v = y + 0.4f;
//		scene->object[0]->shade = SHADE_ENVMAP;
		scene->object[0]->vertex[i].y = y;
		scene->object[0]->vertex[i].tv1 = v;
	}

	scene->camera->setLoc(100, 10, 50);
	scene->render();

	uniSetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
}

void Scape06::DeInit() 
{
	delete scene;
};
