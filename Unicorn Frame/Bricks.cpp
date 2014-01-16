#include "Main.h"

#define BRICKS_NRSPRITES 40
#define BRICKS_MAXHEIGHT 50

//#define BRICKS_WALK
#define BRICKS_SHAKE

enum BrickStatus
{
	BRICK_BRICK   = 1,
	BRICK_HOLE    = 2,
	BRICK_WITH    = 3,
	BRICK_WITHOUT = 4
};

class Brick
{
public:
	Brick(BrickStatus iStatus, float radius)
	{
		base = genPlane(1, radius, NULL);

		setStatus(iStatus);

		if (status == BRICK_WITH)
		{
			part = genPlane(1, radius, "bricks_brick");
			part->method = METHOD_NONE;
			part->cull = CULL_NONE;
			part->alphatest = true;

			#ifdef BRICKS_NRSPRITES
				sprites = new SpriteGroup(BRICKS_NRSPRITES, 0.1f, 0xffffffff, "sprite01");
				sprites->enabled = false;
			#endif
		}
		else
		{
			part = NULL;
		}

		explode = false;
	}
	~Brick()
	{
	}

	void setLoc(float x, float y, float z)
	{
		loc.x = x;
		loc.y = y;
		loc.z = z;
		base->setLoc(x, y, z);
	}

	void setStatus(BrickStatus iStatus)
	{
		status = iStatus;
		switch (status)
		{
			case BRICK_BRICK:
				base->setTexture1("bricks_brick");
				break;
			case BRICK_HOLE:
				base->setTexture1("bricks_hole");
				break;
			case BRICK_WITH:
				base->setTexture1("bricks_with");
				break;
			case BRICK_WITHOUT:
				base->setTexture1("bricks_without");
				break;
		}
	}

	void doExplode(dword timer)
	{
		if (!explode)
		{
			explodeTimer = timer;
			if (status != BRICK_WITH)
			{
				return;
			}
			part->method = METHOD_SOLID;
			base->setTexture1("bricks_hole");
			base->alphatest = true;

			#ifdef BRICKS_NRSPRITES
				for (int i = 0; i < BRICKS_NRSPRITES; i++)
				{
					sprites->sprite[i].setLoc(loc.x + (((rand()%100) - 50) / 200.f),
											  (-rand()%200) / 25.f,
											  loc.z + (((rand()%100) - 50) / 200.f));
				}			
			#endif

			sprites->enabled = true;

			spriteLastTimer = timer;
		}	

		explode = true;
	}

	void update(dword timer, int seed)
	{
		if (!explode)
		{
			return;
		}

		float t = timer - explodeTimer;

		float x = loc.x;
		float y = loc.y;
		float z = loc.z;

#ifdef BRICKS_SHAKE
		if (t < 1000)
		{
			x += sin(t / 10.f) * 0.02f;
			z += cos(t / 11.f) * 0.02f;
			part->setLoc(x, 0, z);
			spriteLastTimer = timer;
			sprites->enabled = false;
			return;
		}
		else
		{
			sprites->enabled = true;
		}

		t -= 1000;
#endif

		y += t / 100.f;

		part->setLoc(x, y, z);

		float rotx = t / 6.f;
		float roty = 0;
		float rotz = t / 7.f;
		if (seed & 1)
			rotx = -rotx;
		if (seed & 3)
			rotz = -rotz;
		if (seed & 7)
		{
			rotx /= 2.f;
			rotz /= 1.4f;
		}
		if (seed & 15)
		{
			roty = t / 3.f;
		}
		part->setRot(rotx, 0, rotz);

		if (y > BRICKS_MAXHEIGHT)
		{
			part->method = METHOD_NONE;
			sprites->enabled = false;
			return;
		}


		#ifdef BRICKS_NRSPRITES
			for (int i = 0; i < BRICKS_NRSPRITES; i++)
			{
				x = sprites->sprite[i].x;
				y = sprites->sprite[i].y;
				z = sprites->sprite[i].z;
				float scale = i / (float)BRICKS_NRSPRITES;
				y += ((timer - spriteLastTimer) / 220.f) * (scale + 1.0f);
				sprites->sprite[i].setLoc(x, y, z);
			}
			spriteLastTimer = timer;
		#endif

	}

public:
	UniVector   loc;
	BrickStatus status;
	Object *base;			// op "dambord"
	Object *part;			// vliegend object;

