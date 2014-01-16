#include "Main.h"

#define EYE_NRPARTS 8000




bool Eye::Init() 
{
	scene = new Scene();

	scene->addSpriteGroup(new SpriteGroup(EYE_NRPARTS, 0.2f, 0xffffff, "sprite02"));

	for (int i = 0; i < EYE_NRPARTS; i++)
	{
		scene->spriteGroup[0]->sprite[i].setLoc(uniRand() * 2.0f - 1.0f,
			                                    uniRand() * 2.0f - 1.0f,
												uniRand() * 2.0f - 1.0f);
//		scene->spriteGroup[0]->sprite[i].setLoc(uniRand() * 1.2f - 0.6f,
//			                                    uniRand() * 1.2f - 0.6f,
//												uniRand() * 1.2f - 0.6f);
	}

	camTimer1 = camTimer2 = camTimer3 = camTimer4 = -99999999;

	return true;
}   

float oldTime = 0;

void Eye::Do(dword timer) 
{

	float dif = timer - oldTime;
	oldTime = timer;

	int color = 0x505050;

	if ((timer - camTimer1) < 300)
	{
		int fade = 0x50;
		fade = 0x50 + (sin((timer - camTimer1) * (_PI2 / 600.f)) * 150.f);
		color = (fade << 16) | (fade << 8) | fade;
	}
	else if ((timer - camTimer2) < 400)
	{
		int fade = 0x50;
		fade = 0x50 - (sin((timer - camTimer2) * (_PI2 / 800.f)) * 55.f) - (sin(timer / 20.f) * 0.f);
		if (fade < 0)
			fade = 0;
		color = (fade << 16) | (fade << 8) | fade;
	}
	else if ((timer - camTimer3) < 800)
	{
		int fade = 0x50;
		fade = 0x50 - (sin((timer - camTimer3) * (_PI2 / 1600.f)) * 65.f) - (sin(timer / 20.f) * 0.f);
		if (fade < 0)
			fade = 0;
		color = (fade << 16) | (fade << 8) | fade;
	}
	else if ((timer - camTimer4) < 300)
	{
		int fade = 0x50;
		fade = 0x50 + (sin((timer - camTimer4) * (_PI2 / 600.f)) * 60.f);
		color = (fade << 16) | (fade << 8) | fade;
	}
	

	for (int i = 0; i < EYE_NRPARTS; i++)
	{
		VertexPoint *v = &scene->spriteGroup[0]->sprite[i];

		v->diffuse = color;

		float dirx = ((sin(v->y + 2341.f) / 6.9f) + (sin(v->z + 1234.f) / 5.9f)) * 0.003f;
		float diry = ((cos(v->x + 0000.f) / 5.1f) + (cos(v->z + 1234.f) / 5.9f)) * 0.003f;
		float dirz = ((sin(v->x + 0000.f) / 5.1f) + (cos(v->y + 2341.f) / 6.9f)) * 0.003f;

		v->x += dirx * dif;
		v->y += diry * dif;
		v->z += dirz * dif;
	}

	float y = 0;

	if (timer < 15000)
	{
		y = 8.f - sin(timer * (_PI / 30000.f)) * 8.f;
	}

	scene->camera->setLoc(sin(timer / 7000.f) * 13.f, y, cos(timer / 7000.f) * 15.f);
	scene->camera->setTar(0, y, 0);

	if (timer > 42000)
	{
		float distx = 13.f * cos((timer - 42000) / 4000.f);
		float distz = 15.f * cos((timer - 42000) / 4000.f);
		scene->camera->setLoc(sin(timer / 7000.f) * distx, 0, cos(timer / 7000.f) * distz);
	}


	blurRadial->SetMode(1);
	blurRadial->Open();

	scene->render();

	blurRadial->Close();

	blurRadial->Do(timer - camTimer1);
}

void Eye::Receive(dword Message, dword timer)
{
	int i;
	switch (Message & 255)
	{
	case 1:
		camTimer1 = timer;
		break;
	case 2:
		camTimer2 = timer;
		break;
	case 3:
		camTimer3 = timer;
		break;
	case 4:
		camTimer4 = timer;
		break;
	}
}

void Eye::DeInit() 
{
	delete scene;
}
