#include "Main.h"

#define KOKKELS_SEGMENTSX     5
#define KOKKELS_SEGMENTSY    17
#define KOKKELS_RADIUS     0.3f
#define KOKKELS_LENGTH       20

#define KOKKELS_NRCLAWS	     15
#define KOKKELS_NRKOKKELS     7

#define KOKKELS_NRSTONES      50

#define KOKKELS_NRBUBBLES    8

float randTab[256];

Object *makeShell()
{
	Object *obj = genPilar(25, 10, 3.f, 6.f, "texture07");

	int index = 0;

	for (int y = 0; y < 11; y++)
	{
		float radius = 2.5f - (y / 5.4f);
		for (int x = 0; x < 26; x++)
		{
			if (y > 8)
			{
				obj->vertex[index].x *= radius;
				obj->vertex[index].z *= radius;
			}
			else
			{
				obj->vertex[index].x *= radius + (0.7f - ((0.7f / 8.f) * y));
				obj->vertex[index].z *= radius + (0.7f - ((0.7f / 8.f) * y));
			}
			index++;
		}
	}

	return obj;
}

void bendTube(Object *tar, Object *src, dword timer)
{
	float xrot = 0;
	float yrot = 0;
	float zrot = 0;

	float curLocX = 0;
	float curLocY = 0;
	float curLocZ = 0;

	float time = timer;

	float xscale = 1.f;
	float yscale = 1.f;
	float zscale = 1.f;

	if (time < 700)
	{
		yscale = 0.3f + sin(time * (_PI / 1400.f)) * 0.7f;
	}
	else if (time < 850)
	{
		yscale = 1.0f;
	}
	else if (time < 1000)
	{
		yscale = 1.0f - ((time - 850) / 214.3f);
	}
	else
	{
		yscale = 0.3f;
	}

	xscale = zscale = yscale;

	int index = 0;
	for (int y = 0; y < KOKKELS_SEGMENTSY + 1; y++)
	{
		D3DMATRIX rotate;
		uniMatrixRotate(rotate, xrot * _PI / 180.f, yrot * _PI / 180.f, zrot * _PI / 180.f);
		float radius = 1.0f - (y / 19.f);
		for (int x = 0; x < KOKKELS_SEGMENTSX + 1; x++)
		{
			float x = src->vertex[index].x * radius;
			float y = 0;
			float z = src->vertex[index].z * radius;

			D3DVECTOR point = {x, y, z};
			uniMatrixMultiplyVector(point, point, rotate);

			x = point.x;
			y = point.y;
			z = point.z;

			tar->vertex[index].x = x + (curLocX * xscale);
			tar->vertex[index].y = y + (curLocY * yscale);
			tar->vertex[index].z = z + (curLocZ * zscale);

			index++;
		}

		D3DVECTOR dir = {0, 1.5f, 0};
		D3DMATRIX dirMatrix;
		uniMatrixRotate(dirMatrix, xrot * _PI / 180.f, yrot * _PI / 180.f,  zrot * _PI / 180.f);
		uniMatrixMultiplyVector(dir, dir, dirMatrix);

		curLocX += dir.x; 
		curLocY += dir.y; 
		curLocZ += dir.z; 

		// benden van tentacle
		if (y > 0)
		{
			if (time < 700)			// step 0
			{
			}
			else if (time < 850)	// step 1
			{
				time = 700 - ((time - 700) * 4.667f);
			}
			else					// step 2
			{
				time = 0;
			}

			float bend  = 2.4f;
			float mult2 = 20.f + (time / 100.f);
			float mult1 = 4.f + cos(y / 10.0f) * (sin(time / 440.f) * 17.f);
			if (y < 10)
				zrot += sin(y / bend) * mult1;
			else
				zrot += sin(y / bend) * (mult1 + 5.f);
		}
	}
}


void copyTube(Object *tar, Object *src)
{
	memcpy(tar->vertex, src->vertex, sizeof(Vertex) * tar->nrVertices);
}

