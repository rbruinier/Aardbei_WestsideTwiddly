/*

	Pyramid DX8 Engine - Vector code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "Engine.h"

UniVector::UniVector(D3DVALUE f)
{
    x = y = z = f;
}

UniVector::UniVector(D3DVALUE _x, D3DVALUE _y, D3DVALUE _z)
{
    x = _x; y = _y; z = _z;
}

UniVector::UniVector(const D3DVALUE f[3])
{
    x = f[0]; y = f[1]; z = f[2];
}

// =====================================
// Access grants
// =====================================


const D3DVALUE& UniVector::operator[](int i) const
{
    return (&x)[i];
}


D3DVALUE& UniVector::operator[](int i)
{
    return (&x)[i];
}


// =====================================
// Assignment operators
// =====================================

 
UniVector& UniVector::operator += (const UniVector& v)
{
   x += v.x;   y += v.y;   z += v.z;
   return *this;
}


 
UniVector& UniVector::operator -= (const UniVector& v)
{
   x -= v.x;   y -= v.y;   z -= v.z;
   return *this;
}


 
UniVector& UniVector::operator *= (const UniVector& v)
{
   x *= v.x;   y *= v.y;   z *= v.z;
   return *this;
}


 
UniVector& UniVector::operator /= (const UniVector& v)
{
   x /= v.x;   y /= v.y;   z /= v.z;
   return *this;
}


 
UniVector& UniVector::operator *= (D3DVALUE s)
{
   x *= s;   y *= s;   z *= s;
   return *this;
}


UniVector& UniVector::operator /= (D3DVALUE s)
{
   x /= s;   y /= s;   z /= s;
   return *this;
}


 
UniVector operator + (const UniVector& v)
{
   return v;
}


 
UniVector operator - (const UniVector& v) {
   return UniVector(-v.x, -v.y, -v.z);
}


 
UniVector operator + (const UniVector& v1, const UniVector& v2)
{
   return UniVector(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

 
UniVector operator - (const UniVector& v1, const UniVector& v2)
{
   return UniVector(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}


 
UniVector operator * (const UniVector& v1, const UniVector& v2)
{
   return UniVector(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}


 
UniVector operator / (const UniVector& v1, const UniVector& v2)
{
   return UniVector(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}


 
int operator < (const UniVector& v1, const UniVector& v2)
{
   return v1[0] < v2[0] && v1[1] < v2[1] && v1[2] < v2[2];
}


 
int operator <= (const UniVector& v1, const UniVector& v2)
{
   return v1[0] <= v2[0] && v1[1] <= v2[1] && v1[2] <= v2[2];
}


 
UniVector operator * (const UniVector& v, D3DVALUE s)
{
   return UniVector(s*v.x, s*v.y, s*v.z);
}


UniVector operator * (D3DVALUE s, const UniVector& v)
{
   return UniVector(s*v.x, s*v.y, s*v.z);
}



UniVector operator / (const UniVector& v, D3DVALUE s)
{
   return UniVector(v.x/s, v.y/s, v.z/s);
}


 
int operator == (const UniVector& v1, const UniVector& v2)
{
   return v1.x==v2.x && v1.y==v2.y && v1.z == v2.z;
}



D3DVALUE Magnitude (const UniVector& v)
{
   return (D3DVALUE) sqrt(SquareMagnitude(v));
}



D3DVALUE SquareMagnitude (const UniVector& v)
{
   return v.x*v.x + v.y*v.y + v.z*v.z;
}


 
UniVector Normalize (const UniVector& v)
{
   return v / Magnitude(v);
}



D3DVALUE Min (const UniVector& v)
{
   D3DVALUE ret = v.x;
   if (v.y < ret) ret = v.y;
   if (v.z < ret) ret = v.z;
   return ret;
}



D3DVALUE Max (const UniVector& v)
{
   D3DVALUE ret = v.x;
   if (ret < v.y) ret = v.y;
   if (ret < v.z) ret = v.z;
   return ret;
}



UniVector Minimize (const UniVector& v1, const UniVector& v2)
{
   return UniVector( v1[0] < v2[0] ? v1[0] : v2[0],
                   v1[1] < v2[1] ? v1[1] : v2[1],
                   v1[2] < v2[2] ? v1[2] : v2[2]);
}


 UniVector Maximize (const UniVector& v1, const UniVector& v2)
{
   return UniVector( v1[0] > v2[0] ? v1[0] : v2[0],
                   v1[1] > v2[1] ? v1[1] : v2[1],
                   v1[2] > v2[2] ? v1[2] : v2[2]);
}


D3DVALUE DotProduct (const UniVector& v1, const UniVector& v2)
{
   return v1.x*v2.x + v1.y * v2.y + v1.z*v2.z;
}

UniVector CrossProduct (const UniVector& v1, const UniVector& v2)
{
	UniVector result;

	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];

	return result;
}
