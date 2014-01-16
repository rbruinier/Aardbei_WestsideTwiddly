#include "Main.h"

#define HOSPITAL_RADIUS    5.f
#define HOSPITAL_NRPLANES  6
#define HOSPITAL_NRSEGS    2
#define HOSPITAL_NRSPRITES 400

//#define HOSPITAL_NRSMOKE   100

bool Hospital::Init() 
{
	scene = new Scene();

	scene->addObject(genPlane(HOSPITAL_NRSEGS,  30.f, "hospfloor"));
	scene->addObject(genPlane(HOSPITAL_NRSEGS,  30.f, "hosproof"));

	scene->object[0]->setLoc(0, -5, 0);
	scene->object[1]->setLoc(0,  5, 0);

	scene->object[0]->cull = CULL_NONE;
	scene->object[1]->cull = CULL_NONE;

	scene->object[0]->scaleUV(2, 2);
	scene->object[1]->scaleUV(2, 2);

	scene->object[1]->setRot(180, 0, 0);


	int base = 0;
	int objNr = 0;
	for (int k = 0; k < 4; k++)
	{
		for (int y = 0; y < 2; y++) 
		{
			for (int x = 0; x < HOSPITAL_NRPLANES; x++) 
			{
				bool withSmoke = false;

				char *texture = "hospwall1";
				if (k < 2)
				{
					if ((x + y) & 1)
					{
						texture = "hospwall2";
						withSmoke = true;
					}
				}
				else
				{
					if (!((x + y) & 1))
					{
						texture = "hospwall2";
						withSmoke = true;
					}
				}


				Object *obj = genPlane(HOSPITAL_NRSEGS, HOSPITAL_RADIUS, texture);

				float xx, yy, zz;

				float a = (y * HOSPITAL_RADIUS) - (HOSPITAL_RADIUS / 2.f);
				float b = (x * HOSPITAL_RADIUS) - ((HOSPITAL_NRPLANES - 1) * (HOSPITAL_RADIUS / 2.f));

				switch (k)
				{
					case 0:
						yy = a;
						zz = b;
						xx = -15;						
						obj->setRot(0, 180, 90);
						break;
					case 1:
						yy = a;
						zz = b;
						xx =  15;						
						obj->setRot(0, 0, 90);
						break;
					case 2:
						yy = a;
						zz = -15;
						xx =  b;						
						obj->setRot(90, 0, 0);
						break;
					case 3:
						yy = a;
						zz = 15;
						xx =  b;						
						obj->setRot(90 + 180, 0, 0);
						break;
				}

				if (k >= 2)
				{
					for (int i = 0; i < obj->nrVertices; i++)
					{
						float temp = obj->vertex[i].tu1;
						obj->vertex[i].tu1 = 1.f - obj->vertex[i].tv1;
						obj->vertex[i].tv1 = temp;
					}
				}

				obj->setLoc(xx, yy, zz);

				obj->writeZBuffer = false;

				if (withSmoke)
				{
#ifdef HOSPITAL_NRSMOKE
					SmokeSys *smoke = new SmokeSys(HOSPITAL_NRSMOKE, 6.0f, 0x111111,  4, UniVector(xx * 1.1f, yy, zz * 1.1f), "smoke01");
					smoke->defRandVel.x = 5.0f;
					smoke->defRandVel.y = 1.2f;
					smoke->defRandVel.z = 5.2f;
					smoke->acc.x = 0;
					smoke->acc.y = 0.1f;
					smoke->acc.z = 0;
					smoke->Reset();
					scene->addSpriteGroup(smoke);
#endif
				}

				obj->cull = CULL_NONE;

				scene->addObject(obj);
			}
		}
	}

#ifdef HOSPITAL_NRSPRITES
//	scene2 = new Scene();
	scene->addSpriteGroup(new SpriteGroup(HOSPITAL_NRSPRITES, 0.4f, 0xffffffff, "sprite02"));
	scene->spriteGroup[0]->blend = UNI_ADD;
	for (int i = 0; i < HOSPITAL_NRSPRITES; i++)
	{
		float x = uniRand() * 28.f - 14.0f;
		float y = uniRand() * 9.5f - 4.75f;
		float z = uniRand() * 28.f - 14.0f;
		scene->spriteGroup[0]->sprite[i].setLoc(x, y, z);
	}
#endif

	scene->ambient = 0;

	scene->addLight(new Light(1, 1, 1, 1));

	scene->addLight(new Light(1, 1, 1, 1));

	return true;
}   

