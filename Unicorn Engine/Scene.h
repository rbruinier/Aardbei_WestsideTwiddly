/*

	Pyramid DX8 Engine - Scene code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __SCENE_H__
#define __SCENE_H__

class Scene {
public:
	int		nrObjects;
	int		nrLights;
	//int		nrSprites;
	int		nrSpriteGroups;

	Object	**object;
	Light   **light;
	//Sprite	**sprite;
	SpriteGroup **spriteGroup;
	Camera	*camera;

	long	ambient;

	bool	fogSprites;
	bool	fog;
	bool    fogPixel;
	long	fogColor;
	float	fogStart;
	float	fogEnd;
	float	fogDensity;
	DWORD   fogMode;

public:
	Scene();
	~Scene();

public:
	void render();
	void renderSprites();
	void finish();
	void addObject(Object * obj);
	void addLight(Light * lht);
	//void addSprite(Sprite * spr);
	void addSpriteGroup(SpriteGroup * spt);
	Object * findObject(char * naam);
};

#endif
