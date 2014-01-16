#include "Main.h"

#define STARFIELD_NRSTARS   10000
#define STARFIELD_STARSPEED    0.01f

/*
011100011100111100111100111100011110011
110110110110110110110110110110110000011
110110110110110110110110110110110000011
110110110110110110110110111100111110011
111110111110111100110110110110110000011
110110110110110110110110110110110000011
110110110110110110111100111100011110011 
*/

//#define STARFIELD_LOGOEFFECT

// 39 bij 7
bool STARFIELD_LOGO[] =
{
	0,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,
	1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,
	1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,
	1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,
	1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,
	1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,
	1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1 
};


bool Starfield::Init() 
{
	scene = new Scene();

#ifndef STARFIELD_LOGOEFFECT
	scene->fogSprites = true;

	scene->addSpriteGroup(new SpriteGroup(STARFIELD_NRSTARS, 0.14f, 0xffffffff, "sprite01"));
	for (int i = 0; i < STARFIELD_NRSTARS; i++) {
		scene->spriteGroup[0]->sprite[i].setLoc((uniRand() * 20.f) - 10.f,
								                (uniRand() * 20.f) - 10.f,
												(uniRand() * 30.f) - 5.f);
	}
#else
	scene->addSpriteGroup(new SpriteGroup(39 * 7, 0.14f, 0xffffffff, "sprite01"));
	for (int i = 0; i < 39 * 7; i++) {
		scene->spriteGroup[0]->sprite[i].setLoc(((i % 39) - 20.f) * 0.05f,
												(3.f - ((i / 39))) * 0.1f,
												0);
		if (!STARFIELD_LOGO[i])
		{
			scene->spriteGroup[0]->sprite[i].diffuse = 0;
		}
	}
#endif


	oldTimer = 0;

	return true;
}   

void Starfield::Do(dword timer) 
{	
	float timing = timer - oldTimer;
	oldTimer = timer;

#ifndef STARFIELD_LOGOEFFECT
	for (int i = 0; i < STARFIELD_NRSTARS; i++) {
		float x = scene->spriteGroup[0]->sprite[i].x;
		float y = scene->spriteGroup[0]->sprite[i].y;
		float z = scene->spriteGroup[0]->sprite[i].z;

		scene->spriteGroup[0]->sprite[i].setLoc(x, y, z - (timing * STARFIELD_STARSPEED));
/*
		int r = (int)((sin(x * 2.f) * 64.f) + 190.f) << 16;
		int g = (int)((sin(y) * 127.f) + 127.f) << 8;
		int b = (int)((sin(z / 3.f) * 127.f) + 127.f) << 0;
		scene->spriteGroup[0]->sprite[i].diffuse = r | g| b;
*/
		if (z < -5.0f) {
		/*
			x = (uniRand() * 20.f) - 10.f;
			y = (uniRand() * 20.f) - 10.f;
			z = 25 + (uniRand() * 4.f);
			if ((x == 0.0f) & (y == 0.0f))
				x = y = 1;
		*/
///*
			float p = uniRand() * _PI2;
			x = sin(p) * (uniRand() * 0.4f + 1.f + (sin(timer / 1000.f) * 0.4f));
			y = cos(p) * (uniRand() * 0.4f + 1.f + (cos(timer / 1300.f) * 0.4f));
//			x = sin(p) * (uniRand() * 0.8f + 1.5f);
//			y = cos(p) * (uniRand() * 0.8f + 1.f);
			z = 25 + (uniRand() * 4.f);

//			x += sin(timer / 780.f) * 4.f;
//			y += cos(timer / 780.f) * 4.f;
//*/
			
			scene->spriteGroup[0]->sprite[i].setLoc(x, y, z);
		}
	}

	scene->fog = true;
	scene->fogEnd = 30.f;

	scene->camera->setRoll(sin(timer / 5000.f) * 20.f);

//	scene->camera->setTar(sin(timer / 1000.f) * 2.f, cos(timer / 1300.f) * 2.f, 10);
//	scene->camera->setLoc(sin((timer - 2200.f) / 780.f) * 4.f, cos((timer - 2200.f) / 780.f) * 4.f, 0);

	uniSetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

//	scene->camera->setLoc(-0.04f, 0, 0);

	blurRadial->Open();

	scene->render();

	blurRadial->Close();
	blurRadial->Do(timer);


	uniSetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
#else

	for (int i = 0; i < 39 * 7; i++) {
		float radius = sin(timer / 2000.f) * 0.1f;
		float xAdd = sin((timer / 260.f) + (i * 124)) * radius;
		float yAdd = cos((timer / 400.f) + (i *  53)) * radius;
		float zAdd = cos((timer / 330.f) + (i * 110)) * radius;
		float x = ((i % 39) - 20.f) * 0.05f;
		float y = (3.f - ((i / 39))) * 0.1f;
		scene->spriteGroup[0]->sprite[i].setLoc(x + xAdd,
												y + yAdd,
												0 + zAdd);
		if (!STARFIELD_LOGO[i])
		{
			scene->spriteGroup[0]->sprite[i].diffuse = 0;
		}
	}

	scene->camera->setLoc(sin(timer / 1000.f) * 4.f, cos(timer / 1244.f) * 2.f, cos(timer / 1700.f) * 5.f);
	scene->render();
#endif
}

void Starfield::DeInit() 
{
	delete scene;
}
