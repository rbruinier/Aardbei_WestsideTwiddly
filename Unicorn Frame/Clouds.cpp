#include "Main.h"

bool Clouds::Init() 
{
	scene = new Scene();

	for (int i = 0; i < 3; i++)
	{
		scene->addObject(genPlane(1, 10, "sweet04"));
		scene->object[i]->writeZBuffer = false;
		scene->object[i]->setRot(-90, 0, 0);
		scene->object[i]->transMode = UNI_ADD;
		scene->object[i]->setAmbient(0.40f, 0.40f, 0.40f, 1);
		scene->object[i]->scaleUV(4, 4);
		scene->object[i]->finish(false);
	}

	return true;
}

void Clouds::Do(dword timer) 
{
	scene->object[0]->setRot(-90, sin((timer + 34346) / 13000.f) * 180.f, 0);
	scene->object[1]->setRot(-90, sin((timer + 58363) / 13000.f) * 180.f, 0);
	scene->object[2]->setRot(-90, sin((timer + 92346) / 13000.f) * 180.f, 0);

	float dist = -3.0f;

	if (timer < 2000)
	{
		dist = -3.0f + (1.6f * cos(timer * _PI / 4000.f));

	}

	scene->camera->setLoc(sin(timer / 4000.f) * 0.5f, cos(timer / 5000.f) * 0.5f, sin(timer / 3500.f) * 1.7f + dist);

	scene->object[0]->setLoc(sin((timer + 12455) / 5000.f) * 0.6f,
		                     sin((timer + 82455) / 8000.f) * 0.6f,
							 cos((timer + 67376) / 7000.f) * 0.6f);

	scene->object[1]->setLoc(sin((timer + 67635) / 6000.f) * 0.6f,
		                     sin((timer + 23553) / 8000.f) * 0.6f,
							 cos((timer + 95254) / 5000.f) * 0.6f);

	scene->object[2]->setLoc(sin((timer + 53512) / 7000.f) * 0.6f,
		                     sin((timer + 27625) / 5000.f) * 0.6f,
							 cos((timer + 12455) / 9000.f) * 0.6f);
	scene->render();
}

void Clouds::DeInit() 
{
	delete scene;
}