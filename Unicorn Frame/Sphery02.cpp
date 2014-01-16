#include "Main.h"

bool Sphery02::Init() 
{
	scene = new Scene();

	scene->addObject(genSphere(26, 2.f, "texture04"));
//	scene->object[0]->shade = SHADE_ENVMAP;
//	scene->object[0]->optimize(0);

	oldObject = genSphere(26, 2.f, NULL);

	scene->object[0]->finish(true);

	scene->ambient = 0x0;

	scene->addLight(new Light(1.3f, 1.3f, 1.3f, 1));
//	scene->addLight(new Light(-0.8, -0.8, -0.8, 1));

	return true;
}   

void Sphery02::Do(dword timer) 
{	
	timer += 34000;
	Vertex *src = oldObject->vertex;
	for (int i = 0; i < oldObject->nrVertices; i++)
	{
		float x = src[i].x;
		float y = src[i].y;
		float z = src[i].z;

		float scale;

		scale = 1 + (sin((x * 7.f) + (timer / 3000.f)) * (sin(timer / 3000.f) * 0.4f)) + (cos((z * 6.5f) - (timer / 2000.f)) * 0.5f);
//		scale = 1 + (sin((x * 4.f) + (timer / 3000.f)) * (sin(timer / 3000.f) * 0.4f)) + (cos((z * 3.5f) - (timer / 2000.f)) * 0.5f);

		x *= scale;
		y *= scale;
		z *= scale;

		float dist = 3.0f - sqrt((x * x) + (y * y) + (z * z));

		D3DMATRIX rotate;
//		uniMatrixRotate(rotate, dist * sin(timer / 1700.f), dist * cos(timer / 1100.f), dist * sin(timer / 1500.f));
		uniMatrixRotate(rotate, sin((timer / 1500.f) + (dist * 0.3f)) * 3.3f, 
			                    cos((timer / 2300.f) + (dist * 0.3f)) * 3.3f, 
								sin((timer / 1800.f) + (dist * 0.3f)) * 3.3f);


		D3DVECTOR point = {x, y, z};
		uniMatrixMultiplyVector(point, point, rotate);


		scene->object[0]->vertex[i].x = point.x;
		scene->object[0]->vertex[i].y = point.y;
		scene->object[0]->vertex[i].z = point.z;		
	}

	scene->camera->setLoc(sin(timer / 6000.f) * 5.f, 0, cos(timer / 6000.f) * 7.f);
	scene->light[0]->setLoc(sin(timer / 6000.f) * 5.f, 1, cos(timer / 6000.f) * 7.f);
//	scene->light[0]->setLoc(sin(timer / 6000.f) * 8.f, 1, cos(timer / 6000.f) * 7.f);

	scene->render();
}

void Sphery02::DeInit() 
{
	delete scene;
	delete oldObject;
}
