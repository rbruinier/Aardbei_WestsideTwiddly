#include "Main.h"

//#define EYE_NRPARTS 8000


Greet greets[14];

bool Eye02::Init() 
{
	scene = new Scene();

	scene->addSpriteGroup(new SpriteGroup(8000, 0.2f, 0xffffff, "sprite02"));

	for (int i = 0; i < 8000; i++)
	{
		scene->spriteGroup[0]->sprite[i].setLoc(uniRand() * 20.0f - 10.0f,
			                                    uniRand() * 20.0f - 10.0f,
												uniRand() * 20.0f - 10.0f);
	}

	greets[0].name  = "greet00";
	greets[1].name  = "greet01";
	greets[2].name  = "greet02";
	greets[3].name  = "greet03";
	greets[4].name  = "greet04";
	greets[5].name  = "greet05";
	greets[6].name  = "greet06";
	greets[7].name  = "greet07";
	greets[8].name  = "greet08";
	greets[9].name  = "greet09";
	greets[10].name = "greet10";
	greets[11].name = "greet11";
	greets[12].name = "greet12";
	greets[13].name = "greet13";

	greets[0].x  = 336; // array
	greets[1].x  = 326; // confine
	greets[2].x  = 322; // cryonics
	greets[3].x  = 327; // exceed
	greets[4].x  = 311; // farbrausch
	greets[5].x  = 325; // haujobb
	greets[6].x  = 320; // outbreak
	greets[7].x  = 332; // rebels
	greets[8].x  = 331; // replay
	greets[9].x  = 316; // sunflower
	greets[10].x = 349; // tbl
	greets[11].x = 324; // teastate
	greets[12].x = 333; // tpolm
	greets[13].x = 300; // wrathdesign


	return true;
}   

float oldTimeEye02 = 0;

unsigned int eye02OldTime = 0;

bool erkgjhekhgeerkjgh = true;

void Eye02::Do(dword timer) 
{
	int fadeTimer = timer;

	clearColor = 0x461901;
	if (erkgjhekhgeerkjgh)
	{
		uniClearBuffers(UNI_BBF, clearColor);
		erkgjhekhgeerkjgh = false;
	}

	// limit frame rate
	while ((uniTimer() - eye02OldTime) < 50)
	{
	}
	eye02OldTime = uniTimer();


	float dif = timer - oldTimeEye02;

	oldTimeEye02 = timer;

	int color = 0xffffffff;

	for (int i = 0; i < 8000; i++)
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

	scene->camera->setTar(0, 0, 0);
	scene->camera->setLoc(sin(timer / 7000.f) * 13.f, 0, cos(timer / 7000.f) * 15.f);

	if (fadeTimer > 8000)
	{
		float blah = fadeTimer - 8000.f;
		float amp = (blah / 24000.f) * 650.f;
		scene->camera->setRoll(sin(blah / 11000.f) * cos(blah / 12240.f) * amp);
	}
	else
	{
		scene->camera->setRoll(0);
	}


	if (fadeTimer > 26000)
	{
		float distx = 13.f * cos((fadeTimer - 26000) / 4000.f);
		float distz = 15.f * cos((fadeTimer - 26000) / 4000.f);
		scene->camera->setLoc(sin(timer / 7000.f) * distx, 0, cos(timer / 7000.f) * distz);
	}


	blurMotion->Open();

	scene->render();

	blurMotion->Close();

	blurMotion->Do(timer);

	uniSetTexture(NULL, 1);

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	sd->Start(UNI_ADD);

	float x1, x2, y1, y2;

	for (i = 0; i < 14; i++)
	{
		uniSetTexture(greets[i].name);


		x1 = greets[i].x + 40;
		y1 = 102 + (i * 28);
		x2 = x1 + 256;
		y2 = y1 + 32;

		int fadel;
		int fader;

		if (fadeTimer > 29000)
		{
			fadel = 255 - (fadeTimer - 29000) / 7;
			fader = 255 - (fadeTimer - 29000) / 7;
		}
		else
		{
			fadel = (fadeTimer - 7000 - (i * 400)) / 10;
			fader = (fadeTimer - 9500 - (i * 400)) / 10;
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
		int	cr = (fader << 16) | (fader << 8) | fader;


		sd->Draw2D(x1, y1, 0, 0, cl,
   				   x2, y1, 1, 0, cr,
  				   x1, y2, 0, 1, cl,
				   x2, y2, 1, 1, cr);

	}

	sd->Stop();

	uniSetTexture("distort5");

	x1 = uniSystem->viewportX1;
	y1 = uniSystem->viewportY1;
	x2 = uniSystem->viewportX2;
	y2 = uniSystem->viewportY2;
	
	float uadd = sin(timer / 10000.f) * 1.f;
	float vadd = cos(timer / 10000.f) * 1.f;

	int pos, row, sync;

	fmodGetInfo(pos, row, sync);
	
	int fade = 80 - row * 5;
	if (pos == 27 || pos == 29)
	{
		fade = 0;
	}
		
	if (fade > 0)
	{
		int c = ((int)((fade / 255.f) * 0x80) << 16) |
				((int)((fade / 255.f) * 0x40) << 8) |  
				((int)((fade / 255.f) * 0x10));
		sd->Start(UNI_ADD);
		sd->Draw2D(x1, y1, 0 + uadd, 0 + vadd, c,
				   x2, y1, 6 + uadd, 0 + vadd, c,
				   x1, y2, 0 + uadd, 6 + vadd, c,
				   x2, y2, 6 + uadd, 6 + vadd, c);
		sd->Stop();
	}

	uniSetTexture(NULL); 


    int c = 0;//clearColor;
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

void Eye02::Receive(dword Message, dword timer)
{
	int i;
	switch (Message & 255)
	{
	case 1:
		camTimer1 = timer;
		break;

	}
}

void Eye02::DeInit() 
{
	delete scene;
}
