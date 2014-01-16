#include "Main.h"

bool FastRoad::Init() 
{
	scene = new Scene();

	scene->addObject(genPlane(39, 100, "road03"));

	Object *obj = scene->object[0];

	int index = 0;
	for (int z = 0; z < 40; z++)
	{
		for (int x = 0; x < 40; x++)
		{
			obj->vertex[index].y = (sin(z / 10.f) * 7.f) + ((sin(x / 10.f) * 2.f) - 2.f);
			obj->vertex[index].z *= 3.f;
			index++;
		}		
	}


	scene->object[0]->finish(true);

	scene->ambient = 0x202020;

	scene->addLight(new Light(1, 1, 1, 1));
	
	return true;
}   


void FastRoad::Do(dword timer) 
{
	Object *obj = scene->object[0];

	int index = 0;
	for (int v = 0; v < 40; v++)
	{
		for (int u = 0; u < 40; u++)
		{
			obj->vertex[index].tu1 = (float)u / 40.f;
			obj->vertex[index].tv1 = (float)v / 40.f * 2.f + (fmod(timer / 400.f, 1));
			index++;
		}		
	}
		
	float x, y;

	x = sin(timer / 4000.f) * sin(timer / 5000.f) * 20.f;
	y = sin(timer / 3000.f) * sin(timer / 4100.f)  * 1.f;

	scene->camera->setLoc(x, 14 + y, -100);

	timer += 3000.f;

	x = sin(timer / 4000.f) * sin(timer / 5000.f) * 20.f;

	scene->camera->setTar(x, 0, 0);

	scene->camera->fov = 1.1f;
	scene->light[0]->setLoc(0, 14, -90);

	scene->fog = true;
	scene->fogEnd = 190;

	scene->render();
}

void FastRoad::DeInit() 
{
	delete scene;
};
