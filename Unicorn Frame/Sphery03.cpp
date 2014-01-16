#include "Main.h"

bool Sphery03::Init() 
{
	scene = new Scene();

//	scene->addObject(genSphere(36, 2.f, "env3"));
	scene->addObject(genSphere(30, 2.f, "env3"));
//	scene->object[0]->shade = SHADE_ENVMAP;
//	scene->object[0]->optimize(0);

	oldObject = genSphere(30, 2.f, NULL);

	scene->object[0]->finish(true);

	scene->ambient = 0x0;

	scene->addLight(new Light(1.3f, 1.3f, 1.3f, 1));
//	scene->addLight(new Light(-0.8, -0.8, -0.8, 1));

	currentCam = 1;
	camTimer1  = 0;

	return true;
}   

void Sphery03::Do(dword timer) 
{	

	//scene->object[0]->setLoc(sin(timer / 2000.f) * 3.f, 0, 0);


	timer += 0;
	Vertex *src = oldObject->vertex;
//	float timer1 = timer / 1500.f;
//	float timer2 = timer / 2300.f;
//	float timer3 = timer / 1800.f;
	float timer1 = timer / 2000.f;
	float timer2 = timer / 2700.f;
	float timer3 = timer / 2100.f;
	for (int i = 0; i < oldObject->nrVertices; i++)
	{
		float x = src[i].x;
		float y = src[i].y;
		float z = src[i].z;

		float scale;


	//scene->object[0]->setLoc(-10, 0, 0);
		//scene->object[0]->
// sphery 05
//		scale = 1.0f + ((sin((x * 12.f) + (timer / 1200.f)) * cos((z * 10.5f) + (timer / 2000.f)) * sin((y * 14.5f) + (timer / 1700.f))) * ((sin((timer / 2000.f)) * 0.35f) + 0.35f));
//		scale = 1.0f + ((sin((x * 12.f) + (timer / 1200.f)) * cos((z * 10.5f) + (timer / 2000.f)) * sin((y * 14.5f) + (timer / 1700.f))) * ((sin((timer / 2000.f)) * 0.15f) + 0.45f));
//		scale = 1.0f + ((sin((x * 7.f) + (timer / 1200.f)) * cos((z * 8.5f) + (timer / 2000.f)) * sin((y * 7.5f) + (timer / 1700.f))) * ((sin((timer / 2000.f)) * 0.15f) + 0.45f));

		scale = 1.0f + ((sin((x * 12.f)  + (timer / 1200.f)) * 
			             cos((z * 10.5f) + (timer / 2000.f)) * 
						 sin((y * 14.5f) + (timer / 1700.f))) * 
//						 ((sin((timer / 2000.f)) * 0.35f) + 0.35f));
						 ((cos((timer / 2350.f)) * -0.35f) + 0.35f));


// sphery 06
//		scale = 1.f + ((sin((x * 12.f) + (timer / 1200.f)) * cos((z * 10.5f) + (timer / 2000.f)) * sin((y * 14.5f) + (timer / 1700.f))) * ((sin((timer / 2000.f) + (x * 4.f)) * 0.35f) + 0.35f));
		
		x *= scale;
		y *= scale;
		z *= scale;

		float dist = 2.5f - scale;
///*
		D3DMATRIX rotate;
		uniMatrixRotate(rotate, sin((timer1) + (dist * 0.3f)) * 3.3f, 
			                    cos((timer2) + (dist * 0.3f)) * 3.3f, 
								sin((timer3) + (dist * 0.3f)) * 3.3f);
//*/
//		D3DMATRIX rotate;
//		uniMatrixRotate(rotate, sin(0) * 3.3f, 
//			                    cos(0) * 3.3f, 
//								sin(0) * 3.3f);

		
		D3DVECTOR point = {x, y, z};
		uniMatrixMultiplyVector(point, point, rotate);

//		scene->object[0]->vertex[i].x = point.x + (cos((timer / 1100.f) + (dist * 0.4f)) * 1.f);
//		scene->object[0]->vertex[i].y = point.y + (cos((timer / 800.f) + (dist * 0.4f)) * 1.f);
//		scene->object[0]->vertex[i].z = point.z + (cos((timer / 1500.f) + (dist * 0.4f)) * 1.f);
		scene->object[0]->vertex[i].x = point.x + (cos((timer / 630.f) + (dist * 0.4f)) * 1.f);
		scene->object[0]->vertex[i].y = point.y + (cos((timer / 600.f) + (dist * 0.4f)) * 1.f);
		scene->object[0]->vertex[i].z = point.z + (cos((timer / 550.f) + (dist * 0.4f)) * 1.f);
	}


	scene->camera->setLoc(sin(timer / 6000.f) * 4.f, 0, cos(timer / 6000.f) * 5.f);
	scene->light[0]->setLoc(sin(timer / 6000.f) * 4.f, 2, cos(timer / 6000.f) * 5.f);


	float timerTemp, x, y;
	switch (currentCam)
	{
	case 1:
		timerTemp = timer - camTimer1;
		if (timerTemp < 2000)
		{
			x = 4.f - (sin(timerTemp * _PI / 4000.f) * 4.f);
			y = 4.f - (sin(timerTemp * _PI / 4000.f) * 4.f);

			scene->object[0]->setLoc(x, y, 0);
		}
		break;
	case 2:
		timerTemp = timer - camTimer1;
		if (timerTemp < 2000)
		{
			x = 4.f - (cos(timerTemp * _PI / 4000.f) * 4.f);
			y = 4.f - (cos(timerTemp * _PI / 4000.f) * 4.f);

			scene->object[0]->setLoc(x, y, 0);
		}
		break;
	}

//	scene->object[0]->transMode = UNI_ALPHA;
//	scene->object[0]->setDiffuse(1, 1, 1, 0.5f - cos(timer / 7000.f) * 0.5f);
/*
	float alpha = 0.1f;

	if ((timer % 880) < 500)
	{
		alpha = 1.0f - sin((timer % 880) * _PI / 1000.f) * 0.9f;
	}


	scene->object[0]->setDiffuse(1, 1, 1, alpha);
*/
	scene->camera->aspect = 0.6f;
	scene->render();







	SpriteDrawer *sd = uniSystem->spriteDrawer;


	uniSetTexture("distort5");

	float x1 = uniSystem->viewportX1;
	float y1 = uniSystem->viewportY1;
	float x2 = uniSystem->viewportX2;
	float y2 = uniSystem->viewportY2;
	
	float uadd = sin(timer / 10000.f) * 1.f;
	float vadd = cos(timer / 10000.f) * 1.f;

	int pos, row, sync;

	fmodGetInfo(pos, row, sync);

	int fade = 80 - row * 7;

	if (pos == 19)
	{
		if (row > 24)
			fade = (row - 24) * 6;
		else
			fade = 0;
	}

	if ((fade > 0))
	{
		int c = (fade << 16) | (fade << 8) | fade;
		sd->Start(UNI_ADD);
		sd->Draw2D(x1, y1, 0 + uadd, 0 + vadd, c,
				   x2, y1, 8 + uadd, 0 + vadd, c,
				   x1, y2, 0 + uadd, 8 + vadd, c,
				   x2, y2, 8 + uadd, 8 + vadd, c);
		sd->Stop();
	}
}

void Sphery03::Receive(dword Message, dword timer)
{
	int i;
	switch (Message & 255)
	{
	case 2:
		currentCam = 2;
		camTimer1  = timer;
		break;
	}
}

void Sphery03::DeInit() 
{
	delete scene;
	delete oldObject;
}
