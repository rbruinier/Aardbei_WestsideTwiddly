/*

	Pyramid DX8 Engine - Light code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __LIGHT_H__
#define __LIGHT_H__

extern int lightNumber;

class Light {
public:
	char				name[50];

	int					handle;
	D3DLIGHT8			light;
	float				size;
	bool				calc;
	bool				draw;

//	Sprite				*sprite;

public:
	Light(float r = 1, float g = 1, float b = 1, float a = 1);
	~Light();

public:
	render();
	activate();
	deactivate();
	setLoc(float x, float y, float z);
	setTar(float x, float y, float z);
	setAttenuation(float a0, float a1, float a2);
	setAmbient(float r, float g, float b, float a);
	setDiffuse(float r, float g, float b, float a);
	setSpecular(float r, float g, float b, float a);
	setSpot(float falloff, float theta, float phi);
	setType(D3DLIGHTTYPE type);
	setRange(float range);
};

#endif