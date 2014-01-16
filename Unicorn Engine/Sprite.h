/*

	Pyramid DX8 Engine - Sprite code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.
		- 11-01-02 (rob):
			Changed SpriteGroup.. now it can be used to make child classes (for particle systems)


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __SPRITE_H__
#define __SPRITE_H__
/*
class Sprite {
public:
	float		size;
	float		xLoc;
	float		yLoc;
	float		zLoc;
	dword		color;

private:
	char		*texture;

public:
	Sprite(float size, dword color, char * texture);
	~Sprite();

public:
	void render();
	void setSize(float size);
	void setLoc(float x, float y, float z);
	void setColor(dword color);
	void setTexture(char * naam);
};
*/

class SpriteGroup {
public:
	SpriteGroup(int nrSprites, float size, dword color, char * texture);
	~SpriteGroup();

	void render();
	void setSize(const float size);
	void setTexture(char * naam);

	int			nrSprites;
	VertexPoint *sprite;

	float		size;

	bool		enabled;

	int			blend;

protected:
	SpriteGroup();
	UniVBuffer	*vertexBuffer;
	char		*texture;
};

#endif

