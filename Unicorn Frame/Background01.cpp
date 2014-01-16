#include "Main.h"
		
bool Background01::Init()
{
	return true;
}
 

void Background01::Do(dword timer)
{
	uniSetTexture("back01", 0);
	uniSetTexture(NULL, 1);

	blurRadial->SetMode(2);

	blurRadial->Open();

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	sd->Start(UNI_ADD);

	for (int yy = 0; yy < 8; yy++)
	{
		for (int xx = 0; xx < 16; xx++)
		{
			int x1 = xx * 32;
			int x2 = x1 + 32;
			int y1 = yy * 32;
			int y2 = y1 + 32;

// 1
			int fade = sin(xx * yy + (timer / 1000.f)) * 127 + 128;

// 2
//			int fade = (sin((xx / 2.f) + (timer / 1400.f)) * cos((yy / 2.f) + (timer / 1000.f))) * 127 + 128;

// 3
//			int fade = (sin(((xx + yy) / 3.f) + (timer / 2000.f)) * 127.f) + 128;

			int color = (fade << 16) | (fade << 8) | fade;

			sd->Draw2D(x1, y1, 0, 0, color,
								 x2, y1, 1, 0, color,
								 x1, y2, 0, 1, color,
								 x2, y2, 1, 1, color);
		}
	}

/*
	for (int yy = 0; yy < 16; yy++)
	{
		for (int xx = 0; xx < 32; xx++)
		{
			int x1 = xx * 16;
			int x2 = x1 + 16;
			int y1 = yy * 16;
			int y2 = y1 + 16;

// 1
			int fade = sin(xx * yy + (timer / 1000.f)) * 127 + 128;

// 2
//			int fade = (sin((xx / 2.f) + (timer / 1400.f)) * cos((yy / 2.f) + (timer / 1000.f))) * 127 + 128;

// 3
//			int fade = (sin(((xx + yy) / 3.f) + (timer / 2000.f)) * 127.f) + 128;

			int color = (fade << 16) | (fade << 8) | fade;

			spriteDrawer->Draw2D(x1, y1, 0, 0, color,
								 x2, y1, 1, 0, color,
								 x1, y2, 0, 1, color,
								 x2, y2, 1, 1, color);
		}
	}
*/


	sd->Stop();

	blurRadial->Close();

	blurRadial->Do(timer);

}

void Background01::DeInit()
{
}