#include "Main.h"

bool Atom::Init() {
	scene = new Scene();
/*
	for (int i = 0; i < 5; i++) {
		scene->addObject(genSphere(5, 0.2f, "texture04"));
		scene->object[i]->setAmbient(0xff / 255.f, 0xcc / 255.f, 0, 0);
		scene->object[i]->shade    = SHADE_ENVMAP;
		scene->object[i]->optimize();
		scene->object[i]->method = METHOD_NONE;
	}

	for (i = 0; i < 10; i++) {
		scene->addObject(genSphere(5, 0.3f, "texture04"));
		if (i % 2)
			scene->object[i + 5]->setAmbient(1, 0, 0, 0);
		else
			scene->object[i + 5]->setAmbient(1, 0.9f, 0, 0);
		scene->object[i + 5]->optimize();
		scene->object[i + 5]->shade = SHADE_ENVMAP;
		scene->object[i + 5]->setLoc(sin(i*20) * 0.2f, cos(i*34) * 0.2f,sin(i*30) * 0.2f);
	}
	*/

	for (int i = 0; i < 5; i++)
	{
		scene->addSpriteGroup(new SpriteGroup(50, 0.15f, 0x0, "sprite01"));
		for (int j = 0; j < 50; j++)
		{
			int r = 0xaa - ((0xaa / 50) * j);
			int g = 0x70 - ((0x70 / 50) * j);
			int b = 0x10 - ((0x10 / 50) * j);
			scene->spriteGroup[i]->sprite[j].diffuse = (0xff << 24) | (r << 16) | (g << 8) | (b);
		}
	}


	return true;
}

void Atom::Do(dword timer) {
	int i;
	float time;
	float delay = 30;

//////////////////////////////////////////////////////////////////
/*
	scene->object[0]->setLoc(cos(timer / 220.f) * 1.f,
		                     cos(timer / 220.f) * 0.0f,
							 sin(timer / 220.f) * 1.f);
*/
	time = timer - delay;
	for (i = 0; i < 50; i++) {
		scene->spriteGroup[0]->sprite[i].setLoc(cos(time / 220.f) * 1.f,
			                                    cos(time / 220.f) * 0.0f,
								                sin(time / 220.f) * 1.f);
		time -= 13.f;
	}
	timer += 100;

//////////////////////////////////////////////////////////////////
/*
	scene->object[1]->setLoc(cos(timer / 220.f) * 1.f,
		                     sin(timer / 220.f) * 1.f,
							 sin(timer / 220.f) * 0.f);
*/
	time = timer - delay;
	for (i = 0; i < 50; i++) {
		scene->spriteGroup[1]->sprite[i].setLoc(cos(time / 220.f) * 1.f,
							  			 sin(time / 220.f) * 1.f,
										 sin(time / 220.f) * 0.0f);
		time -= 13.f;
	}
	timer += 100;
//////////////////////////////////////////////////////////////////

/*
	scene->object[2]->setLoc(sin(timer / 220.f) * 0.0f,
		                     sin(timer / 220.f) * 1.f,
							 cos(timer / 220.f) * 1.f);
*/
	time = timer - delay;
	for (i = 0; i < 50; i++) {
		scene->spriteGroup[2]->sprite[i].setLoc(sin(time / 220.f) * 0.0f,
						    		            sin(time / 220.f) * 1.f,
									            cos(time / 220.f) * 1.f);
		time -= 13.f;
	}
	timer += 100;
//////////////////////////////////////////////////////////////////
/*
	scene->object[3]->setLoc(cos(timer / 220.f) * 0.8f,
		                     cos(timer / 220.f) * 0.8f,
							 sin(timer / 220.f) * 0.8f);
*/
	time = timer - delay;
	for (i = 0; i < 50; i++) {
		scene->spriteGroup[3]->sprite[i].setLoc(cos(time / 220.f) * 0.8f,
							             	    cos(time / 220.f) * 0.8f,
									            sin(time / 220.f) * 0.8f);
		time -= 13.f;
	}
	timer += 100;
//////////////////////////////////////////////////////////////////
/*
	scene->object[4]->setLoc(cos(timer / 220.f) * 0.8f,
		                     sin(timer / 220.f) * 0.8f,
							 sin(timer / 220.f) * 0.8f);
*/
	time = timer - delay;
	for (i = 0; i < 50; i++) {
		scene->spriteGroup[4]->sprite[i].setLoc(cos(time / 220.f) * 0.8f,
									            sin(time / 220.f) * 0.8f,
									            sin(time / 220.f) * 0.8f);
		time -= 13.f;
	}
	timer += 100;

//////////////////////////////////////////////////////////////////


	scene->camera->setLoc(sin(timer / 1000.f) * 15.f,
						  cos(timer / 700.f) * 20.f,
						  cos(timer / 1000.f) * 15.f);

//	scene->camera->setLoc(sin(timer / 3000.f) * 13.f,
//						  cos(timer / 2000.f) * 5.f,
//						  cos(timer / 3000.f) * 8.f);

	scene->render();
}

void Atom::DeInit() 
{
	delete scene;
}