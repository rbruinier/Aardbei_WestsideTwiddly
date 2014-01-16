/*

	Pyramid DX8 Engine - Light code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "Engine.h"

int lightNumber = 0;

Light::Light(float r, float g, float b, float a) {
	handle = lightNumber++;

    ZeroMemory( &light, sizeof(light) );

	setType(D3DLIGHT_POINT);
	setDiffuse(r, g, b, a); 
	setAmbient(0, 0, 0, 0);
	setRange((float)sqrt(FLT_MAX));
	setSpecular(r, g, b, a);
    setAttenuation(1, 0, 0);

	uniSetLightInfo(handle, light);

	size = 1;

	int color = ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | ((int)b * 255) | 0xff000000;

	calc = true;
	draw = false;

//	this->sprite = new Sprite(0.6f, color, "lens02");
}

Light::~Light() {
//	delete sprite;
}

Light::activate() {
	if (calc) {
		uniSetLightInfo(handle, light);
		uniSetLightState(handle, true);
	}
}

Light::deactivate() {
	if (calc)
		uniSetLightState(handle, false);
}

Light::render() {
//	if (draw)
//		this->sprite->render();
}


Light::setLoc(float x, float y, float z) {
	light.Position.x = x;
	light.Position.y = y;
	light.Position.z = z;

//	if (draw)
//		sprite->setLoc(x, y, z);
}

Light::setTar(float x, float y, float z) {
	light.Direction.x = x;
	light.Direction.y = y;
	light.Direction.z = z;
}

Light::setAttenuation(float a0, float a1, float a2) {
	light.Attenuation0 = a0;
	light.Attenuation1 = a1;
	light.Attenuation2 = a2;
}

Light::setAmbient(float r, float g, float b, float a) {
	light.Ambient.r = r;
	light.Ambient.g = g;
	light.Ambient.b = b;
	light.Ambient.a = a;
}

Light::setDiffuse(float r, float g, float b, float a) {
	light.Diffuse.r = r;
	light.Diffuse.g = g;
	light.Diffuse.b = b;
	light.Diffuse.a = a;
}

Light::setSpecular(float r, float g, float b, float a) {
	light.Specular.r = r;
	light.Specular.g = g;
	light.Specular.b = b;
	light.Specular.a = a;
}

Light::setSpot(float falloff, float theta, float phi) {
	light.Falloff = falloff;
	light.Theta   = theta;
	light.Phi     = phi;
}

Light::setType(D3DLIGHTTYPE type) {
	light.Type = type;
}

Light::setRange(float range) {
	light.Range = range;
}