float hospitalOldTime = 0;

void Hospital::Do(dword timer) 
{
///*
	// limit frame rate
	while ((uniTimer() - hospitalOldTime) < 25)
	{
	}

	hospitalOldTime = timer;


#ifdef HOSPITAL_NRSMOKE
	for (int i = 0; i < scene->nrSpriteGroups; i++)
	{
		((SmokeSys*)scene->spriteGroup[i])->Update(timer);
	}
#endif



//*/
// normal view
//	scene->camera->setLoc(sin(timer / 5000.f) * 13.f, 0, cos(timer / 6000.f) * 10.f);
//	scene->camera->setTar(0, 0, 0);
/*
	int ord, row, sync;
	fmodGetInfo(ord, row, sync);

	row = row / 16;
	int i;

	switch (row)
	{
		case 0:
		case 1:
		case 2:
			scene->camera->setLoc(sin(timer / 5000.f) * 13.f, 0, cos(timer / 6000.f) * 10.f);
			scene->camera->setTar(sin((timer + 1500) / 5000.f) * 13.f, sin(timer / 600.f) * 2.f, cos((timer + 1500) / 6000.f) * 10.f);
			break;
		case 3:
			scene->camera->setLoc(sin(timer / 500.f) * 13.f, -3, cos(timer / 600.f) * 10.f);
			scene->camera->setTar(sin((timer + 1500) / 500.f) * 13.f, sin(timer / 600.f) * 2.f, cos((timer + 1500) / 600.f) * 10.f);
//			for (i = 0; i < scene->spriteGroup[0]->nrSprites; i++)
//			{
//				float x = sin((timer + (i * 4252)) / (2400.f + (sin(i) * 400.f))) * 14.f;
//				float y = sin((timer + (i * 2552)) / (2000.f + (cos(i + 152) * 500.f))) * 4.75f;
//				float z = cos((timer + (i * 5152)) / (2600.f + (sin(i + 53) * 500.f))) * 14.f;
//				scene->spriteGroup[0]->sprite[i].setLoc(x, y, z);
//			}
			break;
	}

*/	
	
// suicide
//	scene->camera->setLoc(sin(timer / 5000.f) * 13.f, 0, cos(timer / 6000.f) * 10.f);
	//scene->camera->setTar(sin((timer + 1500) / 5000.f) * 13.f, sin(timer / 600.f) * 2.f, cos((timer + 1500) / 6000.f) * 10.f);

// crazy
	scene->camera->setLoc(sin(timer / 500.f) * 13.f, -3, cos(timer / 600.f) * 10.f);
	scene->camera->setTar(sin((timer + 1500) / 500.f) * 13.f, sin(timer / 600.f) * 2.f, cos((timer + 1500) / 600.f) * 10.f);

	scene->camera->roll = sin(timer / 2000.f) * cos((timer + 74574) / 2300.f) * sin((timer + 34256) / 3000.f) * 100.f;

	scene->light[0]->setLoc(sin(timer / 400.f) * 13.f, sin(timer / 600.f) * 3.f, cos(timer / 600.f) * 10.f);
	scene->light[1]->setLoc(cos(timer / 500.f) * 13.f, cos(timer / 500.f) * 3.f, sin(timer / 500.f) * 10.f);
	scene->light[1]->setDiffuse(sin(timer / 25.f) * 0.5f + 0.5f, 
		                        sin(timer / 25.f) * 0.5f + 0.5f, 
								sin(timer / 25.f) * 0.5f + 0.5f, 1);
	blurRadial->Open();
//	blurMotion->Open();

	scene->render();


	blurRadial->Close();
	blurRadial->Do(timer);
//	blurMotion->Close();
//	blurMotion->Do(timer);

/*
#ifdef HOSPITAL_NRSPRITES
	scene2->camera->setLoc(scene->camera->xLoc,
		                   scene->camera->yLoc,
						   scene->camera->zLoc);
	scene2->camera->setTar(scene->camera->xTar,
		                   scene->camera->yTar,
						   scene->camera->zTar);

	scene2->render();
#endif
*/
						   
}

void Hospital::DeInit() 
{
	delete scene;
}
