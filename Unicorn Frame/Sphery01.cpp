#include "Main.h"

bool Sphery01::Init() 
{
	scene = new Scene();

	scene->addObject(genSphere(26, 2.f, "sphery01"));
	scene->object[0]->shade = SHADE_ENVMAP;
	scene->object[0]->optimize(0);

	oldObject = genSphere(26, 2.f, NULL);

	scene->object[0]->finish(true);

	scene->ambient = 0x0;

	scene->addLight(new Light(1.3f, 1.3f, 1.3f, 1));

	currentCam = 1;
	camTimer1  = 0;

	ShowLoader(1.0f);


	return true;
}   

void Sphery01::Do(dword timer) 
{	
	//timer += 25000;
	Vertex *src = oldObject->vertex;
	for (int i = 0; i < oldObject->nrVertices; i++)
	{
		float x = src[i].x;
		float y = src[i].y;
		float z = src[i].z;

		float scale;

//		scale = 1 + sin((x * 3.f) + (timer / 1100.f)) * 0.4f + cos((z * 4.5f) - (timer / 2000.f)) * 0.4f;
		scale = 1 + sin((x * 3.f) + (timer / 550.f)) * 0.4f + cos((z * 4.5f) - (timer / 1000.f)) * 0.4f;
		if (scale > 0)
		{
		}
		else
			scale = 1;

		x *= scale;
		y *= scale;
		z *= scale;

		float dist = sqrt((x * x) + (y * y) + (z * z));

		D3DMATRIX rotate;
		uniMatrixRotate(rotate, dist, dist * 1.2f, dist * 0.7f);

		D3DVECTOR point = {x, y, z};
		uniMatrixMultiplyVector(point, point, rotate);

		scene->object[0]->vertex[i].x = point.x;
		scene->object[0]->vertex[i].y = point.y;
		scene->object[0]->vertex[i].z = point.z;		
	}

	scene->light[0]->setLoc(0, 0, -5);

	scene->camera->setLoc(sin(timer / 6000.f) * 5.f, 0, cos(timer / 6000.f) * 3.f);

	scene->light[0]->setLoc(sin(timer / 6000.f) * 5.f, 1, cos(timer / 6000.f) * 3.f);


	float timerTemp, x, y;
	switch (currentCam)
	{
	case 1:
		timerTemp = timer - camTimer1;
		if (timerTemp < 2000)
		{
			x = 4.f - (sin(timerTemp * _PI / 4000.f) * 4.f);
			y = 4.f - (sin(timerTemp * _PI / 4000.f) * 4.f);

			scene->object[0]->setLoc(x, y, 0);
		}
		break;
	case 2:
		timerTemp = timer - camTimer1;
		if (timerTemp < 2000)
		{
			x = 4.f - (cos(timerTemp * _PI / 4000.f) * 4.f);
			y = 4.f - (cos(timerTemp * _PI / 4000.f) * 4.f);

			scene->object[0]->setLoc(x, y, 0);
		}
		break;
	}

	scene->render();


	SpriteDrawer *sd = uniSystem->spriteDrawer;


	uniSetTexture("distort5");

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
	
	float uadd = sin(timer / 10000.f) * 1.f;
	float vadd = cos(timer / 10000.f) * 1.f;

	int pos, row, sync;

	fmodGetInfo(pos, row, sync);

	int fade = 80 - row * 7;
	if (fade > 0)
	{
		int c = (fade << 16) | (fade << 8) | fade;
		sd->Start(UNI_ADD);
		sd->Draw2D(x1, y1, 0 + uadd, 0 + vadd, c,
				   x2, y1, 8 + uadd, 0 + vadd, c,
				   x1, y2, 0 + uadd, 8 + vadd, c,
				   x2, y2, 8 + uadd, 8 + vadd, c);
		sd->Stop();
	}
}

void Sphery01::Receive(dword Message, dword timer)
{
	int i;
	switch (Message & 255)
	{
	case 2:
		currentCam = 2;
		camTimer1  = timer;
		break;
	}
}


void Sphery01::DeInit() 
{
	delete scene;
	delete oldObject;
}
