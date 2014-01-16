/*

	Pyramid DX8 Engine - Camera code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __Camera_h__
#define __Camera_h__

class Camera {
public:
	float		xLoc;
	float		yLoc;
	float		zLoc;

	float		xTar;
	float		yTar;
	float		zTar;

	float		roll;

	float		fov;
	float		aspect;

	float		farPlane;
	float		nearPlane;


public:
	Camera();
	~Camera();

public:
	update();
	setLoc(float x, float y, float z);
	setTar(float x, float y, float z);
	setRoll(float r);
};

#endif

