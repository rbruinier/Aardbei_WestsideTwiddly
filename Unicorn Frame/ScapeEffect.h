#ifndef __SCAPEEFFECT_H__
#define __SCAPEEFFECT_H__

class ScapeEffect: public Effect 
{
public:
	virtual bool Init() = 0;
	virtual void Do(dword) = 0;
	virtual void DeInit() = 0;


protected:
	typedef void (*EXTRASHAPE_GEN) (float *heightMap);

	void CreateShape(EXTRASHAPE_GEN ExtraFunction, Vertex *vertex, float fSoftness, 
		             float fDensity, long seed);
	bool GetHeightAtPosition(Vertex *vertex, float x, float z, float &height);
	dword *CreateTexture(int width, int height, dword *texture1, dword *texture2, dword *texture3,
			             dword *texture4, float startY, float scaleY);
	long GenSemiRand(long _x, long &adder);

	Scene	*scene;
	int nrSegmentsX;
	int nrSegmentsZ;
	float left, right;
	float top, bottom;
};

#endif