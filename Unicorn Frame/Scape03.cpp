#include "Main.h"

struct partData
{
	float time;
	float eindX;
	float eindY;
	float eindZ;
	bool  opzij;
};

#define NRPARTS 5000
#define PARTSTARTX (float)   30
#define PARTSTARTZ (float)   -25

#define SCAPE03_NRSMOKEPARTS 50
#define SCAPE03_NRFIREPARTS  40

#define VULCAN_SKY

#ifdef NRPARTS
partData parts[NRPARTS];
#endif

//#define DO_SPIKEY

#ifdef DO_SPIKEY
Object *oldPlane;
#endif

bool Scape03::Init() 
{
	nrSegmentsX = 127;
	nrSegmentsZ = 127;
	left   = -150;
	top    = -150;
	right  =  150;
	bottom =  150;

	scene = new Scene();

	scene->addObject(genPlane(nrSegmentsX, 300, "vulcanfinal"));

	CreateShape(NULL, scene->object[0]->vertex, 4.f, 0.8f, 0);

	Vertex *vertex = scene->object[0]->vertex;

	// vulkaan creeeren in het midden
	for (int y = 0; y < nrSegmentsZ + 1; y++)
	{
		for (int x = 0; x < nrSegmentsX + 1; x++)
		{
			int index = (y * (nrSegmentsX + 1)) + x;

			float vx = vertex[index].x;
			float vy = vertex[index].y;
			float vz = vertex[index].z;

			float x = vx - 30.f;
			float z = vz + 25.f;
			

			float length = sqrt((x * x) + (z * z));
			float amp = 40.f;
			if (length < 25.f)
			{
				if (length > 10)
				{
					vy += sin((25.f - length) * (_PI / 15.f)) * 10.f;
				}
				else
				{
					vy -= 20.f;
				}
			}


			vertex[index].x = vx;
			vertex[index].y = vy;
			vertex[index].z = vz;
		}
	}

	int width, height;	
//	dword *grond1 = loadATSFromMem(ATS_GROUND2, width, height);
//	dword *grond2 = loadATSFromMem(ATS_GROUND1, width, height);
//	dword *grond1 = loadATSFromMem(ATS_VULCANGROUND1, width, height);
//	dword *grond2 = loadATSFromMem(ATS_VULCANGROUND2, width, height);
//	dword *grond3 = loadATSFromMem(ATS_GROUND3, width, height);
//	dword *grond4 = loadATSFromMem(ATS_WIT,   width, height);

	dword *grond1 = extraTextures[5];//loadATSFromMem(ATS_VULCANGROUND1, width, height);
	dword *grond2 = extraTextures[4];//loadATSFromMem(ATS_VULCANGROUND2, width, height);
	dword *grond3 = extraTextures[0];//loadATSFromMem(ATS_GROUND3, width, height);
	dword *grond4 = extraTextures[3];//loadATSFromMem(ATS_WIT,   width, height);

	dword *textureFinal = CreateTexture(1024, 1024, grond1, grond2, grond3, grond4, -6.f, 1.8f);

	uniTextureCreate("vulcanfinal", textureFinal, 1024, 1024, UNI_MIPMAP);

//	delete [] grond1;
//	delete [] grond2;
//	delete [] grond3;
//	delete [] grond4;

	scene->fog = true;
	scene->fogColor = 0x000000;
	scene->fogStart = 50;
	scene->fogEnd = 200;//150;
	scene->fogSprites = true;

//	scene->camera->farPlane = 220;//170;

//	scene->ambient = 0x202020;
	scene->ambient = 0x404040;

	scene->addLight(new Light(1, 1, 1, 1));

	scene->addLight(new Light(0.1f, 0, 0, 1));
	scene->light[1]->setLoc(PARTSTARTX, 60, PARTSTARTZ);
	scene->light[1]->setRange(80);


#ifdef NRPARTS
	scene->addSpriteGroup(new SpriteGroup(NRPARTS, 0.7f, 0xffffffff, "vulcansprite"));

	for (int i = 0; i < NRPARTS; i++)
	{
		float t = rand()%360;
		float x = sin((t * _PI) / 180.f) * 200.f;
		float y = 0;
		float z = cos((t * _PI) / 180.f) * 200.f;
		parts[i].eindX = x;
		parts[i].eindY = y;
		parts[i].eindZ = z;
		parts[i].time = rand()%12000;
		parts[i].opzij = false;
	}
#endif

#ifdef SCAPE03_NRSMOKEPARTS
	scene->addSpriteGroup(new SmokeSys(SCAPE03_NRSMOKEPARTS, 15.7f, 0xcccccccc,   10, UniVector(PARTSTARTX, 20, PARTSTARTZ), "smoke01"));
	scene->addSpriteGroup(new SmokeSys(SCAPE03_NRFIREPARTS,  5.7f, 0xffffffff, 2.8f, UniVector(PARTSTARTX, 20, PARTSTARTZ), "vulcansprite"));
#endif

#ifdef DO_SPIKEY
	oldPlane = genPlane(nrSegmentsX, 300, "vulcanfinal");
	for (i = 0; i < oldPlane->nrVertices; i++)
	{
		oldPlane->vertex[i] = scene->object[0]->vertex[i];
	}

	scene->object[0]->finish(true);
#endif

	currentCam = 1;
	camTimer1  = 0;
	camTimer2  = 0;
	camTimer3  = camTimer4 = camTimer5 = -999999;

#ifdef VULCAN_SKY
	scene2 = new Scene();
	scene2->addObject(genSphere(4, 2000.f, "vulcanback"));
	scene2->object[0]->cull = CULL_ACW;
	scene2->camera->farPlane = 5000;
	scene2->object[0]->scaleUV(2, 2);
#endif

	scene->object[0]->finish(false);
	scene2->object[0]->finish(false);

	ShowLoader(0.8f);

	return true;
}   


