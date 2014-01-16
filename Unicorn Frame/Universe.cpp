#include "Main.h"

#define UNIVERSE_NRSTARS1 1500

#define UNIVERSE_PLANETS

class MovingStar
{
public:
	float radius;
	float timer;
};

MovingStar universeStars[UNIVERSE_NRSTARS1];

bool Universe::Init() {
	scene = new Scene();

	scene2 = new Scene();

	scene->addSpriteGroup(new SpriteGroup(UNIVERSE_NRSTARS1, 1.0f, 0xffffffff, "little")); 
	for (int i = 0; i < UNIVERSE_NRSTARS1; i++) {
		float x, y, z;
		float r = uniRand() * 100.f;
		universeStars[i].radius = r;
		x = sin(i * 14.f) * r;
		z = cos(i * 14.f) * r;
		y = (sin(i) * (100 - r)) / 15.f;
		if (r < 50)
			y *= 2.0f - ((r) / 25.f);
		scene->spriteGroup[0]->sprite[i].setLoc(x, y, z);
		int fade = uniRand() * 255;
		scene->spriteGroup[0]->sprite[i].diffuse = (fade << 16) | (fade << 8) | fade;
	}

	scene->addSpriteGroup(new SpriteGroup(2, 25.4f, 0xffff00, "sprite01")); 

	scene->addObject(genSphere(5, 15.f, NULL));
	scene->object[0]->cull = CULL_NONE;
	scene->object[0]->transMode = UNI_ADD;
	scene->object[0]->writeZBuffer = false;
	scene->object[0]->setAmbient(0.4f, 0.2f, 0.1f, 1);

	scene->addObject(genPlane(4, 200.f, "nevel01"));
	scene->object[1]->cull = CULL_NONE;
	scene->object[1]->transMode = UNI_ADD;
	scene->object[1]->setLoc(0, -1, 0);
	scene->object[1]->setAmbient(0.6f, 0.6f, 0.6f, 1);
	scene->object[1]->writeZBuffer = false;

	scene->addObject(genPlane(4, 200.f, "nevel01"));
	scene->object[2]->cull = CULL_NONE;
	scene->object[2]->transMode = UNI_ADD;
	scene->object[2]->setLoc(0,  1, 0);
	scene->object[2]->setAmbient(0.6f, 0.6f, 0.6f, 1);
	scene->object[2]->writeZBuffer = false;

#ifdef UNIVERSE_PLANETS

	// planet 1
	// 0x101050
	scene->addSpriteGroup(new SpriteGroup(2, 6.0f, 0x402010, "sprite01")); 
	scene->spriteGroup[2]->sprite[0].setLoc(30, 0, 0);
	scene->spriteGroup[2]->sprite[1].setLoc(30, 0, 0);
	
	scene->addObject(genSphere(5, 3.f, NULL));
	scene->object[3]->cull = CULL_NONE;
	scene->object[3]->transMode = UNI_ADD;
	scene->object[3]->writeZBuffer = false;
	scene->object[3]->setAmbient(0.1f, 0.1f, 0.4f, 1);
	scene->object[3]->setAmbient(0.7f, 0.5f, 0.5f, 1);
	

	// planet 2
	scene->addSpriteGroup(new SpriteGroup(2, 9.0f, 0x402010, "sprite01")); 
	scene->spriteGroup[3]->sprite[0].setLoc(30, 0, 0);
	scene->spriteGroup[3]->sprite[1].setLoc(30, 0, 0);
	
	scene->addObject(genSphere(5, 5.f, NULL));
	scene->object[4]->cull = CULL_NONE;
	scene->object[4]->transMode = UNI_ADD;
	scene->object[4]->writeZBuffer = false;
	scene->object[4]->setAmbient(0.1f, 0.1f, 0.4f, 1);
	scene->object[4]->setAmbient(0.7f, 0.5f, 0.5f, 1);

	// planet 3
	scene->addSpriteGroup(new SpriteGroup(2, 8.5f, 0x402010, "sprite01"));
	scene->spriteGroup[4]->sprite[0].setLoc(30, 0, 0);
	scene->spriteGroup[4]->sprite[1].setLoc(30, 0, 0);
	
	scene->addObject(genSphere(5, 4.f, NULL));
	scene->object[5]->cull = CULL_NONE;
	scene->object[5]->transMode = UNI_ADD;
	scene->object[5]->writeZBuffer = false;
	scene->object[5]->setAmbient(0.1f, 0.3f, 0.10f, 1);
	scene->object[5]->setAmbient(0.7f, 0.5f, 0.5f, 1);


	// planet 4
	scene->addSpriteGroup(new SpriteGroup(2, 6.5f, 0x402010, "sprite01"));
	scene->spriteGroup[5]->sprite[0].setLoc(30, 0, 0);
	scene->spriteGroup[5]->sprite[1].setLoc(30, 0, 0);
	
	scene->addObject(genSphere(5, 3.f, NULL));
	scene->object[6]->cull = CULL_NONE;
	scene->object[6]->transMode = UNI_ADD;
	scene->object[6]->writeZBuffer = false;
	scene->object[6]->setAmbient(0.1f, 0.1f, 0.4f, 1);
	scene->object[6]->setAmbient(0.7f, 0.5f, 0.5f, 1);


	// planet 5
	scene->addSpriteGroup(new SpriteGroup(2, 8.5f, 0x402010, "sprite01"));
	scene->spriteGroup[6]->sprite[0].setLoc(30, 0, 0);
	scene->spriteGroup[6]->sprite[1].setLoc(30, 0, 0);
	
	scene->addObject(genSphere(5, 4.f, NULL));
	scene->object[7]->cull = CULL_NONE;
	scene->object[7]->transMode = UNI_ADD;
	scene->object[7]->writeZBuffer = false;
	scene->object[7]->setAmbient(0.1f, 0.1f, 0.4f, 1);
	scene->object[7]->setAmbient(0.7f, 0.5f, 0.5f, 1);


	// planet 6
	scene->addSpriteGroup(new SpriteGroup(2, 5.5f, 0x402010, "sprite01"));
	scene->spriteGroup[7]->sprite[0].setLoc(30, 0, 0);
	scene->spriteGroup[7]->sprite[1].setLoc(30, 0, 0);
	
	scene->addObject(genSphere(5, 2.4f, NULL));
	scene->object[8]->cull = CULL_NONE;
	scene->object[8]->transMode = UNI_ADD;
	scene->object[8]->writeZBuffer = false;
	scene->object[8]->setAmbient(0.1f, 0.1f, 0.4f, 1);
	scene->object[8]->setAmbient(0.7f, 0.5f, 0.5f, 1);

#endif

	scene2->addObject(genSphere(4, 2000.f, "stars01"));
	scene2->object[0]->cull = CULL_ACW;


	scene->camera->fov = 1.0f;
	scene->fog = true;
	scene->fogEnd = 200.f;

	scene->fogSprites = true;

	scene2->camera->farPlane = 5000;

	scene->finish();
	scene2->finish();

	return true;
}  


