#include "Main.h"

char motionTexture[] = {'m', 'o', 't', 'i', 'o', 'n', 'X', '\0'};

bool BlurMotion::Init()
{
	nrLayers = 8;

	for (int i = 0; i < nrLayers; i++)
	{
		motionTexture[6] = i + 48;
		uniTextureCreate(motionTexture, NULL, 512, 256, UNI_RENDERTARGET);
	}


	activeLayer = 0;

	Clear(0);

	return true;
}

void BlurMotion::Open()
{
	motionTexture[6] = activeLayer + 48;

	uniTextureSetRenderTarget(motionTexture);

	uniClearBuffers(UNI_BBF | UNI_ZBF, 0x0);
}

void BlurMotion::Close()
{
	uniRenderTargetReset();
}

void BlurMotion::Do(dword timer)
{
	SpriteDrawer *sd = uniSystem->spriteDrawer;

	sd->Start(UNI_ADD);

	float xl =   0;
	float yb =  90;

	float xr = 800;
	float yo = 510;

	float ul = 0.01f;
	float vb = 0.01f;

	float ur = 1;
	float vo = 1;

	for (int i = 0; i < nrLayers; i++)
	{
		int layer = (activeLayer + i + 1) % nrLayers;
		int fade = (64 / nrLayers) * i;

		unsigned int color = (fade << 16) | (fade << 8) | fade;

		motionTexture[6] = layer + 48;
		uniSetTexture(motionTexture);

		sd->Draw2D(xl, yb, ul, vb, color,
			       xr, yb, ur, vb, color,
				   xl, yo, ul, vo, color,
				   xr, yo, ur, vo, color);
	}

	sd->Stop();

	activeLayer = (activeLayer + 1) % nrLayers;
}

void BlurMotion::Clear(dword color)
{
	for (int i = 0; i < nrLayers; i++)
	{
		motionTexture[6] = i + 48;

		uniTextureSetRenderTarget(motionTexture);

		uniClearBuffers(UNI_BBF | UNI_ZBF, color);

		uniRenderTargetReset();
	}
}

void BlurMotion::DeInit()
{
}

bool BlurRadial::Init()
{
	nrLayers = 6;
	textureName = "radialBlurTarget";

	uniTextureCreate(textureName, NULL, 512, 256, UNI_RENDERTARGET);

	mode = 1;

	return true;
}

void BlurRadial::Open()
{
	uniTextureSetRenderTarget(textureName);

	uniClearBuffers(UNI_BBF | UNI_ZBF, 0x0);
}

void BlurRadial::Close()
{
	uniRenderTargetReset();
}


void BlurRadial::SetMode(int iMode)
{
	mode = iMode;
}

void BlurRadial::Do(dword timer)
{
	float blurSpeed;
	switch (mode)
	{
	case 1:
		if (timer < 200)
		{
			blurSpeed = 0.006f + (sin(timer * (_PI2 / 400.f)) * 0.01f);
		}
		else
		{
			blurSpeed = 0.005f;
		}
		break;
	case 2:
		blurSpeed = 0.014f;
	default:
		blurSpeed = 0.01f;
		break;
	}

	SpriteDrawer *sd = uniSystem->spriteDrawer;

	uniSetTexture(textureName);

	sd->Start(UNI_ADD);

	float xl =   0;
	float yb =  90;

	float xr = 800;
	float yo = 510;

	float ul = 0.01f;
	float vb = 0.01f;

	float ur = 1;
	float vo = 1;

	int fade = 0xff / nrLayers;

	unsigned int color = (fade << 16) | (fade << 8) | fade;

	for (int i = 0; i < nrLayers; i++)
	{
		unsigned int defcolor;

		defcolor = color;

		if (mode == 1)
		{
			if (i == 0)
				defcolor = 0xffffff;
		}


		sd->Draw2D(xl, yb, ul, vb, defcolor,
			                xr, yb, ur, vb, defcolor,
				    		xl, yo, ul, vo, defcolor,
					    	xr, yo, ur, vo, defcolor);

		switch (mode)
		{
		case 1:
			ul += blurSpeed;
			ur -= blurSpeed;

			vb += blurSpeed;
			vo -= blurSpeed;
			break;
		case 2:
			ul += blurSpeed - cos(timer / 2000.f) * 0.005f;
			ur -= blurSpeed - cos(timer / 2000.f) * 0.005f;

			vb += blurSpeed - cos(timer / 2000.f) * 0.0045f;
			vo -= blurSpeed - cos(timer / 2000.f) * 0.0045f;
			break;
		}
/*
 // hospital02
		ul += blurSpeed - cos(timer / 2000.f) * 0.005f + 0.005f;
		ur -= blurSpeed - cos(timer / 2000.f) * 0.005f + 0.005f;

		vb += blurSpeed - cos(timer / 2000.f) * 0.005f + 0.005f;
		vo -= blurSpeed - cos(timer / 2000.f) * 0.005f + 0.005f;
*/
 // background - hospital01
/*
		ul += blurSpeed - cos(timer / 2000.f) * 0.005f;
		ur -= blurSpeed - cos(timer / 2000.f) * 0.005f;

		vb += blurSpeed - cos(timer / 2000.f) * 0.005f;
		vo -= blurSpeed - cos(timer / 2000.f) * 0.005f;
*/
	}

	sd->Stop();
}

void BlurRadial::DeInit()
{
}



bool BlurHorizontal::Init()
{
	nrLayers = 6;
	textureName = "horizontalBlurTarget";

	uniTextureCreate(textureName, NULL, 512, 256, UNI_RENDERTARGET);

	return true;
}

void BlurHorizontal::Open()
{
	uniTextureSetRenderTarget(textureName);

	uniClearBuffers(UNI_BBF | UNI_ZBF, 0x0);
}

void BlurHorizontal::Close()
{
	uniRenderTargetReset();
}

void BlurHorizontal::Do(dword timer)
{
	SpriteDrawer *sd = uniSystem->spriteDrawer;

	uniSetTexture(textureName);

	sd->Start(UNI_ADD);

	float xl =   0;
	float yb =  90;

	float xr = 800;
	float yo = 510;

	float ul = 0.01f;
	float vb = 0.01f;

	float ur = 1;
	float vo = 1;

	int fade = 0xff / (nrLayers * 2);

	int color = (fade << 16) | (fade << 8) | fade;

	float ieks = 30.f - cos(timer / 200.f) * 30.f;

	for (int i = 0; i < nrLayers; i++)
	{
		float blah = (ieks / (nrLayers / 2.f)) * (i + 1.f);
		sd->Draw2D(xl - blah, yb, ul, vb, color,
			                xr, yb, ur, vb, color,
				    		xl - blah, yo, ul, vo, color,
					    	xr, yo, ur, vo, color);


		sd->Draw2D(xl, yb, ul, vb, color,
			                xr + blah, yb, ur, vb, color,
				    		xl, yo, ul, vo, color,
					    	xr + blah, yo, ur, vo, color);
	}

	sd->Stop();
}

void BlurHorizontal::DeInit()
{
}