bool scape03RunParticles = false;

void Scape03::Do(dword timer) 
{
	uniSetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

#ifdef SCAPE03_NRSMOKEPARTS
	SmokeSys *smoke = (SmokeSys*) scene->spriteGroup[1];
	smoke->Update(timer);

	smoke = (SmokeSys*) scene->spriteGroup[2];
	smoke->Update(timer);
#endif

#ifdef NRPARTS
	if (scape03RunParticles)
	{
		for (int i = 0; i < NRPARTS; i++)
		{
			float t = parts[i].time;
			float x, y, z;
			float height;
			if (!parts[i].opzij)
			{
				if (timer >= parts[i].time)
				{
					parts[i].opzij = true;
					parts[i].time = timer;
				}
			}

			if (parts[i].opzij)
			{/*
				float posInTime = (timer - parts[i].time) / 12000.f;
				x = parts[i].eindX * posInTime;
				z = parts[i].eindZ * posInTime;
				float l = sqrt((x * x) + (z * z));
				x += PARTSTARTX;
				z += PARTSTARTZ;
				GetHeightAtPosition(scene->object[0]->vertex, x, z, y);
				if (l < 35.f)
				{
					y += 2.f + sin((35.f - l) * (_PI / 35.f)) * 1.5f;
				}
				else
				{
					y += 2.0f;
				}
				if (posInTime > 1.0f)
				{
					parts[i].time = timer + rand()%2000;
					parts[i].opzij = false;
				}
				*/
				///*
				float posInTime = 1.0f - ((timer - parts[i].time) / 12000.f);
				x = parts[i].eindX * posInTime;
				z = parts[i].eindZ * posInTime;
				float l = sqrt((x * x) + (z * z));
				x += PARTSTARTX;
				z += PARTSTARTZ;
				GetHeightAtPosition(scene->object[0]->vertex, x, z, y);
				if (l < 35.f)
				{
					y += 2.f + sin((35.f - l) * (_PI / 35.f)) * 1.5f;
				}
				else
				{
					y += 2.0f;
				}
				if (posInTime < 0.0f)
				{
					parts[i].time = timer + rand()%2000;
					parts[i].opzij = false;
				}
				//*/
			}
			scene->spriteGroup[0]->sprite[i].setLoc(x, y, z);
		}
	}
#endif
/*
	float x = sin(timer / 10000.f) * 120.f;
	float y = 30.f + (sin(timer / 12000.f) * 2.f);
	float z = cos(timer / 7000.f) * 120.f;

	float height;
	GetHeightAtPosition(scene->object[0]->vertex, x, z, height);

	if (y < height + 3.5f)
		y = height + 3.5f;

	scene->camera->setLoc(x, y, z);

	scene->camera->setRoll((sin(timer / 3000.f) + cos(timer / 4000.f)) * 5.f);

	timer += 11000;
	
	scene->camera->setTar(sin(timer / 10000.f) * 80.f, 10 + sin(timer / 4000.f) * 3.f, cos(timer / 7000.f) * 80.f);

*/

	float x, y, z, timerTemp;

	switch (currentCam)
	{
	case 1:
		timerTemp = timer - camTimer1;

		x = -sin(timerTemp / 10000.f) * 100.f;
		z = -cos(timerTemp / 10000.f) * 70.f;
		y = 80.f - sin(timerTemp / 6000.f) * 35.f;
		if (timerTemp > 9424.5f) 
			y = 80 - 35.f;

		scene->camera->setLoc(x, y, z);

		timerTemp += 10000;

		x = -sin(timerTemp / 10000.f) * (80.f - sin(timerTemp / 10000.f) * 40.f);
		z = -cos(timerTemp / 10000.f) * (80.f - sin(timerTemp / 10000.f) * 40.f);
		y = 35.f;

		scene->camera->setTar(x, y, z);
		break;
	case 2:
		timerTemp  = timer - 25000.f - camTimer1;

		x = sin(timerTemp / 10000.f) * 100.f;
		z = cos(timerTemp / 10000.f) * 100.f;
		y = 41.f - sin((timerTemp + 8000) / 10000.f) * 10.f;
		if (timerTemp > 9424.5f) 
			y = 41 - 10.f;

		scene->camera->setLoc(x, y, z);

		timerTemp += 10000;

		x = sin(timerTemp / 10000.f) * (50.f - sin(timerTemp / 20000.f) * 40.f);
		z = cos(timerTemp / 10000.f) * (50.f - sin(timerTemp / 20000.f) * 40.f);
		y = 35.f;

		scene->camera->setTar(x, y, z);
		break;
	case 3:
		timerTemp = timer - camTimer1;

//		x = sin((timerTemp - 10000) / 7000.f) *  80.f + PARTSTARTX;
//		z = cos((timerTemp - 10000) / 7000.f) * 120.f + PARTSTARTZ;
		x = sin((timerTemp - 20000) / 7000.f) *  80.f + PARTSTARTX;
		z = cos((timerTemp - 20000) / 7000.f) * 120.f + PARTSTARTZ;

		y = 40.f;

		scene->camera->setLoc(x, y, z);

		scene->camera->setRoll(sin(timerTemp / 7000.f) * 15.f);

		x = PARTSTARTX;
		z = PARTSTARTZ;
		y = 30.f;

		scene->camera->setTar(x, y, z);

		break;
	case 4:
		timerTemp = timer - camTimer1;

//		x = sin((timerTemp + 6500) / -5500.f) * 165.f + PARTSTARTX;
//		z = cos((timerTemp + 6500) / -5500.f) * 120.f + PARTSTARTZ;
		x = sin((timerTemp + 16500) / -5500.f) * 120.f + PARTSTARTX;
		z = cos((timerTemp + 16500) / -5500.f) * 180.f + PARTSTARTZ;
		y = 75.f;

		scene->camera->setLoc(x, y, z);

		scene->camera->setRoll(sin(timerTemp / 7000.f) * -10.f);

		x = PARTSTARTX;
		z = PARTSTARTZ;
		y = 30.f;

		scene->camera->setTar(x, y, z);
		break;

	}

	scene->light[0]->setLoc(scene->camera->xLoc, 
		                    scene->camera->yLoc, 
							scene->camera->zLoc);
#ifdef VULCAN_SKY
	scene2->camera->setLoc(scene->camera->xLoc,
						   scene->camera->yLoc,
						   scene->camera->zLoc);

	scene2->camera->setTar(scene->camera->xTar,
						   scene->camera->yTar,
						   scene->camera->zTar);

	scene2->camera->setRoll(scene->camera->roll);

	scene2->render();
#endif

	scene->render();


	uniSetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

	uniSetTexture(NULL);
	uniSetTexture(NULL, 1);

	bool bFade = false;
    int c = 0x000000;
	int fade = 0x0;

	if ((timer - camTimer3) < 2850)
	{
		float amp = 0xff - ((timer - camTimer5) / 11.17f);
		fade = 0x00 + (sin(fmod((timer - camTimer5), 950) * (_PI2 / 950.f)) * amp);

		if ((timer - camTimer3) < 950)
		{
			fade = 0x00 + (sin((timer - camTimer3) * (_PI2 / 3800.f)) * 100.f);
		}
		else
		{
			fade = 0x00 + (sin((timer - camTimer3) * (_PI2 / 3800.f)) * 80.f) +
				(cos((timer - camTimer3 - 950) * (_PI / 390.f)) * 20.f);
		}

		bFade = true;
	} 
	else if ((timer - camTimer4) < 400)
	{
		fade = 0x00 + (sin((timer - camTimer4) * (_PI2 / 800.f)) * 100.f);

		c = 0xFF4E00;
 
		bFade = true;
	}
	else if ((timer - camTimer5) < 3250)
	{
		float amp = 0xff - ((timer - camTimer5) / 15.f);
		fade = 0x00 + (sin(fmod((timer - camTimer5), 650) * (_PI2 / 700.f)) * amp);

		c = 0xFF4E00;
 
		bFade = true;
	}

	if (bFade)
	{
		SpriteDrawer *sd = uniSystem->spriteDrawer;

		float x1 = uniSystem->viewportX1;
		float y1 = uniSystem->viewportY1;
		float x2 = uniSystem->viewportX2;
		float y2 = uniSystem->viewportY2;
 
		int a = fade;
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
}

void Scape03::Receive(dword Message, dword timer)
{
	int i;
	switch (Message & 255)
	{

	// cam movement
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
	case 5:
		scape03RunParticles = true;
#ifdef NRPARTS
		for (i = 0; i < NRPARTS; i++)
		{
			parts[i].time = (rand()%12000) + timer;
		}
#endif
		break;

	// fades/flashes
	case 10:
		camTimer3 = timer;
		break;
	case 11:
		camTimer4 = timer;
		break;
	case 12:
		camTimer5 = timer;
		break;
	}
}

void Scape03::DeInit() 
{
	delete scene;
};
