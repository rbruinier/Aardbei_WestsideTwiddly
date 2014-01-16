/*

	Pyramid DX8 Engine - Object Generator code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __GENERATE_H__
#define __GENERATE_H__

#ifdef UNI_GENERATOR_CUBE
Object * genCube(int xseg1, float radius, char * texture);
#endif

#ifdef UNI_GENERATOR_SPHERE
Object * genSphere(int xseg1, float radius, char * texture);
#endif

#ifdef UNI_GENERATOR_SPHEREOLD
Object * genSphereOld(int segments, float radius, char * texture = NULL);
#endif

#ifdef UNI_GENERATOR_TUBE
Object * genTube(int segments, int layers, float radius, float length, char * texture = NULL);
#endif

#ifdef UNI_GENERATOR_TUBEOLD
Object * genTubeOld(int segments, int layers, float radius, float length, char * texture = NULL);
#endif

#ifdef UNI_GENERATOR_PLANE
Object * genPlane(int xseg1, float radius, char * texture = NULL);
#endif

#ifdef UNI_GENERATOR_TOROID
Object * genToroid(int numsegs, int numrings, float r0, float r1, char * texture = NULL);
Object * genToroid(int numsegs, int numrings, float r0, float r1, float n1, float n2, char * texture = NULL);
#endif


#endif
