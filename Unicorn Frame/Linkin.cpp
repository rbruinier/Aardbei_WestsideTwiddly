#include "Main.h"

#define TENT_SEGX 20
#define TENT_SEGY 180

#define LINKIN_MULTIPASS

void genTentacleShape(Vertex *vertex, int nrSegmentsX, int nrSegmentsY)
{
	int timer = uniTimer();

	int index = 0;

	float yMul = 1.f / nrSegmentsY;
	float xMul = (2.f * _PI) / nrSegmentsX;
	float uStp = 1.f / nrSegmentsX;
	float vStp = 1.f / nrSegmentsY;

	for (int y = 0; y < nrSegmentsY + 1; y++)
	{
		float rr = sin((y + (timer / 80.f))* (_PI / TENT_SEGY) * 22.f) * 0.3f + 0.9f;
		float yy = y * yMul;
		float vv = y * vStp;
		for (int x = 0; x < nrSegmentsX + 1; x++)
		{
			float xx, zz, uu;

			xx = sin(x * xMul) * rr;
			zz = cos(x * xMul) * rr;
			uu = x * uStp;

			vertex[index].x = xx;
			vertex[index].y = yy;
			vertex[index].z = zz;
			vertex[index].tu1 = uu;
			vertex[index].tv1 = vv * 20.f;

			index++;
		}
	}
}

Object *genTentacle(int nrSegmentsX, int nrSegmentsY, char *texture)
{
	Object *obj = 
		new Object((nrSegmentsX + 1) * (nrSegmentsY + 1), nrSegmentsX * nrSegmentsY * 2, texture);

	genTentacleShape(obj->vertex, nrSegmentsX, nrSegmentsY);
	
	int index = 0;

	for (int j = 0; j < nrSegmentsY; j++)
	{
		for (int i = 0; i < nrSegmentsX; i++)
		{
			int e = j * (nrSegmentsX + 1);
			int a = e + i;
			int b = a + 1;
			int c = a + nrSegmentsX + 1;
			int d = c + 1;

			obj->face[index].a = a;
			obj->face[index].b = c;
			obj->face[index].c = b;

			index++;

			obj->face[index].a = b;
			obj->face[index].b = c;
			obj->face[index].c = d;

			index++;
		}
	}

	return obj;
}

void Linkin::bendTentacle(Vertex *vertex, int nrSegmentsX, int nrSegmentsY, float radius, float yScale, int timer)
{
//	int timer = uniTimer();

	int index = 0;

	float xRot = 0;
	float yRot = 0;
	float zRot = 0;

	float curLocX = 0;
	float curLocY = 0;
	float curLocZ = 0;
		
	for (int y = 0; y < nrSegmentsY + 1; y++)
	{
		D3DMATRIX rotate;
		uniMatrixRotate(rotate, xRot * _PI / 180.f, yRot * _PI / 180.f, zRot * _PI / 180.f);
		float rr = sin((y + (timer / 80.f))* (_PI / TENT_SEGY) * 22.f) * 0.3f + 0.9f;
		for (int x = 0; x < nrSegmentsX + 1; x++)
		{
			float xx = source[x].x * rr;
			float yy = source[x].y;
			float zz = source[x].z * rr;

			D3DVECTOR point = {xx, yy, zz};
			uniMatrixMultiplyVector(point, point, rotate);

			xx = point.x + curLocX;
			yy = point.y + curLocY;
			zz = point.z + curLocZ;
			
			vertex[index].x = xx + 18.f;
			vertex[index].y = yy;
			vertex[index].z = zz;

			index++;
		}

		D3DVECTOR dir = {0, yScale / nrSegmentsY, 0};
		D3DMATRIX dirMatrix;
		uniMatrixRotate(dirMatrix, xRot * _PI / 180.f, yRot * _PI / 180.f,  zRot * _PI / 180.f);
		uniMatrixMultiplyVector(dir, dir, dirMatrix);

		curLocX += dir.x; 
		curLocY += dir.y; 
		curLocZ += dir.z; 

		zRot += 3.5f;
		yRot += 4;
		zRot += 2.5f;
	}
}


bool Linkin::Init() 
{
	scene = new Scene();

	Object *obj = genTentacle(TENT_SEGX, TENT_SEGY, "sweet1");

	obj->finish(true);
	obj->setSpecular(1, 1, 1, 1, 20.f);

	scene->addObject(obj);

	source = new D3DVECTOR[TENT_SEGX + 1];
	for (int i = 0; i < TENT_SEGX + 1; i++)
	{
		source[i].x = obj->vertex[i].x;
		source[i].y = 0;
		source[i].z = obj->vertex[i].z;
	}

	scene->addLight(new Light(0.8f, 0.8f, 1, 1));
	scene->light[0]->setLoc(50, 50, -40);
	scene->addLight(new Light(0.8f, 0.8f, 1, 1));
	scene->light[1]->setLoc(50, 50, -40);
	scene->ambient = 0x202020;

//	scene->addSpriteGroup(new SpriteGroup(2000, 0.5f, 0x3f1f3f, "sprite01")); 
	scene->addSpriteGroup(new SpriteGroup(2000, 0.5f, 0x4f3f0f, "sprite01")); 

	
//	scene2 = new Scene();
//	scene2->addObject(genSphere(3, 140.f, "sweet04"));
//	scene2->object[0]->cull = CULL_ACW;

//	scene2->object[0]->method = METHOD_NONE;

//	scene2->camera->fov = 0.63f;

	currentCam = 1;
	camTimer4  = 0;

	ugh = false;

	return true;
}   

