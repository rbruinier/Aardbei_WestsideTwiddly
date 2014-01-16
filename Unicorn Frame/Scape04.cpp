#include "Main.h"

void Scape04CreateFlatSurface(float *heightMap)
{
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			float height = heightMap[(y << 8) + x];

			float length = sqrt(((x - 127.f) * (x - 127.f)) + ((y - 127.f) * (y - 127.f)));

				if (height < 50)
					height = 50;

			heightMap[(y << 8) + x] = height;
		}
	}
}

bool Scape04::Init() {
	nrSegmentsX = 127;
	nrSegmentsZ = 127;
	left   = -150;
	top    = -150;
	right  =  150;
	bottom =  150;

	scene = new Scene();

	scene->addObject(genPlane(nrSegmentsX, 300, "landschap2"));
	scene->addObject(genPlane(30, 300, "fsolGrond01"));
	scene->object[1]->scaleUV(100, 100);
	scene->object[0]->setLoc(0, -2, 0);
	scene->object[1]->setLoc(0, 1, 0);
//	scene->object[0]->method = METHOD_NONE;

	CreateShape(Scape04CreateFlatSurface, scene->object[0]->vertex, 4.f, 1.0f, 2345);

	int width, height;	
	dword *grond2 = NULL;//loadATSFromMem(ATS_GROEN, width, height);
	dword *grond1 = NULL;//loadATSFromMem(ATS_BROWN, width, height);
	dword *grond3 = NULL;//loadATSFromMem(ATS_GRIJS, width, height);
	dword *grond4 = loadATSFromMem(ATS_WIT,   width, height);

	dword *textureFinal = CreateTexture(1024, 1024, grond1, grond1, grond3, grond4, 0.0f, 2.f);

	uniTextureCreate("landschap2", textureFinal, 1024, 1024, UNI_MIPMAP);
	uniTextureCreate("fsolGrond01", grond3, 256, 256, UNI_MIPMAP4);

	scene->fog = true;
	scene->fogEnd = 140;
	scene->fogStart = 90;
	scene->fogColor = 0x0;

	scene->addLight(new Light(1, 1, 1, 1));

	scene->ambient = 0x303030;

	return true;
}   



void Scape04::Do(dword timer) {
	uniSetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

	//scene->camera->setLoc(sin(timer / 4000.f) * 30.f, 3, cos(timer / 4000.f) * 30.f);
	scene->camera->setTar(-5, 1.4f, sin(timer / 4000.f) * 10.f); 
	scene->camera->setLoc(0, 1.5, 0);
	
	scene->light[0]->setLoc(200, 40, 200);

	scene->render();


	uniSetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
}

void Scape04::DeInit() {
	delete scene;
};
