#include "Main.h"

#define SPIKEY_SEGX   50
#define SPIKEY_SEGZ   50
#define SPIKEY_RADIUS 40.f
#define SPIKEY_PERIOD 30.f

bool Spikey::Init() 
{
	scene = new Scene();

	scene->addObject(genPlane(SPIKEY_SEGX, SPIKEY_RADIUS, "texture02"));
	scene->object[0]->scaleUV(4, 4);
	scene->object[0]->cull = CULL_NONE;
	scene->object[0]->finish(true);

	oldPlane = genPlane(SPIKEY_SEGX, SPIKEY_RADIUS, NULL);

	scene->ambient = 0x202020;
	scene->addLight(new Light(1, 1, 1, 1));
	scene->light[0]->setLoc(40, 40, 40);

	return true;
}   

void Spikey::Do(dword timer) 
{
	int index = 0;
	Vertex *target = scene->object[0]->vertex;
	for (int z = 0; z < SPIKEY_SEGZ + 1; z++)
	{
		for (int x = 0; x < SPIKEY_SEGX + 1; x++)
		{
			float pos = z + 10 + (timer / 300.f);
			float amp = sin(pos * _PI2 / SPIKEY_PERIOD);

			float xor = oldPlane->vertex[index].x;
			float yor = oldPlane->vertex[index].y;
			float zor = oldPlane->vertex[index].z;

			D3DVECTOR final = {xor, yor, zor}, original = {0, yor, 0};

			if (amp > 0)
			{
				if (z & 1) // om de z as
				{
					if (z & 2)
					{
						if (x & 1)
						{
							original.y += amp * 6.f;
						}
					}
					else
					{
						if (!(x & 1))
						{
							original.y += amp * 6.f;
						}
					}
				}
		
				float rotx = (sin(fmod(pos - 1, SPIKEY_PERIOD) * _PI / SPIKEY_PERIOD) - 0.5f) * 1.5f;
				float roty = 0;
				float rotz = 0;

				D3DMATRIX rotate;

				uniMatrixRotate(rotate, rotx, roty, rotz);

				uniMatrixMultiplyVector(original, original, rotate);

				final.x += original.x;
				final.y += original.y;
				final.z += original.z;



			}

			D3DMATRIX rotate;
			uniMatrixRotate(rotate, sin((z + (timer / 90.f)) / 6.f) * 0.5f, 
				                    sin((x + (timer / 100.f)) / 12.f) * 0.5f, 
							        cos((x + (timer / 120.f)) / 5.f) * 0.5f);
			uniMatrixMultiplyVector(final, final, rotate);

			target[index].x = final.x;
			target[index].y = final.y;
			target[index].z = final.z;

			index++;
		}
	}




	scene->camera->setLoc(sin(timer / 6000.f) * 50.f, 40.f, cos(timer / 6000.f) * 60.f);
	scene->light[0]->setLoc(sin(timer / 8000.f) * 40.f, 60, cos(timer / 9000.f) * 60.f);
//	scene->camera->setLoc(50, 0, 0);
	scene->render();
}

void Spikey::DeInit() 
{
	delete scene;
	delete oldPlane;
}