void Linkin::Do(dword timer) 
{

	camTimer1 = timer - camTimer4;
	camTimer3 = timer - camTimer4;

	if ((camTimer1 > 1450))
	{
		float temp = camTimer1;
		if (!ugh)
		{
			ugh = true;
			camTimer2 = temp;
		}
		camTimer1 = camTimer2 + ((temp - camTimer2) / 10.f);	
		camTimer3 = camTimer2 + ((temp - camTimer2) / 2.5f);	
	}

	bendTentacle(scene->object[0]->vertex, TENT_SEGX, TENT_SEGY, 0.5f, 200.f, camTimer3 * 2.5f);
//	scene->object[0]->setRot((camTimer3 / 9.f) / 2.5f, (camTimer3 / 8.f) / 2.5f, (camTimer3 / 12.f) / 2.5f);

	scene->camera->setTar(0, 0.5f, 0);

	
	switch (currentCam)
	{
	case 1:
		scene->camera->setLoc(cos(camTimer1 / 1000.f) * 140.f, 0, sin(camTimer1 / 1000.f) * 40.f);
		scene->camera->setRoll(cos(camTimer1 / 1000.f) * 200.f);
		break;
	case 2:
		scene->camera->setLoc(sin(camTimer1 / 1000.f) * 40.f, 0, cos(camTimer1 / 1000.f) * 140.f);
		scene->camera->setRoll(cos(camTimer1 / 1000.f) * -200.f);
		break;
	case 3:
		scene->camera->setLoc(20, cos(camTimer1 / 1000.f) * 140.f, sin(camTimer1 / 1000.f) * 40.f);
		scene->camera->setRoll(cos(camTimer1 / 1000.f) * 200.f);
		break;
	case 4:
		scene->camera->setLoc(cos(camTimer1 / 1000.f) * 40.f, 30, cos(camTimer1 / 1000.f) * 140.f);
		scene->camera->setRoll(cos(camTimer1 / 1000.f) * -200.f);
		break;
/*
		scene->camera->setLoc(cos(camTimer1 / 1150.f) * 140.f, 0, sin(camTimer1 / 1150.f) * 40.f);
		scene->camera->setRoll(cos(camTimer1 / 1000.f) * 200.f);
		break;
	case 2:
		scene->camera->setLoc(sin(camTimer1 / 1150.f) * 40.f, 0, cos(camTimer1 / 1150.f) * 140.f);
		scene->camera->setRoll(cos(camTimer1 / 1000.f) * -200.f);
		break;
	case 3:
		scene->camera->setLoc(cos(camTimer1 / 1150.f) * 140.f, 0, sin(camTimer1 / 1150.f) * 40.f);
		scene->camera->setRoll(cos(camTimer1 / 1000.f) * 200.f);
		break;
	case 4:
		scene->camera->setLoc(cos(camTimer1 / 1150.f) * 40.f, 0, cos(camTimer1 / 1150.f) * 140.f);
		scene->camera->setRoll(cos(camTimer1 / 1000.f) * -200.f);
		break;*/
	}
	

	scene->light[0]->setLoc(scene->camera->xLoc,
						    scene->camera->yLoc,
							scene->camera->zLoc);

	for (int i = 0; i < 2000; i++)
	{
		scene->spriteGroup[0]->sprite[i].x = sin((timer + (i * 25236)) / 20000.f) * 100.f;
		scene->spriteGroup[0]->sprite[i].y = sin((timer + (i * 35636)) / 17000.f) * 100.f;
		scene->spriteGroup[0]->sprite[i].z = cos((timer + (i * 52636)) / 15000.f) * 100.f;
	}

//	scene2->camera->setLoc(scene->camera->xLoc,
//					       scene->camera->yLoc,
//						   scene->camera->zLoc);

//	scene2->render();


	scene->render();

#ifdef LINKIN_MULTIPASS
	int timer2 = (timer + 475) % 2025;
	if (timer2 < 1625)
	{
		scene->object[0]->setDiffuse(1, 1, 1, 0);
	}
	else if (timer2 < 1825)
	{
		scene->object[0]->setDiffuse(1, 1, 1, (timer2 - 1625) / 200.f);
	} else
	{
		scene->object[0]->setDiffuse(1, 1, 1, 1.0f - ((timer2 - 1825) / 200.f));
	}

	scene->object[0]->setTexture1("sweet02");
	scene->object[0]->transMode = UNI_ALPHA;

	scene->render();

	scene->object[0]->setTexture1("sweet01");
	scene->object[0]->transMode = UNI_NONE;
	scene->object[0]->setDiffuse(1, 1, 1, 1);
#endif

}

void Linkin::Receive(dword Message, dword timer)
{
	int i;
	switch (Message & 255)
	{
	case 1:
		camTimer4 = timer;
		currentCam = 2;
		ugh = false;
		break;
	case 2:
		camTimer4 = timer;
		currentCam = 2;
		ugh = false;
		break;
	case 3:
		camTimer4 = timer;
		currentCam = 3;
		ugh = false;
		break;
	case 4:
		camTimer4 = timer;
		currentCam = 4;
		ugh = false;
		break;
	}
}


void Linkin::DeInit() 
{
	delete scene;
//	delete scene2;
	delete source;
};
