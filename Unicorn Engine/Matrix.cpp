/*

	Pyramid DX8 Engine - Matrix code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "Engine.h"

void uniMatrixMultiply(D3DMATRIX & q, D3DMATRIX & a, D3DMATRIX & b) {
    float * pA = (float*) &a;
    float * pB = (float*) &b;
    float   pM[16];

    ZeroMemory(pM, sizeof(D3DMATRIX));

    for (int i = 0; i < 4; i++ ) 
        for (int j = 0; j < 4; j++) 
            for (int k = 0; k < 4; k++) 
                pM[(4 * i) + j] += pA[(4 * i) + k] * pB[(4 * k) + j];

    memcpy( &q, pM, sizeof(D3DMATRIX) );
}

void uniMatrixMultiplyOld(D3DMATRIX & q, D3DMATRIX & a, D3DMATRIX & b) {
    float * pA = (float*) &a;
    float * pB = (float*) &b;
    float   pM[16];

    ZeroMemory(pM, sizeof(D3DMATRIX));

    for (int i = 0; i < 4; i++ ) 
        for (int j = 0; j < 4; j++) 
            for (int k = 0; k < 4; k++) 
                pM[(4 * i) + j] += pA[(4 * k) + j] * pB[(4 * i) + k];

    memcpy( &q, pM, sizeof(D3DMATRIX) );
}

void uniMatrixMultiplyVector(D3DVECTOR & vDest, D3DVECTOR & vSrc, D3DMATRIX & mat) {
    float x = (vSrc.x * mat._11) + (vSrc.y * mat._21) + (vSrc.z * mat._31) + mat._41;
    float y = (vSrc.x * mat._12) + (vSrc.y * mat._22) + (vSrc.z * mat._32) + mat._42;
    float z = (vSrc.x * mat._13) + (vSrc.y * mat._23) + (vSrc.z * mat._33) + mat._43;

	vDest.x = x;
	vDest.y = y;
	vDest.z = z;
}

void uniMatrixIdentity(D3DMATRIX & mat) {
	mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = mat._21 = 0.0f;
	mat._23 = mat._24 = mat._31 = mat._32 = 0.0f;
	mat._34 = mat._41 = mat._42 = mat._43 = 0.0f;
}

void uniMatrixRotateX(D3DMATRIX & mat, FLOAT fRads) {
    uniMatrixIdentity(mat);
    mat._22 =  (float) cos(fRads);
    mat._23 =  (float) sin(fRads);
    mat._32 = -(float) sin(fRads);
    mat._33 =  (float) cos(fRads);
}

void uniMatrixRotateY(D3DMATRIX & mat, FLOAT fRads) {
    uniMatrixIdentity(mat);
    mat._11 =  (float) cos(fRads);
    mat._13 = -(float) sin(fRads);
    mat._31 =  (float) sin(fRads);
    mat._33 =  (float) cos(fRads);
}

void uniMatrixRotateZ(D3DMATRIX & mat, FLOAT fRads) {
    uniMatrixIdentity(mat);
    mat._11  =  (float) cos(fRads);
    mat._12  =  (float) sin(fRads);
    mat._21  = -(float) sin(fRads);
    mat._22  =  (float) cos(fRads);
}

void uniMatrixRotate(D3DMATRIX & mat, FLOAT radX, FLOAT radY, FLOAT radZ) {
	D3DMATRIX matSpinX;
	D3DMATRIX matSpinY;
	D3DMATRIX matSpinZ;

	uniMatrixRotateZ(matSpinZ, radZ);
	uniMatrixRotateY(matSpinY, radY);
	uniMatrixRotateX(matSpinX, radX);

	uniMatrixMultiply(mat, matSpinZ, matSpinY);
	uniMatrixMultiply(mat, mat, matSpinX);
}
/*
void uniMatrixInvert(D3DMATRIX & dst, D3DMATRIX src) {
	D3DMATRIX tmp;

    FLOAT fDetInv = 1.0f / (src._11 * (src._22 * src._33 - src._23 * src._32) -
                            src._12 * (src._21 * src._33 - src._23 * src._31) +
                            src._13 * (src._21 * src._32 - src._22 * src._31));

    tmp._11 =  fDetInv * (src._22 * src._33 - src._23 * src._32);
    tmp._12 = -fDetInv * (src._12 * src._33 - src._13 * src._32);
    tmp._13 =  fDetInv * (src._12 * src._23 - src._13 * src._22);
    tmp._14 = 0.0f;

    tmp._21 = -fDetInv * (src._21 * src._33 - src._23 * src._31);
    tmp._22 =  fDetInv * (src._11 * src._33 - src._13 * src._31);
    tmp._23 = -fDetInv * (src._11 * src._23 - src._13 * src._21);
    tmp._24 = 0.0f;

    tmp._31 =  fDetInv * (src._21 * src._32 - src._22 * src._31);
    tmp._32 = -fDetInv * (src._11 * src._32 - src._12 * src._31);
    tmp._33 =  fDetInv * (src._11 * src._22 - src._12 * src._21);
    tmp._34 = 0.0f;

    tmp._41 = -(src._41 * dst._11 + src._42 * dst._21 + src._43 * dst._31);
    tmp._42 = -(src._41 * dst._12 + src._42 * dst._22 + src._43 * dst._32);
    tmp._43 = -(src._41 * dst._13 + src._42 * dst._23 + src._43 * dst._33);
    tmp._44 = 1.0f;

    memcpy(&dst, &tmp, sizeof(D3DMATRIX));
}
*/