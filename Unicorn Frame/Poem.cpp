#include "Main.h"

float poemPosX[] = 
{
	130, 0, -20, 
	100, 20, -30, -50,
	0, -50, 50, 200, -50, -80, 20
};

float poemPosY[] = 
{
	430, 120, 320, 
	430, 250, 120, 440,
	220, 440, 120, 400, 280, 430, 260
};

char *poemName[] = 
{
	"vulcantext1", 
	"vulcantext2", 
	"vulcantext3", 
	"kokkeltext1", 
	"kokkeltext2", 
	"kokkeltext3", 
	"kokkeltext4", 
	"liquidtext1", 
	"liquidtext2", 
	"liquidtext3", 
	"liquidtext4",
	"liquidtext5", 
	"liquidtext6", 
	"liquidtext7"
};

bool Poem::Init() 
{
	curText  = 0;
	return true;
}   

void Poem::Do(dword timer) 
{
	if (curText == -1)
	{
		return;
	}

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	float x1, x2, y1, y2;

	x1 = 10 + poemPosX[curText] + (timer / 7000.f) * 300.f;
	y1 = poemPosY[curText];


	x2 = x1 + 409;

	y2 = y1 + 51;

	uniSetTexture(poemName[curText]);

	if (timer < 7000)
	{
		if ((timer > 1000) && (timer < 6000))
		{
			int fade = 20 - (cos(((timer - 1000) / 2500.f) * _PI) * 20.f);

			int color = (fade << 16) | (fade << 8) | fade;


			sd->Start(UNI_ADD);

			sd->Draw2D(x1, y1, 0, 0, color,
				       x2, y1, 1, 0, color,
					   x1, y2, 0, 1, color,
					   x2, y2, 1, 1, color);

			fade = fade / 2.5;
			color = (fade << 16) | (fade << 8) | fade;

			float adder = sin(timer / 5000.f) * 40.f;

			sd->Draw2D(x1 - adder, y1, 0, 0, color,
				       x2 - adder, y1, 1, 0, color,
					   x1 - adder, y2, 0, 1, color,
					   x2 - adder, y2, 1, 1, color);

			sd->Draw2D(x1 + adder, y1, 0, 0, color,
				       x2 + adder, y1, 1, 0, color,
					   x1 + adder, y2, 0, 1, color,
					   x2 + adder, y2, 1, 1, color);

			sd->Stop();
		}
	}

}

void Poem::Receive(dword Message, dword timer)
{
	curText = Message & 255;
}

void Poem::DeInit() 
{
}