unsigned int universeOldTime = 0;

void Universe::Do(dword timer) {
	int fadeTimer = timer;

	// limit frame rate
	while ((uniTimer() - universeOldTime) < 35)
	{
	}
	universeOldTime = uniTimer();

	int i;

	for (i = 0; i < UNIVERSE_NRSTARS1; i++) 
	{
		float x, y, z;
		float r = universeStars[i].radius;
		float p = (i * 14.f) - ((timer / r) / 300.f);
		x = sin(p) * r;
		z = cos(p) * r;
		scene->spriteGroup[0]->sprite[i].x = x;
		scene->spriteGroup[0]->sprite[i].z = z;
	}

#ifdef UNIVERSE_PLANETS
	float x, z;
	x = sin(timer / -2400.f) * 25.f;
	z = cos(timer / -2400.f) * 25.f;
	scene->spriteGroup[2]->sprite[0].setLoc(x, 0, z);
	scene->spriteGroup[2]->sprite[1].setLoc(x, 0, z);
	scene->object[3]->setLoc(x, 0, z);
	scene->object[3]->setRot(0, timer / 80.f, 0);


	x = sin((timer + 73232) / -3000.f) * 35.f;
	z = cos((timer + 73232) / -3000.f) * 35.f;
	scene->spriteGroup[3]->sprite[0].setLoc(x, 0, z);
	scene->spriteGroup[3]->sprite[1].setLoc(x, 0, z);
	scene->object[4]->setLoc(x, 0, z);
	scene->object[4]->setRot(0, timer / 110.f, 0);

	x = sin((timer + 63232) / -3700.f) * 45.f;
	z = cos((timer + 63232) / -3700.f) * 45.f;
	scene->spriteGroup[4]->sprite[0].setLoc(x, 0, z);
	scene->spriteGroup[4]->sprite[1].setLoc(x, 0, z);
	scene->object[5]->setLoc(x, 0, z);
	scene->object[5]->setRot(0, timer / 120.f, 0);

	x = sin((timer + 35665) / -4200.f) * 58.f;
	z = cos((timer + 35665) / -4200.f) * 58.f;
	scene->spriteGroup[5]->sprite[0].setLoc(x, 0, z);
	scene->spriteGroup[5]->sprite[1].setLoc(x, 0, z);
	scene->object[6]->setLoc(x, 0, z);
	scene->object[6]->setRot(0, timer / 150.f, 0);

	x = sin((timer + 84236) / -5000.f) * 65.f;
	z = cos((timer + 84236) / -5000.f) * 65.f;
	scene->spriteGroup[6]->sprite[0].setLoc(x, 0, z);
	scene->spriteGroup[6]->sprite[1].setLoc(x, 0, z);
	scene->object[7]->setLoc(x, 0, z);
	scene->object[7]->setRot(0, timer / 180.f, 0);

	x = sin((timer + 35902) / -5700.f) * 77.f;
	z = cos((timer + 35902) / -5700.f) * 77.f;
	scene->spriteGroup[7]->sprite[0].setLoc(x, 0, z);
	scene->spriteGroup[7]->sprite[1].setLoc(x, 0, z);
	scene->object[8]->setLoc(x, 0, z);
	scene->object[8]->setRot(0, timer / 200.f, 0);

#endif


	scene->fogStart = 100;
	scene->fogEnd = 230;

	scene->object[1]->setRot(0, timer / -80.f, 0);
	scene->object[2]->setRot(0, timer / -170.f, 0);

	float amp = 140.f;
	if (timer < 5000)
	{
		amp = 0 + sin(timer * (_PI / 10000.f)) * 140.f;
	}

	scene->camera->setLoc(sin(timer / 4000.f) * amp,
						  sin(timer / 4800.f) * 60.f,
						  cos(timer / 4000.f) * amp);

	timer += 12000.f;
	scene->camera->setTar(sin(timer / 4000.f) * 200.f,
						  sin(timer / 4800.f) * 90.f,
						  cos(timer / 4000.f) * 200.f);

	scene2->camera->setLoc(scene->camera->xLoc,
						   scene->camera->yLoc,
						   scene->camera->zLoc);

	scene2->camera->setTar(scene->camera->xTar,
						   scene->camera->yTar,
						   scene->camera->zTar);

	blurMotion->Open();

	scene2->render();
	scene->render();

	blurMotion->Close();

	blurMotion->Do(timer);

	uniSetTexture("title");
	uniSetTexture(NULL, 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	sd->Start(UNI_ADD);

	float x1 = 5;
	float y1 = 485;
	float x2 = 249;
	float y2 = 517;

	int fadel;
	int fader;

	if (fadeTimer > 13500)
	{
		fadel = 255 - (fadeTimer - 13500) / 7;
		fader = 255 - (fadeTimer - 13500) / 7;
	}
	else
	{
		fadel = (fadeTimer - 1000) / 10;
		fader = (fadeTimer - 1500) / 10;
	}

	if (fadel < 0)
		fadel = 0;
	if (fader < 0)
		fader = 0;
	if (fadel > 255)
		fadel = 255;
	if (fader > 255)
		fader = 255;


	int cl = (fadel << 16) | (fadel << 8) | fadel;
	int cr = (fader << 16) | (fader << 8) | fader;


	sd->Draw2D(x1, y1, 0, 0, cl,
			  x2, y1, 1, 0, cr,
  			  x1, y2, 0, 1, cl,
			  x2, y2, 1, 1, cr);

	sd->Stop();


	uniSetTexture(NULL);

	x1 = uniSystem->viewportX1;
	y1 = uniSystem->viewportY1;
	x2 = uniSystem->viewportX2;
	y2 = uniSystem->viewportY2;
 
    int c = 0xffd000;
	int a = 0xff - ((timer - 12000.f) / 5.f);
	if (a >= 0) 
	{
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0, 0, c,
				   x2, y1, 0, 0, c,
  				   x1, y2, 0, 0, c,
				   x2, y2, 0, 0, c);
		sd->Stop();
	}
} 

void Universe::DeInit() 
{
	delete scene;
}
