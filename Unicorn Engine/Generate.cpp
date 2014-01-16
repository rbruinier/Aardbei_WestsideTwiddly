/*

	Pyramid DX8 Engine - Object Generator code (source file)


	(c) 2001, Robert Jan Bruinier

*/


#include "Engine.h"


#ifdef UNI_GENERATOR_CUBE

Object * genCube(int xseg1, float radius, char * texture) {
	Object * object;

    int i,j,k;
 
    int xseg2 = xseg1 + 1;
 
    float xseguv = 0.99f / (float) xseg1;

    int nr_verts  = (xseg2 * xseg2) *  6;
    int nr_faces  = (xseg1 * xseg1) * 12;

	object = new Object(nr_verts, nr_faces, texture);
 
    int v = 0;
    int f = 0;

    int adder = xseg2 * xseg2;

	Vertex  * vertex = object->vertex;
	Face	* facep  = object->face;

    for (j = 0; j < xseg2; j++) {
        for (i = 0; i < xseg2; i++) {
            float x = (i * (radius/xseg1)) - (radius/2.f);
            float y = (j * (radius/xseg1)) - (radius/2.f);
            float z = radius / 2.f;
			float uu = xseguv * (float) i;
			float vv = xseguv * (float) j;

            vertex[v].x = x;
            vertex[v].y = y;
            vertex[v].z = z;
			vertex[v].tu1 = uu;
			vertex[v].tv1 = vv;
            vertex[v+adder].x =  x;
            vertex[v+adder].y =  y;
            vertex[v+adder].z = -z;
			vertex[v+adder].tu1 = uu;
			vertex[v+adder].tv1 = vv;

            vertex[v+(adder * 2)].x = -z;
            vertex[v+(adder * 2)].y =  y;
            vertex[v+(adder * 2)].z =  x;
            vertex[v+(adder * 2)].tu1 =  uu;
            vertex[v+(adder * 2)].tv1 =  vv;
            vertex[v+(adder * 3)].x =  z;
            vertex[v+(adder * 3)].y =  y;
            vertex[v+(adder * 3)].z =  x;
            vertex[v+(adder * 3)].tu1 =  uu;
            vertex[v+(adder * 3)].tv1 =  vv;

            vertex[v+(adder * 4)].x =  y;
            vertex[v+(adder * 4)].y =  z;
            vertex[v+(adder * 4)].z =  x;
            vertex[v+(adder * 4)].tu1 =  uu;
            vertex[v+(adder * 4)].tv1 =  vv;
            vertex[v+(adder * 5)].x =  y;
            vertex[v+(adder * 5)].y = -z;
            vertex[v+(adder * 5)].z =  x;
            vertex[v+(adder * 5)].tu1 =  uu;
            vertex[v+(adder * 5)].tv1 =  vv;

            v++;
        }
    }

	for (k = 0; k < 3; k++) {
	    int adder2 = xseg1 * xseg1 * 2;
	    int adder3 = xseg2 * xseg2;
		f = adder2 * 2 * k;
		int adder4 = adder3 * 2 * k;
		for (j = 0; j < xseg1; j++) {
			for (i = 0; i < xseg1; i++) {
	            int a = ((j) * xseg2) + i;
		        int b = ((j) * xseg2) + i + 1;
			    int c = ((j+1) * xseg2) + i;
				int d = ((j+1) * xseg2) + i + 1;
	            int u1 = (int) (i * xseguv);
		        int v1 = (int) (j * xseguv);
			    int u2 = (int) ((i+1) * xseguv);
				int v2 = (int) (j * xseguv);
	            int u3 = (int) (i * xseguv);
		        int v3 = (int) ((j+1) * xseguv);
			    int u4 = (int) ((i+1) * xseguv);
				int v4 = (int) ((j+1) * xseguv);
				a += adder4;
				b += adder4;
				c += adder4;
				d += adder4;

				facep[f].a = a;
				facep[f].b = d;
				facep[f].c = b;
				facep[f+adder2].a = a+adder3;
				facep[f+adder2].b = b+adder3;
				facep[f+adder2].c = d+adder3;
			    f++;

				facep[f].a = a;
				facep[f].b = c;
				facep[f].c = d;
				facep[f+adder2].a = a+adder3;
				facep[f+adder2].b = d+adder3;
				facep[f+adder2].c = c+adder3;
			    f++;
	        }
		}
	}

	for (i = 0; i < object->nrVertices; i++) {
		object->vertex[i].tu2 = object->vertex[i].tu1;
		object->vertex[i].tv2 = object->vertex[i].tv1;
	}

	return object;
}

#endif

#ifdef UNI_GENERATOR_SPHERE

Object * genSphere(int xseg1, float radius, char * texture) {
	Object * object = genCube(xseg1, radius, texture);

	radius /= 2.f;
	Vertex * v = object->vertex;
	for (int i = 0; i < object->nrVertices; i++) {
		float length = (float) sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
		v->x /= length;
		v->y /= length;
		v->z /= length;
		v->x *= radius;
		v->y *= radius;
		v->z *= radius;
		v++;
	}

	return object;
}