bool Kokkels::Init() 
{
	scene = new Scene();

	tube = genPilar(KOKKELS_SEGMENTSX, KOKKELS_SEGMENTSY, KOKKELS_RADIUS, KOKKELS_LENGTH, NULL);

	int objIndex = 0;


	// LANDSCAPE GENERATION
	scene->addObject(genPlane(30, 300, "sand01"));
	int index = 0;
	Object *plane = scene->object[objIndex];
	plane->scaleUV(10, 10);
	for (int y = 0; y < 31; y++) 
	{
		for (int x = 0; x < 31; x++)
		{
			float x = plane->vertex[index].x;
			float y = plane->vertex[index].y;
			float z = plane->vertex[index].z;

			float dist = sqrt((x * x) + (z * z));

			y = (sin(x / 23.f) + cos(z / 21.f) + sin((x * y) / 24.f)) * (dist / 10.f);
			if (y < 0)
				y = 0;

			if (dist > 70)
				y += ((dist - 70) / 2.f) + (uniRand() * 2.f);

			plane->vertex[index].y = y;
			index++;
		}
	}
	plane->finish(false);
	objIndex++;

	
	// KOKKELS GENERATION
	float pos[KOKKELS_NRKOKKELS][3] = {{-20, -2, 0}, {20, 1, 10}, {-40, -2, -30}, {10, -2, 50}, {-10, -2, 30}, {0, -2, -40}, { -50, -2, 0}};

	for (int j = 0; j < KOKKELS_NRKOKKELS; j++)
	{
		for (int i = 0; i < KOKKELS_NRCLAWS; i++)
		{
			scene->addObject(genPilar(KOKKELS_SEGMENTSX, KOKKELS_SEGMENTSY, KOKKELS_RADIUS, KOKKELS_LENGTH, "texture06"));
			scene->object[objIndex]->scaleUV(1, 5);
			scene->object[objIndex]->finish(true);
	
			scene->object[objIndex]->setRot(0, (360.f / KOKKELS_NRCLAWS) * i, 0);
			scene->object[objIndex]->setLoc((sin((i * _PI2 / KOKKELS_NRCLAWS) - (_PI / 2.f)) * 0.3f) + pos[j][0], 
				                            pos[j][1],
									        (cos((i * _PI2 / KOKKELS_NRCLAWS) - (_PI / 2.f)) * 0.3f) + pos[j][2]);
//			scene->object[objIndex]->transMode = UNI_ADD;
//			scene->object[objIndex]->setDiffuse(1, 1, 1, 0.8f);
			objIndex++;

		}

		scene->addObject(makeShell());
		scene->object[objIndex]->setLoc(pos[j][0], pos[j][1] - 1, pos[j][2]);
		scene->object[objIndex]->cull = CULL_NONE;
		scene->object[objIndex]->finish(false);
		objIndex++;

		scene->addObject(genSphere(8, 7.43f, "texture07"));
		scene->object[objIndex]->setScl(1, 0.1f, 1);
		scene->object[objIndex]->setLoc(pos[j][0], pos[j][1] + 3.2f, pos[j][2]);
		scene->object[objIndex]->setDiffuse(0.3f, 0.3f, 0.3f, 1);
		scene->object[objIndex]->finish(false);
		objIndex++;

				
#ifdef KOKKELS_NRBUBBLES
	SmokeSys *smoke = new SmokeSys(KOKKELS_NRBUBBLES, 0.9f, 0xffffff,  4 + uniRand() * 2.f, UniVector(pos[j][0], pos[j][1], pos[j][2]), "bubble01");
	smoke->defRandVel.x = 0.4f;
	smoke->defRandVel.y = 1.2f;
	smoke->defRandVel.z = 1.2f;
	smoke->Reset();
	scene->addSpriteGroup(smoke);
#endif

	}

	SmokeSys *smoke = new SmokeSys(2000, 0.5f, 0x101010,  25, UniVector(0, -100, 0), "little");
	smoke->acc.y = 0;
	smoke->defVel.y = -3;
	smoke->defRandVel.y = 0;
	for (int i = 0; i < smoke->nrSprites; i++)
	{
		smoke->sprite[i].setLoc(uniRand() * 250.f - 125.f,
			                    uniRand() * 70.f,
								uniRand() * 250.f - 125.f);
	}
	scene->addSpriteGroup(smoke);

	for (i = 0; i < 256; i++)
	{
		randTab[i] = uniRand() * 0.2f + 0.8f;
	}


	scene->fog      = true;
	scene->fogEnd   = 200.f;
	scene->fogStart =  40.f;
	scene->fogColor = 0x153560;

	scene->ambient = 0;

	scene->addLight(new Light(1.0f, 1.0f, 1.0f, 1));
	scene->addLight(new Light(1.0f, 1.0f, 1.0f, 1));

	currentCam = 1;

	morphing = false;

	camTimer1  = 0;
	camTimer2  = 0;
	morphTimer = 0;


	return true;
}   

