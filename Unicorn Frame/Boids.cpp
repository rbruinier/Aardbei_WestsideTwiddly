/*

	Boids voorlopig alleen particles (geen rekening houdend met rotatie bv)

*/

#include "Main.h"

#define rnd() (((FLOAT)rand() ) / RAND_MAX)

UniVector follow;

class Boid
{
public:
	Boid();
	~Boid();

	void Update(Boid *boids, int nrBoids);

	UniVector	location;
	UniVector	velocity;
	UniVector	acceleration;

	float maxVelocity;
	float maxAcceleration;
};

Boid::Boid()
{
	location.x = (rnd() * 50.f) - 25.f;
	location.y = (rnd() * 10.f) +  1.f;
	location.z = (rnd() * 50.f) - 25.f;

	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;

	acceleration.x = 0;
	acceleration.y = 0;
	acceleration.z = 0;

	maxAcceleration = 0.005f;
	maxVelocity = 0.2f;
}

Boid::~Boid()
{
}

void Boid::Update(Boid *boids, int nrOfBoids)
{

//      D3DVECTOR vGoal = 0.5f * Normalize(m_vGoal-m_pBoids[i].vLoc );
        //vDelta += vGoal;

	acceleration += 0.6f * Normalize(follow - location);


	if (acceleration.x > maxAcceleration)
		acceleration.x = maxAcceleration;
	if (acceleration.y > maxAcceleration)
		acceleration.y = maxAcceleration;
	if (acceleration.z > maxAcceleration)
		acceleration.z = maxAcceleration;

	if (acceleration.x < -maxAcceleration)
		acceleration.x = -maxAcceleration;
	if (acceleration.y < -maxAcceleration)
		acceleration.y = -maxAcceleration;
	if (acceleration.z < -maxAcceleration)
		acceleration.z = -maxAcceleration;

	velocity += acceleration;

	if (velocity.x > maxVelocity)
		velocity.x = maxVelocity;
	if (velocity.y > maxVelocity)
		velocity.y = maxVelocity;
	if (velocity.z > maxVelocity)
		velocity.z = maxVelocity;

	if (velocity.x < -maxVelocity)
		velocity.x = -maxVelocity;
	if (velocity.y < -maxVelocity)
		velocity.y = -maxVelocity;
	if (velocity.z < -maxVelocity)
		velocity.z = -maxVelocity;

	location += velocity;
}


class BoidSystem
{
public:
	BoidSystem(int iNrBoids);
	~BoidSystem();

	void Update();

	int nrBoids;

	Boid *boids;
};

BoidSystem::BoidSystem(int iNrBoids)
{
	nrBoids = iNrBoids;

	boids = new Boid[iNrBoids];
}

BoidSystem::~BoidSystem()
{
	delete [] boids;
}

void BoidSystem::Update()
{
	for (int i = 0; i < nrBoids; i++)
	{
		boids[i].Update(boids, nrBoids);
	}
}




#define NR_BOIDS 100

BoidSystem *boidSystem;

bool Boids::Init() 
{
	boidSystem = new BoidSystem(NR_BOIDS);

	scene = new Scene();

	scene->addObject(genPlane(2, 50.f, "texture02"));
	scene->addSpriteGroup(new SpriteGroup(NR_BOIDS, 0.3f, 0xffffff, "sprite01"));

	scene->addObject(genSphere(4, 1.f, "texture02"));

	scene = new Scene();
	scene->addObject(genCube(1, 2.f, "texture02"));

	return true;
}   

void Boids::Do(dword timer) 
{

	follow.x = sin(timer / 200.f) * 15.f;
	follow.y = cos(timer / 2300.f) * 5.f + 6.f;
	follow.z = sin(timer / 320.f) * 15.f;

	boidSystem->Update();

	for (int i = 0; i < NR_BOIDS; i++)
	{
		scene->spriteGroup[0]->sprite[i].setLoc(boidSystem->boids[i].location.x,
			                                    boidSystem->boids[i].location.y,
												boidSystem->boids[i].location.z);
	}

	scene->camera->setLoc(sin(timer / 4000.f) * 30.f, 6, cos(timer / 4000.f) * 35.f);

	scene->object[1]->setLoc(follow.x, follow.y, follow.z); 

	scene->render();
}

void Boids::DeInit() 
{
	delete scene;
	delete boidSystem;
}