#endif

#ifdef UNI_GENERATOR_SPHEREOLD

// deze kan je dus beter niet gebruiken... nog uit alienation tijdperk ;)
Object * genSphereOld(int segments, float radius, char * texture) {
	int i,j;
	float y,r;
	int nr_vertexes = segments * (segments + 1);
	int nr_faces = (segments-1) * (segments-1) * 2;
	float xstep = 2.f * (float)_PI / (segments - 1);
	float ystep = 1.f * (float)_PI / (segments - 1);
	float rstep = 1.f * (float)_PI / (segments - 1);

	Object * obj = new Object(nr_vertexes, nr_faces, texture);   


	for (j = 0; j < segments; j++) {
		y = (float) cos(ystep * j) * radius;
		r = (float) sin(rstep * j) * radius;
		for (i = 0; i < segments+1; i++) {
			obj->vertex[(j * (segments+1)) + i].x = (float)cos(xstep * i) * r;
			obj->vertex[(j * (segments+1)) + i].y = y;
			obj->vertex[(j * (segments+1)) + i].z = (float)sin(xstep * i) * r;
			obj->vertex[(j * (segments+1)) + i].tu1 = i * (1.f / (segments-1));
 			obj->vertex[(j * (segments+1)) + i].tv1 = j * (1.f / (segments-1));
			obj->vertex[(j * (segments+1)) + i].tu2 = i * (1.f / (segments-1));
 			obj->vertex[(j * (segments+1)) + i].tv2 = j * (1.f / (segments-1));
		}
	}

	int facenr = 0;
	for (j = 0; j < segments - 1; j++) {
		for (i = 0; i < segments - 1; i++) {
			int i1 = (i);
			int i2 = (i + 1);

			obj->face[facenr].a = (j * (segments+1)) + i1;
			obj->face[facenr].b = (j * (segments+1)) + (segments+1) + i1;
			obj->face[facenr].c = (j * (segments+1)) + i2;

			facenr++;

			obj->face[facenr].a = (j * (segments+1)) + i2;
			obj->face[facenr].b = (j * (segments+1)) + (segments+1) + i1;
			obj->face[facenr].c = (j * (segments+1)) + (segments+1) + i2;

			facenr++;
		}
	}

   return obj;
}

#endif

#ifdef UNI_GENERATOR_TUBEOLD

Object * genTubeOld(int segments, int layers, float radius, float length, char * texture) {
	int i,j;
	Object * object = new Object(segments * layers,(segments - 1) * (layers - 1) * 2, texture);

	Vertex * vertex = object->vertex;

	for (j = 0; j < layers; j++) {
		for (i = 0; i < segments; i++) {
			vertex->x = (float) sin(i * ((2.f * _PI) / (segments-1))) * radius;
			vertex->y = (j * (length / (layers))) - (length / 2.f);
			vertex->z = (float) cos(i * ((2.f * _PI) / (segments-1))) * radius;
			vertex->tu1 = (1.0f / (segments-1)) * i;
			vertex->tv1 = (1.0f / layers) * j;
			vertex->tu2 = (1.0f / (segments-1)) * i;
			vertex->tv2 = (1.0f / layers) * j;
			vertex++;
		}
	}

	Face * facep   = object->face;

	float vstep = 256.f / (layers - 1);
	float ustep = 256.f / segments;

	for (j = 0; j < layers - 1; j++) {
		for (i = 0; i < segments - 1; i++) {
			int b = j*segments;
			int o = (j+1)*segments;
			int l = i;
			int r = (i + 1) % segments;
			int lb = b + l;
			int rb = b + r;
			int lo = o + l;
			int ro = o + r;

			facep->a = lb;
			facep->b = lo;
			facep->c = rb;
			facep++;

			facep->a = rb;
			facep->b = lo;
			facep->c = ro;
			facep++;
		}	
	}

	return object;
}

#endif

#ifdef UNI_GENERATOR_TUBE

Object * genTube(int segments, int layers, float radius, float length, char * texture) {
	int i,j;
	segments++;
	layers++;
	Object * object = new Object(segments * layers,(segments - 1) * (layers - 1) * 2, texture );

	Vertex * vertex = object->vertex + (layers * segments) - 1;

	for (j = 0; j < layers; j++) {
		for (i = 0; i < segments; i++) {
			vertex->x = (float) sin(i * ((2.f * _PI) / (segments-1))) * radius;
			vertex->y = (j * (length / (layers-1))) - (length / 2.f);
			vertex->z = (float) cos(i * ((2.f * _PI) / (segments-1))) * radius;
			vertex->tu1 = (1.0f / (segments-1)) * i;
			vertex->tv1 = (1.0f / (layers-1)) * j;
			vertex->tu2 = (1.0f / (segments-1)) * i;
			vertex->tv2 = (1.0f / (layers-1)) * j;
			vertex--;
		}
	}

	Face * facep   = object->face;

	for (j = 0; j < layers - 1; j++) {
		for (i = 0; i < segments - 1; i++) {
			int b = j*segments;
			int o = (j+1)*segments;
			int l = i;
			int r = (i + 1) % segments;
			int lb = b + l;
			int rb = b + r;
			int lo = o + l;
			int ro = o + r;

			facep->a = lb;
			facep->b = lo;
			facep->c = rb;
			facep++;

			facep->a = rb;
			facep->b = lo;
			facep->c = ro;
			facep++;
		}	
	}

	return object;
}

