/*
	Landschap met veel gaten, waartussen organische buizen lopen en uit sommige buizen particles komen
*/

#include "Main.h"

#define TENTACLES_SEGMENTSX     5
#define TENTACLES_SEGMENTSY    30
#define TENTACLES_RADIUS     0.2f
#define TENTACLES_LENGTH       20

#define TENTACLES_NRTENTACLES   1

void bendTentacle(Object *tar, Object *src, dword timer)
{
	float xrot = 0;
	float yrot = 0;
	float zrot = 0;

	float curLocX = 0;
	float curLocY = 0;
	float curLocZ = 0;

	int index = 0;
	for (int y = 0; y < TENTACLES_SEGMENTSY + 1; y++)
	{
		D3DMATRIX rotate;
		uniMatrixRotate(rotate, xrot * _PI / 180.f, yrot * _PI / 180.f, zrot * _PI / 180.f);
		float pos = fmod(timer / 400.f, 30);
		float radius = 1.f;
		if ((y >= pos) && (y <= (pos + 4)))
		{
			radius += sin((y - pos) * 0.8f) * 6.f;
		}
		for (int x = 0; x < TENTACLES_SEGMENTSX + 1; x++)
		{
			float x = src->vertex[index].x * radius;
			float y = 0;
			float z = src->vertex[index].z * radius;

			D3DVECTOR point = {x, y, z};
			uniMatrixMultiplyVector(point, point, rotate);

			x = point.x;
			y = point.y;
			z = point.z;

			tar->vertex[index].x = x + curLocX;
			tar->vertex[index].y = y + curLocY;
			tar->vertex[index].z = z + curLocZ;

			index++;
		}

		D3DVECTOR dir = {0, 2.0f, 0};
		D3DMATRIX dirMatrix;
		uniMatrixRotate(dirMatrix, xrot * _PI / 180.f, yrot * _PI / 180.f,  zrot * _PI / 180.f);
		uniMatrixMultiplyVector(dir, dir, dirMatrix);

		curLocX += dir.x; 
		curLocY += dir.y; 
		curLocZ += dir.z;
		
		xrot += 10.f * sin(y * 0.3f);
		yrot += 15.f * sin(y * 0.5f);
		zrot += 17.f * sin(y * 0.4f);
	}
}

bool Tentacles::Init() 
{
	scene = new Scene();

	oldObject = genPilar(TENTACLES_SEGMENTSX, TENTACLES_SEGMENTSY, TENTACLES_RADIUS, TENTACLES_LENGTH, "texture04");
	
	int objIndex = 0;

	for (int i = 0; i < TENTACLES_NRTENTACLES; i++)
	{
		scene->addObject(genPilar(TENTACLES_SEGMENTSX, TENTACLES_SEGMENTSY, TENTACLES_RADIUS, TENTACLES_LENGTH, "texture04"));
		scene->object[objIndex]->finish(true);
	
		objIndex++;
	}



	return true;
}   

void Tentacles::Do(dword timer) 
{	

	bendTentacle(scene->object[0], oldObject, timer);


	scene->camera->setTar(0, 20, 0);
	scene->camera->setLoc(sin(timer / 3000.f) * 40.f, 35, cos(timer / 3000.f) * 50.f);


	scene->render();
}

void Tentacles::DeInit() 
{
	delete scene;
//	delete oldObject;
}