	#ifdef BRICKS_NRSPRITES
		SpriteGroup *sprites;
		dword spriteLastTimer;
	#endif

	dword explodeTimer;
	bool explode;
};


class BrickSystem
{
public:
	BrickSystem(int nrBricksX, int nrBricksZ, float radiusOfBrick)
	{
		this->nrBricksX     = nrBricksX;
		this->nrBricksZ     = nrBricksZ;
		this->radiusOfBrick = radiusOfBrick;
		bricks = new Brick*[nrBricksX * nrBricksZ];
		for (int z = 0; z < nrBricksZ; z++)
		{
			for (int x = 0; x < nrBricksX; x++)
			{
				BrickStatus status = BRICK_WITH;
				if ((x ^ z) & 1)
				{
					status = BRICK_WITHOUT;
				}
				bricks[(z * nrBricksX) + x] = new Brick(status, radiusOfBrick);
				bricks[(z * nrBricksX) + x]->setLoc((x * radiusOfBrick) - ((nrBricksX - 1) * (radiusOfBrick / 2.f)), 
										            0, 
													(z * radiusOfBrick) - ((nrBricksZ - 1) * (radiusOfBrick / 2.f)));

			}
		}
	}
	~BrickSystem()
	{
	}

	void update(dword timer)
	{
		for (int i = 0; i < nrBricksX * nrBricksZ; i++)
		{
			bricks[i]->update(timer, i);
		}
	}

public:
	int nrBricksX;
	int nrBricksZ;
	float radiusOfBrick;
	Brick **bricks;

};

BrickSystem *brickSystem;

bool Bricks::Init() 
{
	brickSystem = new BrickSystem(10, 10, 3);

	scene = new Scene();

	int index = 0;
	for (int z = 0; z < brickSystem->nrBricksZ; z++)
	{
		for (int x = 0; x < brickSystem->nrBricksX; x++)
		{
				scene->addObject(brickSystem->bricks[index++]->base);
		}
	}

	
	for (int i = 0; i < index; i++)
	{
		if (brickSystem->bricks[i]->part != NULL)
		{
			scene->addObject(brickSystem->bricks[i]->part);
//			#ifdef BRICKS_NRSPRITES
				scene->addSpriteGroup(brickSystem->bricks[i]->sprites);
//			#endif
		}
	}


	scene->fog = true;
	scene->fogEnd = 30.f;
	scene->fogSprites = true;
/*
	scene->addLight(new Light(1, 1, 1, 1));
	scene->ambient = 0x202020;
	scene->light[0]->setLoc(-10, 5, 0);
*/
	return true;
}   


bool exploded = true;

void Bricks::Do(dword timer) 
{
	int speed =  200;
	if (timer < 5000)
		speed = 1000 - (sin(timer * _PI / 10000.f) * 800.f);

	if ((timer % speed) > (speed >> 1))
	{
		exploded = false;
	}
	else
	{
		if (!exploded)
		{
			brickSystem->bricks[rand()%100]->doExplode(timer);
			exploded = true;
		}
	}

	brickSystem->update(timer);

#ifdef BRICKS_WALK

	float x, y, z;

	timer += 0;

	x = sin(timer / 8000.f) * 7.f;
	y = 1.0f + sin(timer / 500.f) * 0.1f;
	z = sin(timer / 11000.f) * 7.f;

	scene->camera->setLoc(x, y, z);

	x = sin((timer + 2000.f) / 8000.f) * 7.f;
	y = 0.8f;
	z = sin((timer + 2000.f) / 11000.f) * 7.f;

	scene->camera->setTar(x, y, z);

#else

	scene->camera->setLoc(sin(timer / 7000.f) * 15.f, 1, cos(timer / 10000.f) * 15.f);

#endif

	scene->render();
}

void Bricks::DeInit() 
{
	delete scene;
}