#endif


#ifdef UNI_GENERATOR_PLANE

Object * genPlane(int xseg1, float radius, char * texture) {
	int i,j;
	Object * object;

	int xseg2 = xseg1 + 1;

	object = new Object(xseg2 * xseg2, xseg1 * xseg1 * 2, texture);

	Vertex * vertex = object->vertex;

	for (i = 0; i < xseg2; i++) {
		for (j = 0; j < xseg2; j++) {
			vertex->x   = ((radius / xseg1) * j) - (radius / 2.f);
			vertex->y   = 0;
			vertex->z   = ((radius / xseg1) * i) - (radius / 2.f);
			vertex->tu1 = j * (1.0f / xseg1);
			vertex->tv1 = i * (1.0f / xseg1);	
			vertex->tu2 = j * (1.0f / xseg1);
			vertex->tv2 = i * (1.0f / xseg1);	
			*vertex++;
		}
	}

	Face * face = object->face;

	for (i = 0; i < xseg1; i++) {
		int line = i * xseg2;
		for (j = 0; j < xseg1; j++) {
			face->a = line + j;
			face->b = line + j + 1;
			face->c = line + j + xseg2;
			*face++;

			face->a = line + j + 1;
			face->b = line + j + xseg2 + 1;
			face->c = line + j + xseg2;
			*face++;
		}
	}

	return object;
}

#endif

#ifdef UNI_GENERATOR_TOROID

Object * genToroid(int numsegs, int numrings, float r0, float r1, char * texture) {
	return genToroid(numsegs, numrings, r0, r1, 1, 1, texture);
}

// code inopia
Object * genToroid(int numsegs, int numrings, float r0, float r1, float n1, float n2, char * texture) {
	int vert = (numsegs+1) * (numrings+1);
	int face = numsegs * numrings * 2;
	int i, j;
	float pi2 = 2 * 3.14159f;
	float m1  = pi2 / (numsegs+0);
	float m2  = pi2 / (numrings+0);
	float tm1 = 1.0f / numsegs;
	float tm2 = 1.0f / numrings;

	Object * object = new Object(vert, face, texture);

	for (i=0; i<numsegs+1; i++) {
		for (j=0; j<numrings+1; j++) {
			float th1 = i * m1;
			float th2 = j * m2;
			int num = (i * (numrings+1)) + j;

			float th1cos = (float) cos(th1);
			float th1sin = (float) sin(th1);
			float th2cos = (float) cos(th2);
			float th2sin = (float) sin(th2);

			float xflip = 1;
			float yflip = 1;
			float zflip = 1;
			float aflip = 1;

			if (th1cos<=0.f) { th1cos = -th1cos; xflip = -xflip; }
			if (th1sin<=0.f) { th1sin = -th1sin; yflip = -yflip; }
			if (th2cos<=0.f) { th2cos = -th2cos; aflip = -aflip; }
			if (th2sin<=0.f) { th2sin = -th2sin; zflip = -zflip; }

			float a = (float) pow(th2cos,n2); a = a * aflip;
			float x = (float) pow(th1cos,n1) * (r0 + (r1 * a));
			float y = (float) pow(th1sin,n1) * (r0 + (r1 * a));
			float z = r1 * (float) pow(th2sin,n2);

			x = x * xflip;
			y = y * yflip;
			z = z * zflip;

			float u = i * tm1;
			float v = j * tm2;

			if (j == numrings) {
				object->vertex[num].x = object->vertex[num - (numrings)].x;
				object->vertex[num].y = object->vertex[num - (numrings)].y;
				object->vertex[num].z = object->vertex[num - (numrings)].z;
			} else {
				object->vertex[num].x = x;
				object->vertex[num].y = y;
				object->vertex[num].z = z;
			}
			object->vertex[num].tu1 = u;
			object->vertex[num].tv1 = v;
			object->vertex[num].tu2 = u;
			object->vertex[num].tv2 = v;
		}
	}
	
	for (i=0; i<numsegs; i++) {
		for (j=0; j<numrings; j++) {
			int num = (i * numrings + j) * 2;
			object->face[num].a = ((i+1)) * (numrings+1) + ((j+1));
			object->face[num].b = i * (numrings+1) + j;
			object->face[num].c = i * (numrings+1) + ((j+1));

			num++;

			object->face[num].a = ((i+1)) * (numrings+1) + j;
			object->face[num].b = i * (numrings+1) + j;
			object->face[num].c = ((i+1)) * (numrings+1) + ((j+1));
		}
	}

	return object;
}

#endif