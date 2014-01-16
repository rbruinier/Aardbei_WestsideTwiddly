/*

	Pyramid DX8 Engine - Matrix code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __MATRIX_H__
#define __MATRIX_H__
 
void uniMatrixIdentity(D3DMATRIX & mat);
void uniMatrixRotateX(D3DMATRIX & mat, FLOAT fRads);
void uniMatrixRotateY(D3DMATRIX & mat, FLOAT fRads);
void uniMatrixRotateZ(D3DMATRIX & mat, FLOAT fRads);
void uniMatrixRotate(D3DMATRIX & mat, FLOAT radX, FLOAT radY, FLOAT radZ);

void uniMatrixMultiply(D3DMATRIX & q, D3DMATRIX & a, D3DMATRIX & b);
void uniMatrixMultiplyOld(D3DMATRIX & q, D3DMATRIX & a, D3DMATRIX & b);
void uniMatrixMultiplyVector(D3DVECTOR & vDest, D3DVECTOR & vSrc, D3DMATRIX & mat);

void uniMatrixInvert(D3DMATRIX & dst, D3DMATRIX src);

#endif