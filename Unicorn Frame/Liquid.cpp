#include "Main.h"

bool Liquid::Init() 
{
	scene = new Scene();

	objOud = genSphere(20, 2.f, NULL);

	scene->addObject(genCube(20, 2.f, "texture01"));
	scene->object[0]->shade    = SHADE_ENVMAP;
	scene->object[0]->optimize(0);

	scene->addObject(genPlane(8, 40.f, "texture02"));

	scene->object[1]->setLoc(0, -0.5f, 0);
	scene->object[1]->scaleUV(2.f, 2.f);

	Object * obj = scene->object[1];
	for (int i = 0; i < obj->nrVertices; i++) {
		float x = obj->vertex[i].x;
		float y = obj->vertex[i].y;
		float z = obj->vertex[i].z;
		float ll = (float) sqrt((x * x) + (z * z));
		if (ll > 7.0f)
			y = (ll - 7.0f) / 3.f;
		if (ll > 15.f)
			y += uniRand()*3;
		obj->vertex[i].y = y;		
	}

	scene->fog = true;
	scene->fogStart = 17.f;
	scene->fogEnd   = 27.f;
	scene->fogColor = 0x050520;

	scene->addLight(new Light(1, 1, 1, 1));

	scene->ambient = 0x202020;

	scene->object[0]->finish(true);

	scene->object[1]->finish(false);

	return true;
}   

void Liquid::Do(dword timer) 
{
	timer += 18850 * 2;

	float speed = (float) sin(timer / 1000.f) * 1.0f + 4.0f;
	float height =	0.5f - (float) cos(timer / 3000.f) * 2.0f;

	for (int i = 0; i < objOud->nrVertices; i++) {
		float x = objOud->vertex[i].x;
		float y = objOud->vertex[i].y;
		float z = objOud->vertex[i].z;
 
		float scale = (float) sin((x * speed) + (timer / 600.f)) +
					  (float) sin((y * speed) + (timer / 400.f)) +
					  (float) sin((z * speed) + (timer / 500.f));

		scale *= 0.25f;

		scale += 1.f;

		x = x + (scale * 0.1f);
		y = y * scale;
		z = z + (scale * 0.1f);

		y += height;

		if (y < 0) {
			x *= (-y * 1.f) + 1.f;
			z *= (-y * 1.f) + 1.f;
			y *= 0.1666667f;
		}

		scene->object[0]->vertex[i].x = x;
		scene->object[0]->vertex[i].y = y;
		scene->object[0]->vertex[i].z = z;
	}


	float x = (float) sin(timer / 4000.f) * 6.f;
	float y = 1.9f;
	float z = (float) cos(timer / 4000.f) * 7.f;

	scene->camera->setLoc(x, y, z);
	scene->camera->setTar(0, (float) cos(timer / 3000.f) * -1.0f + 1, 0);

	scene->light[0]->setLoc(x, y, z);

	scene->render();
}

void Liquid::DeInit() 
{
	delete objOud;
	delete scene;
};
