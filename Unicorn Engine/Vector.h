/*

	Pyramid DX8 Engine - Vector code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#ifndef D3DVALUE
#define D3DVALUE float
#endif

typedef struct UniVector {
    union {
	    D3DVALUE x;
		D3DVALUE dvX;
    };
    union {
		D3DVALUE y;
		D3DVALUE dvY;
    };
    union {
		D3DVALUE z;
		D3DVALUE dvZ;
    };

    UniVector() { }
    UniVector(D3DVALUE f);
    UniVector(D3DVALUE _x, D3DVALUE _y, D3DVALUE _z);
    UniVector(const D3DVALUE f[3]);

    const D3DVALUE&operator[](int i) const;
    D3DVALUE&operator[](int i);

    UniVector& operator += (const UniVector& v);
    UniVector& operator -= (const UniVector& v);
    UniVector& operator *= (const UniVector& v);
    UniVector& operator /= (const UniVector& v);
    UniVector& operator *= (D3DVALUE s);
    UniVector& operator /= (D3DVALUE s);

    friend UniVector operator + (const UniVector& v);
    friend UniVector operator - (const UniVector& v);

    friend UniVector operator + (const UniVector& v1, const UniVector& v2);
    friend UniVector operator - (const UniVector& v1, const UniVector& v2);

	friend UniVector operator * (const UniVector& v, D3DVALUE s);
    friend UniVector operator * (D3DVALUE s, const UniVector& v);
    friend UniVector operator / (const UniVector& v, D3DVALUE s);

    friend UniVector operator * (const UniVector& v1, const UniVector& v2);
    friend UniVector operator / (const UniVector& v1, const UniVector& v2);

    friend int operator < (const UniVector& v1, const UniVector& v2);
    friend int operator <= (const UniVector& v1, const UniVector& v2);

    friend int operator == (const UniVector& v1, const UniVector& v2);

    friend D3DVALUE SquareMagnitude (const UniVector& v);
    friend D3DVALUE Magnitude (const UniVector& v);

    friend UniVector Normalize (const UniVector& v);

    friend D3DVALUE Min (const UniVector& v);
    friend D3DVALUE Max (const UniVector& v);

    friend UniVector Minimize (const UniVector& v1, const UniVector& v2);
    friend UniVector Maximize (const UniVector& v1, const UniVector& v2);

    friend D3DVALUE DotProduct (const UniVector& v1, const UniVector& v2);
    friend UniVector CrossProduct (const UniVector& v1, const UniVector& v2);

} UniVector;

#endif