bool blablablalblablablalabla = true;

float hihiTime = 0;

void Kokkels::Do(dword timer) 
{	
	clearColor = scene->fogColor;

	if (blablablalblablablalabla)
	{
		uniClearBuffers(UNI_BBF, clearColor);
		blablablalblablablalabla = false;
	}


	float times[KOKKELS_NRKOKKELS];

	if (!morphing)
	{
		times[0] = 0;
		times[1] = 0;
		times[2] = 0;
		times[3] = 0;
		times[4] = 0;
		times[5] = 0;
		times[6] = 0;

		morphTimer = timer;
	}
	else
	{
		int timerTemper;

		timerTemper = timer - morphTimer - 202;
		if (timerTemper < 0)
			times[0] = 0;
		else
			times[0] = (timerTemper % 2400) / 2.0f;

		timerTemper = timer - morphTimer - 0;
		if (timerTemper < 0)
			times[1] = 0;
		else
			times[1] = (timerTemper % 3700) / 2.0f;

		timerTemper = timer - morphTimer - 390;
		if (timerTemper < 0)
			times[2] = 0;
		else
			times[2] = (timerTemper % 2600) / 2.0f;

		timerTemper = timer - morphTimer - 900;
		if (timerTemper < 0)
			times[3] = 0;
		else
			times[3] = (timerTemper % 3420) / 2.0f;

		timerTemper = timer - morphTimer - 800;
		if (timerTemper < 0)
			times[4] = 0;
		else
			times[4] = (timerTemper % 2100) / 2.0f;

		timerTemper = timer - morphTimer - 300;
		if (timerTemper < 0)
			times[5] = 0;
		else
			times[5] = (timerTemper % 3500) / 2.0f;

		timerTemper = timer - morphTimer - 500;
		if (timerTemper < 0)
			times[6] = 0;
		else
			times[6] = (timerTemper % 3800) / 2.0f;
	}

//		if (times
/*
		int timerTemper = timer - morphTimer;
		times[0][0] = ((timerTemp % 2500) / 2.0f); 
		times[1][0] = ((timerTemp % 4000) / 2.3f); 
		times[2][0] = ((timerTemp % 2400) / 2.1f); 
		times[3][0] = ((timerTemp % 3420) / 2.2f); 
		times[4][0] = ((timerTemp % 3800) / 2.1f); 
		times[5][0] = ((timerTemp % 3600) / 2.1f);
		times[6][0] = ((timerTemp % 3800) / 2.1f);
*/
//	}

	int objIndex = 1;

	for (int j = 0; j < KOKKELS_NRKOKKELS; j++)
	{
		bendTube(scene->object[objIndex], tube, times[j]);

		objIndex++;

		for (int i = 1; i < KOKKELS_NRCLAWS; i++)
		{
			copyTube(scene->object[objIndex], scene->object[objIndex - i]);
			objIndex++;
		}

		objIndex++;
		objIndex++;

#ifdef KOKKELS_NRBUBBLES
	SmokeSys *smoke = (SmokeSys*) scene->spriteGroup[j];
	smoke->Update(timer);
#endif

	}

	SmokeSys *smoke = (SmokeSys*) scene->spriteGroup[scene->nrSpriteGroups - 1];
//	smoke->basePos = UniVector(sin(timer / 10.f) * 90.f, 80, sin(timer / 13.5f) * 90.f);
//	smoke->Update(timer);
	float timerPPP = timer - hihiTime;
	for (int i = 0; i < smoke->nrSprites; i++)
	{
		smoke->sprite[i].x += (timerPPP * (randTab[i & 255] - 0.9f)) * 0.001f;
		smoke->sprite[i].y += 0;
		smoke->sprite[i].z += (timerPPP * (randTab[i & 255] - 0.9f)) * 0.001f;
	}

	hihiTime = timer;
		
	scene->camera->setTar(0, 15, 0);
	scene->camera->setLoc(sin(timer / 4000.f) * 75.f, 21 + sin(timer / 3000.f) * 8.f, cos(timer / 4500.f) * 70.f);
	scene->camera->setRoll(sin(timer / 3000.f) * cos(timer / 2400.f) * 5.f);

	scene->light[0]->setLoc(sin(timer / 1800.f) * 80.f, 25, sin(timer / 2400.f) * 80.f);
	scene->light[1]->setLoc(cos(timer / 2000.f) * 80.f, 30, sin(timer / 1800.f) * 80.f);

	//morphing = true;
	float timerTemp, timerTemp2;
	switch (currentCam)
	{
	case 1:
		timerTemp = timer - camTimer1 - 8000;
		scene->camera->setTar(0, 15, 0);
		scene->camera->setLoc(sin(timerTemp / 15000.f / 2.67f) * 150.f, 55 + sin(timerTemp / 7000.f / 2.67f) * 8.f, cos(timerTemp / 15000.f / 2.67f) * 150.f);
		scene->camera->setRoll(sin(timerTemp / 8000.f) * cos(timerTemp / 6400.f) * 5.f);
		break;
	case 2:
		timerTemp = timer - camTimer1 + 2400;
		scene->camera->setLoc((-timerTemp / 200.f) + 75, 30, (-timerTemp / 200.f) + 75);
		scene->camera->setTar((-timerTemp / 200.f) + 40,  8, (-timerTemp / 200.f) + 40);
		scene->camera->setRoll(0);
		if ((timer - camTimer1) < 3900)
		{
			float amp = 0.2f - ((timer - camTimer1) / 19500.f);
			scene->camera->fov = 0.8f + (sin(fmod((timer - camTimer1), 650) * (_PI2 / 650.f)) * amp);
		}
		else
		{
			scene->camera->fov = 0.80;
		}

		break;
	case 3:
		timerTemp = timer - camTimer1;
		scene->camera->setTar(-20, 8, 0);
		scene->camera->setLoc(sin(-timerTemp / 10000.f) * 80.f, 30.f, cos(-timerTemp / 10000.f) * 60.f);
		scene->camera->setRoll(20);

		if ((timer - camTimer1) < 3900)
		{
			float amp = 0.2f - ((timer - camTimer1) / 19500.f);
			scene->camera->fov = 0.8f + (sin(fmod((timer - camTimer1), 650) * (_PI2 / 650.f)) * amp);
		}
		else
		{
			scene->camera->fov = 0.80;
		}

		break;
	case 4:
		timerTemp = timer - camTimer1;
		scene->camera->setTar(-20, 10, 0);
		scene->camera->setLoc(sin(timerTemp / 50000.f) * 20.f - 20.f, 1.f, cos(timerTemp / 50000.f) * 20.f);
		scene->camera->setRoll(20);

		if ((timer - camTimer1) < 3900)
		{
			float amp = 0.2f - ((timer - camTimer1) / 19500.f);
			scene->camera->fov = 0.8f + (sin(fmod((timer - camTimer1), 650) * (_PI2 / 650.f)) * amp);
		}
		else
		{
			scene->camera->fov = 0.80;
		}

		break;
	}

	uniSetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

	scene->render();

	uniSetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

	uniSetTexture(NULL);
	uniSetTexture(NULL, 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
 
    int c = 0x153560;
	int a = 0xff - (timer / 8.f);
	if (a >= 0) {
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0, 0, c,
				  x2, y1, 0, 0, c,
  				  x1, y2, 0, 0, c,
				  x2, y2, 0, 0, c);
		sd->Stop();
	}
}

void Kokkels::Receive(dword Message, dword timer)
{
	int i;
	switch (Message & 255)
	{
	case 1:
		camTimer1 = timer;
		currentCam = 1;
		break;
	case 2:
		camTimer1 = timer;
		currentCam = 2;
		break;
	case 3:
		camTimer1 = timer;
		currentCam = 3;
		break;
	case 4:
		camTimer1 = timer;
		currentCam = 4;
		break;
	case 20:
		morphing = true;
		break;
	}
}

void Kokkels::DeInit() 
{
	delete scene;
	delete tube;
}
