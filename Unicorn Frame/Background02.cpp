#include "Main.h"

#define BACKGROUND2_NRPLANES 10

float bgrnd2Timers[BACKGROUND2_NRPLANES][4];

bool Background02::Init() 
{
	scene = new Scene();

	for (int i = 0; i < BACKGROUND2_NRPLANES; i++)
	{
		scene->addObject(genPlane(1, 1, "env3"));
		scene->object[i]->cull = CULL_NONE;
		scene->object[i]->transMode = UNI_ADD;
		scene->object[i]->scaleUV(1, 8);
		for (int j = 0; j < scene->object[i]->nrVertices; j++)
		{
			scene->object[i]->vertex[j].x *= 2.f;
			scene->object[i]->vertex[j].z *= 8.f;
		}
		scene->object[i]->finish(false);
		bgrnd2Timers[i][0] = uniRand() * 10000.f;
		bgrnd2Timers[i][1] = uniRand() * 3000.f + 10000.f;
		bgrnd2Timers[i][2] = uniRand() * 3000.f + 10000.f;
		bgrnd2Timers[i][3] = uniRand() *  500.f +  1000.f;
	}


	return true;
}   

void Background02::Do(dword timer) 
{	
	for (int i = 0; i < BACKGROUND2_NRPLANES; i++)
	{
		scene->object[i]->setRot(sin((timer + bgrnd2Timers[i][0]) / bgrnd2Timers[i][1]) * 360.f,
			                     cos((timer + bgrnd2Timers[i][0]) / bgrnd2Timers[i][1]) * 360.f,
								 sin((timer + bgrnd2Timers[i][0]) / bgrnd2Timers[i][2]) * 360.f);
		scene->object[i]->setLoc(sin((timer + bgrnd2Timers[i][0]) / bgrnd2Timers[i][3]) * 1.f,
			                     cos((timer + bgrnd2Timers[i][2]) / bgrnd2Timers[i][3]) * 1.f,
								 sin((timer + bgrnd2Timers[i][1]) / bgrnd2Timers[i][3]) * 1.f);
		float fade = 0.07f;
		scene->object[i]->setAmbient(fade, fade, fade, 1);
	}

	blurRadial->Open();

	scene->render();

	blurRadial->Close();
	blurRadial->Do(timer);
}

void Background02::DeInit() 
{
	delete scene;
}
