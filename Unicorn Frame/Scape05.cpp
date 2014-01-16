#include "Main.h"

#define SCAPE05_LIQUID_SCALE 9.f
#define SCAPE05_WATER
#define SCAPE05_BUILDING
#define SCAPE05_RAIN 1000

#define SCAPE05_STARTR 0x99
#define SCAPE05_STARTG 0x99
#define SCAPE05_STARTB 0x99

#define SCAPE05_ENDR 0x70
#define SCAPE05_ENDG 0x90
#define SCAPE05_ENDB 0xf0

#define SCAPE05_SUNMODE



Greet credit[3];

Object *genPilar(int nrSegmentsX, int nrSegmentsY, float radius, float length, char *texture)
{

	Object *obj = 
		new Object((nrSegmentsX + 1) * (nrSegmentsY + 1), nrSegmentsX * nrSegmentsY * 2, texture);

	int index = 0;

	float yMul = length / nrSegmentsY;
	float xMul = (2.f * _PI) / nrSegmentsX;
	float uStp = 1.f / nrSegmentsX;
	float vStp = 1.f / nrSegmentsY;

	for (int y = 0; y < nrSegmentsY + 1; y++)
	{
		float yy = y * yMul;
		float vv = y * vStp;
		for (int x = 0; x < nrSegmentsX + 1; x++)
		{

			obj->vertex[index].x = sin(x * xMul) * radius;
			obj->vertex[index].y = yy;
			obj->vertex[index].z = cos(x * xMul) * radius;
			obj->vertex[index].tu1 = x * uStp;
			obj->vertex[index].tv1 = vv;
 
			index++;
		}
	}
	
	index = 0;

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

bool Scape05::Init() 
{
	nrSegmentsX = 127;
	nrSegmentsZ = 127;
	left   = -150;
	top    = -150;
	right  =  150;
	bottom =  150;

	scene = new Scene();

	scene->addObject(genPlane(nrSegmentsX, 300, "scape05final"));

//	scene->object[0]->scaleUV(10, 10);

	CreateShape(NULL, scene->object[0]->vertex, 4.f, 1.0f, 14);//9);

	Vertex *vertex = scene->object[0]->vertex;

	// plaat creeeren in het midden
	for (int y = 0; y < nrSegmentsZ + 1; y++)
	{
		for (int x = 0; x < nrSegmentsX + 1; x++)
		{
			int index = (y * (nrSegmentsX + 1)) + x;

			float vx = vertex[index].x;
			float vy = vertex[index].y;
			float vz = vertex[index].z;

			float length = sqrt((vx * vx) + (vz * vz));

			if (length < 30)
			{
				vy = 35;
			}
			else if (length < 40)
			{
				if (vy < 35)
				{
					vy = (((35 - vy) / 10.f) * (10.f - (length - 30.f))) + vy;
				}
			}

			vertex[index].x = vx;
			vertex[index].y = vy;
			vertex[index].z = vz;
		}
	}


	int width, height;	

	dword *grond1 = extraTextures[6];
	dword *grond2 = extraTextures[1];
	dword *grond3 = extraTextures[2]; 
	dword *grond4 = extraTextures[0];

	dword *textureFinal = CreateTexture(1024, 1024, grond1, grond2, grond3, grond4, -6.f, 1.8f);

	uniTextureCreate("scape05final", textureFinal, 1024, 1024, UNI_MIPMAP);

	ShowLoader(0.85f);

	scene->fog = true;
	scene->fogColor = 0x999999;
	scene->fogStart = 0;
	scene->fogEnd = 210;

	scene->addLight(new Light(1, 1, 1, 1));
	
	scene->camera->farPlane = 230;

	scene->ambient = 0x0;

	scene->camera->fov = 0.9f;

	scene->object[0]->finish(false);

	// liquid object
	oldSphere = genSphere(20, 2.f, NULL);

	scene->addObject(genSphere(20, 2.f, "env2"));
	scene->object[1]->shade    = SHADE_ENVMAP;
	scene->object[1]->optimize(0);
	scene->object[1]->finish(true);


#ifdef SCAPE05_WATER
	scene->addObject(genPlane(20, 300, "water01"));
	scene->object[2]->setLoc(0, 29, 0);
	scene->object[2]->cull = CULL_NONE;
	scene->object[2]->transMode = UNI_ALPHA;
	scene->object[2]->setDiffuse(1, 1, 1, 0.65f);
	scene->object[2]->scaleUV(5, 5);
	scene->object[2]->finish(true);
#endif

#ifdef SCAPE05_BUILDING
	for (int i = 0; i < 8; i++)
	{
		float length = 46.45f;
		if (i & 1)
			length = 46.4f;
		Object *obj = genPilar(10, 4, 1, length, "pelare");
		obj->setLoc(sin(i * _PI2 / 8.f) * 26.f,
			        34.9f,
					cos(i * _PI2 / 8.f) * 26.f);
		obj->scaleUV(1, 3);
		obj->finish(false);
		scene->addObject(obj);
	}

	Object *obj = genPlane(10, 55.f, "tak");
	int index = 0;
	for (y = 0; y < 11; y++)
	{
		for (int x = 0; x < 11; x++)
		{
			obj->vertex[index++].y = 0 - sqrt((x - 5) * (x - 5) + (y - 5) * (y - 5)) * 4.f;
		}
	}
	obj->setLoc(0, 101, 0);
	obj->setRot(0, 45, 0);
	obj->scaleUV(2, 2);
	obj->cull = CULL_NONE;

	obj->finish(false);

	scene->addObject(obj);
#endif

#ifdef SCAPE05_RAIN
	scene->addSpriteGroup(new SpriteGroup(SCAPE05_RAIN, 1.4f, 0x666666, "raindrop1"));
	for (i = 0; i < SCAPE05_RAIN; i++) 
	{
		scene->spriteGroup[0]->sprite[i].setLoc(uniRand() * 300.f - 150, 
							               		 uniRand() * 150.f + 150.f, 
												 uniRand() * 300.f - 150);
	}
#endif

#ifdef SCAPE05_SUNMODE
	scene->addSpriteGroup(new SpriteGroup(1, 30.f, 0xffffff, "sun"));
	scene->spriteGroup[1]->sprite[0].setLoc(80, 100, 40);
#endif

	credit[0].name = "credit00";
	credit[1].name = "credit01";
	credit[2].name = "credit02";

	credit[0].x = 31;
	credit[1].x = -6;
	credit[2].x = 8;

	transition = false;
	morphing   = false;
	credits    = false;
	blue	   = false;

	morphTimer = 0;
	transTimer = 0;
	creditTimer = 0;
	blueTimer  = 0;

	camTimer1 = 0;

	currentCam = 1;

	ShowLoader(0.9f);

	return true;
}   


#ifdef SCAPE05_RAIN
float regenTimer = 0;
float regenSpeed = 0.15f;
float regenBaseY = 150;
#endif

//float scape05Water = 1.0f;


void Scape05::Do(dword timer) 
{

	float speed = (float) sin(timer / 1000.f) * 1.0f + 4.0f;
	float height;
	if (morphing)
	{
		height =	0.7f - (float) cos((timer - morphTimer) / 3000.f) * 2.2f;
	}
	else
	{
		height =	0.7f - 2.2f;
		morphTimer = timer;
	}

	for (int i = 0; i < oldSphere->nrVertices; i++) {
		float x = oldSphere->vertex[i].x;
		float y = oldSphere->vertex[i].y;
		float z = oldSphere->vertex[i].z;
 
		float scale = (float) sin((x * speed) + (timer / 600.f)) +
					  (float) sin((y * speed) + (timer / 400.f)) +
					  (float) sin((z * speed) + (timer / 500.f));

		scale *= 0.25f;

		scale += 1.f;

		x = x + (scale * 0.1f);
		y = y * scale;
		z = z + (scale * 0.1f);

		y += height;

		if (y < 0) {
			x *= (-y * 0.75f) + 1.f;
			z *= (-y * 0.75f) + 1.f;
			y *= 0.1666667f;
		}

		scene->object[1]->vertex[i].x = x * SCAPE05_LIQUID_SCALE;
		scene->object[1]->vertex[i].y = y * SCAPE05_LIQUID_SCALE;
		scene->object[1]->vertex[i].z = z * SCAPE05_LIQUID_SCALE;
	}

	scene->object[1]->setLoc(0, 37.6f, 0);
	
	

#ifdef SCAPE05_RAIN
	float timing = timer - regenTimer;
	regenTimer = timer;
	for (i = 0; i < SCAPE05_RAIN; i++) {
		float x = scene->spriteGroup[0]->sprite[i].x;
		float y = scene->spriteGroup[0]->sprite[i].y;
		float z = scene->spriteGroup[0]->sprite[i].z;
		x -= (timing * 0.003f);
		y -= (timing * regenSpeed);
		z -= (timing * 0.002f);
		float bottom = 12.f;
		if (x > -30 && x < -30 && z > -30 && z < 30)
		{
			bottom = 120;
		}
		if (y < bottom) {
			x = (uniRand() * 300.f) - 150.f;
			y = (uniRand() *  50.f) + regenBaseY;
			z = (uniRand() * 300.f) - 150.f;
		}
		scene->spriteGroup[0]->sprite[i].setLoc(x, y, z);
	}
#endif

#ifdef SCAPE05_WATER	
	float divver = timer / 400.f;
	for (i = 0; i < scene->object[2]->nrVertices; i++) 
	{
		float x = scene->object[2]->vertex[i].x;
		float z = scene->object[2]->vertex[i].z;
		float y = sin((x * 1.5f) + (z * 1.5f) + divver) * 0.5f;
		scene->object[2]->vertex[i].y = y;
	}
#endif
	
	
	uniSetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
/*
	float x = sin(timer / 8000.f) * 60.f;
	float y = 54.f + (sin(timer / 8000.f) * 10.f);
	float z = cos(timer / 8000.f) * 145.f;

	float height2;
	GetHeightAtPosition(scene->object[0]->vertex, x, z, height2);

	if (y < height2 + 3.5f)
		y = height2 + 3.5f;

	scene->camera->setLoc(x, y, z);
	scene->light[0]->setLoc(x, y, z);

	
	scene->camera->setTar(1, height * SCAPE05_LIQUID_SCALE + 37, 1);

	scene->camera->setRoll(cos((timer + 11000) / 8000.f) * 10.f);
*/
	scene->camera->setRoll(0);

	float timerTemp;
	switch (currentCam)
	{
	case 1:
		timerTemp = timer - camTimer1 - 23000;
		scene->camera->setLoc(sin(timerTemp / 14000.f) * 140.f,
			                  70.f + sin(timerTemp / 9000.f) * 10.f,
					          cos(timerTemp / 14000.f) * 150.f);
		scene->camera->setRoll(-15 + (sin((timer - camTimer1) / 5000.f) * 25.f));
		scene->camera->setTar(0, 40, 0);
		break;
	case 2:

		timerTemp = timer - camTimer1;
		scene->camera->setLoc(sin(-timerTemp / 12000.f) * -80.f,
			                  60.f + sin(-timerTemp / 9000.f) * 10.f,
					          cos(-timerTemp / 12000.f) * 180.f);
		scene->camera->setRoll(cos((-timerTemp + 11000) / 10000.f) * 6.f);
		scene->camera->setTar(0, height * SCAPE05_LIQUID_SCALE + 42, 0);

/*
		timerTemp = timer - camTimer1;
		scene->camera->setLoc(sin(-timerTemp / 8000.f) * 40.f,
			                  70.f + sin(timerTemp / 9000.f) * 10.f,
					          cos(-timerTemp / 8000.f) * 50.f);
		scene->camera->setTar(0, 40, 0);
*/
		if ((timer - camTimer1) < 3900)
		{
			float amp = 0.2f - ((timer - camTimer1) / 19500.f);
			scene->camera->fov = 0.9f + (sin(fmod((timer - camTimer1), 650) * (_PI2 / 650.f)) * amp);
		}
		else
		{
			scene->camera->fov = 0.90;
		}

		break;
	case 3:

		timerTemp = timer - camTimer1;
		scene->camera->setLoc(sin(-timerTemp / 8000.f) * 40.f,
			                  70.f + sin(timerTemp / 9000.f) * 10.f,
					          cos(-timerTemp / 8000.f) * 50.f);
		scene->camera->setTar(0, 40, 0);

		if ((timer - camTimer1) < 3900)
		{
			float amp = 0.2f - ((timer - camTimer1) / 19500.f);
			scene->camera->fov = 0.9f + (sin(fmod((timer - camTimer1), 650) * (_PI2 / 650.f)) * amp);
		}
		else
		{
			scene->camera->fov = 0.90;
		}

		/*
		timerTemp = timer - camTimer1;
		scene->camera->setLoc(sin(-timerTemp / 8000.f) * 40.f,
			                  45.f + sin(timerTemp / 9000.f) * 10.f,
					          cos(-timerTemp / 8000.f) * 50.f);
		scene->camera->setTar(0, height * SCAPE05_LIQUID_SCALE + 37, 0);
		*/
		break;
	case 4:
		timerTemp = timer - camTimer1;
		scene->camera->setLoc(sin(timerTemp / 20000.f) * 80.f - 20,
			                  60.f + sin(-timerTemp / 9000.f) * 10.f + ((timerTemp / 30000.f) * 10.f),
					          cos(timerTemp / 20000.f) * -150.f);
		scene->camera->setRoll(0);//cos((-timerTemp + 11000) / 10000.f) * 6.f);
		//scene->camera->setTar(0, height * SCAPE05_LIQUID_SCALE + 42, 0);
		scene->camera->setTar(35 - ((timerTemp / 30000.f) * 20.f), 
			                  height * SCAPE05_LIQUID_SCALE + 42, 
							  0);

/*
		timerTemp = timer - camTimer1;
		scene->camera->setLoc(sin(timerTemp / 8000.f) * 10.f,
			                  85.f,
					          cos(timerTemp / 8000.f) * 10.f);
		scene->camera->setTar(0, 0, 0);
*/
		break;
	case 5:
		timerTemp = timer - camTimer1;
		scene->camera->setLoc(sin(timerTemp / 12000.f) * 60.f,
			                  54.f + sin(timerTemp / 9000.f) * 10.f,
					          cos(timerTemp / 12000.f) * 145.f);
		scene->camera->setRoll(cos((timerTemp + 11000) / 10000.f) * 6.f);
		scene->camera->setTar(0, height * SCAPE05_LIQUID_SCALE + 37, 0);
		break;

	}

	scene->light[0]->setLoc(scene->camera->xLoc, 
		                    scene->camera->yLoc, 
							scene->camera->zLoc);

	
	// onder water
/*	
	scene->camera->setLoc(sin(timer / 5000.f) * 10.f + 65, 27, cos(timer / 6000.f) * 60.f);
	scene->fogEnd = 70.f;
	scene->fogStart = -50.f;
*/	


#ifdef SCAPE05_SUNMODE
	if (transition)
	{
		float startTime = transTimer;
		float   endTime = transTimer + 30000;

		if (timer > startTime)
		{
			float pos = (timer - startTime) / (endTime - startTime);

			if (pos > 1)
				pos = 1;

			int r = ((SCAPE05_ENDR - SCAPE05_STARTR) * pos) + SCAPE05_STARTR;
			int g = ((SCAPE05_ENDG - SCAPE05_STARTG) * pos) + SCAPE05_STARTG;
			int b = ((SCAPE05_ENDB - SCAPE05_STARTB) * pos) + SCAPE05_STARTB;


			// fog & ambient
			clearColor = (r << 16) | (g << 8) | b;
			scene->fogColor = clearColor;
			scene->fogStart = pos * 20;	
	
			int amb = pos * 0x40;
			scene->ambient = (amb << 16) | (amb << 8) | (amb);

			// sun sprite
			amb = pos * 0xcc;
			scene->spriteGroup[1]->enabled = true;
			scene->spriteGroup[1]->sprite[0].diffuse = (amb << 16) | (amb << 8) | amb;
			scene->spriteGroup[1]->sprite[0].setLoc(80, 100, 40);

			// water amplitude
//			scape05Water = 1.0f - (pos * 0.5f);

			// light[0]
			scene->light[0]->setDiffuse(1 + pos * 0.3f, 1 + pos * 0.3f, 1 + pos * 0.3f, 1);
			float lightx = 40;
			float lighty = 100;
			float lightz = 80;
			if (pos < 1)
			{
				lightx = ((lightx - scene->camera->xLoc) * pos) + scene->camera->xLoc;
				lighty = ((lighty - scene->camera->yLoc) * pos) + scene->camera->yLoc;
				lightz = ((lightz - scene->camera->zLoc) * pos) + scene->camera->zLoc;
			}
			scene->light[0]->setLoc(lightx, lighty, lightz);
		

			scene->spriteGroup[0]->nrSprites = SCAPE05_RAIN - (pos * 5000);
			if (scene->spriteGroup[0]->nrSprites <= 0)
				scene->spriteGroup[0]->enabled = false;
		}
	}
	else
	{
		scene->spriteGroup[1]->enabled = false;
		transTimer = timer;
	}
#endif

	clearColor = scene->fogColor;

	scene->render();

	uniSetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

	uniSetTexture(NULL, 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;


	float x1, x2, y1, y2;

	if (credits)
	{
		sd->Start(UNI_ADD);

		int fadeTimer = timer - creditTimer;

		for (i = 0; i < 3; i++)
		{
			uniSetTexture(credit[i].name);


			x1 = credit[i].x + 700;
			y1 = 120 + (i * 28);
			x2 = x1 + 256;
			y2 = y1 + 32;
//			x1 = credit[i].x + 30;
//			y1 = 102 + (i * 28);
//			x2 = x1 + 200;
//			y2 = y1 + 26;

			int fadel;
			int fader;

			if (fadeTimer > 14000)
			{
				fadel = 120 - (fadeTimer - 14000) / 10;
				fader = 120 - (fadeTimer - 14000) / 10;
			}
			else
			{
				fadel = (fadeTimer - 0 - (i * 800)) / 10;
				fader = (fadeTimer - 1500 - (i * 800)) / 10;
			}

			if (fadel < 0)
				fadel = 0;
			if (fader < 0)
				fader = 0;
			if (fadel > 120)
				fadel = 120;
			if (fader > 120)
				fader = 120;


			int cl = (fadel << 16) | (fadel << 8) | fadel;
			int	cr = (fader << 16) | (fader << 8) | fader;


			sd->Draw2D(x1, y1, 0, 0, cl,
					  x2, y1, 1, 0, cr,
  					  x1, y2, 0, 1, cl,
					 x2, y2, 1, 1, cr);

		}

		sd->Stop();
	}


	uniSetTexture(NULL); 

	x1 = uniSystem->viewportX1;
	y1 = uniSystem->viewportY1;
	x2 = uniSystem->viewportX2;
	y2 = uniSystem->viewportY2; 
 
    int c = clearColor;
	int a = 0xff - (timer / 10.f);
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

	if (blue)
	{
	    int c = clearColor;
		int a = (timer - blueTimer) / 15.f;
		if (a > 255)
			a = 255;
		c += a << 24;
		sd->Start(UNI_ALPHA);
		sd->Draw2D(x1, y1, 0, 0, c,
				  x2, y1, 0, 0, c,
				  x1, y2, 0, 0, c,
				  x2, y2, 0, 0, c);
		sd->Stop();

		uniSetTexture("aardbei");

		x1 = 320;
		x2 = x1 + 256;
		y1 = 288;
		y2 = y1 + 32;

		int fadel, fader;

		int fadeTimer = timer - blueTimer;

		if (fadeTimer > 10000)
		{
			fadel = 120 - (fadeTimer - 10000) / 10;
			fader = 120 - (fadeTimer - 10000) / 10;
		}
		else
		{
			fadel = (fadeTimer - 7000) / 10;
			fader = (fadeTimer - 8500) / 10;
		}

		if (fadel < 0)
			fadel = 0;
		if (fader < 0)
			fader = 0;
		if (fadel > 120)
			fadel = 120;
		if (fader > 120)
			fader = 120;


		int cl = (fadel << 16) | (fadel << 8) | fadel;
		int	cr = (fader << 16) | (fader << 8) | fader;

/*
		a = (timer - blueTimer - 5000) / 60.f;

		if (a > 128)
			a = 128;

		if (a < 0)
			a = 0;

		c = (a << 16) | (a << 8) | a;
*/
		sd->Start(UNI_ADD);

		sd->Draw2D(x1, y1, 0, 0, cl,
			      x2, y1, 1, 0, cr,
				  x1, y2, 0, 1, cl,
				  x2, y2, 1, 1, cr);

		sd->Stop();


	}

}

void Scape05::Receive(dword Message, dword timer)
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
	case 5:
		camTimer1 = timer;
		currentCam = 5;
		break;
	case 20:
		morphing = true;
		break;
	case 21:
		transition = true;
		break;
	case 22:
		credits = true;
		creditTimer = timer;
		break;
	case 23:
		blue	  = true;
		blueTimer = timer;
//		credits = true;
//		creditTimer = timer;
		break;
	}
}

void Scape05::DeInit() 
{
	delete oldSphere;
	delete scene;
};
