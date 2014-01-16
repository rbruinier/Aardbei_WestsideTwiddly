/*

	Pyramid DX8 Engine - Engine Setup File


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/
 
#ifndef __ENGSETUP_H__
#define __ENGSETUP_H__

/* - GENERATE ------------------------------------------------------------------------------ */

//#define UNI_GENERATOR_CUBE
#define UNI_GENERATOR_SPHERE
//#define UNI_GENERATOR_SPHEREOLD
//#define UNI_GENERATOR_TOROID
//#define UNI_GENERATOR_TUBE
//#define UNI_GENERATOR_TUBEOLD
#define UNI_GENERATOR_PLANE


#ifdef UNI_GENERATOR_SPHERE
#ifndef UNI_GENERATOR_CUBE
#define UNI_GENERATOR_CUBE
#endif
#endif

#endif