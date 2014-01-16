/*

	Pyramid DX8 Engine - Object code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __OBJECT_H__
#define __OBJECT_H__


class Object {
public:
	typedef void (WINAPI *UNI_CUSTOMIZE) ();

	char			name[50];

	int				nrVertices;
	int				nrFaces;

	Face			*face;
	Vertex			*vertex;
	UniVBuffer		*vBuffer;
	UniIBuffer		*iBuffer;

	int				*normalCounter;

	CULL_MODE		cull;
	dword			shade;
	dword			method;
	dword			transMode;


private:
	UNI_CUSTOMIZE	customCode;

	bool			customCodeSet;
	bool			dynamic;
	bool			finished;
	bool			updated;
	bool			specular;

	char			*texture1;
	char			*texture2;

public:
	bool			alphatest;
	dword			alphatestRef;
	dword			alphatestFunction;

	bool			writeZBuffer;

	int				indexUV1;
	int				indexUV2;

	float			xDeg;
	float			yDeg;
	float			zDeg;

	float			xLoc;
	float			yLoc;
	float			zLoc;

	float			xScl;
	float			yScl;
	float			zScl;


	D3DMATERIAL8	material;

	D3DMATRIX		objMatrix;
 
private:
	void update();

public:
	void render();
	void finish(bool dynamic);

	void setLoc(float x, float y, float z);
	void setRot(float x, float y, float z);
	void setScl(float x, float y, float z);

	void setDiffuse(float r, float g, float b, float a = 1.0f);
	void setAmbient(float r, float g, float b, float a = 1.0f);
	void setEmissive(float r, float g, float b, float a = 1.0f);
	void setSpecular(float r, float g, float b, float a, float p);

	void setTexture1(char *naam);
	void setTexture2(char *naam);

	char *getTexture1();
	char *getTexture2();

	void setCustomCode(UNI_CUSTOMIZE function);

	void normals();
	void envmap();

	void scaleUV(float scaleu, float scalev);

	void optimize(float dif = 0);

public:
	Object(int nrVertices, int nrFaces, char *texture = NULL);
	~Object();
};

#endif
