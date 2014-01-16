/*

	Pyramid DX8 Engine - Camera code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "Engine.h"
 
Camera::Camera() {
	xLoc = yLoc = zLoc = 0;
	xTar = yTar = zTar = 0;

	zLoc = -5.f;
 
	roll = 0;

	fov       = 0.80f;
	aspect    = 0.75f;
	nearPlane = 0.5f;
	farPlane  = 1000.0f;
}

Camera::~Camera() {
}

Camera::update() {
	UniVector vAt		= UniVector(xTar, yTar, zTar);
	UniVector vFrom		= UniVector(xLoc, yLoc, zLoc);
	UniVector vWorldUp  = UniVector(   0,    1,    0);

    UniVector vView = vAt - vFrom;

    FLOAT fLength = Magnitude( vView );

    vView /= fLength;

    FLOAT fDotProduct = DotProduct( vWorldUp, vView );

    UniVector vUp = vWorldUp - fDotProduct * vView;

    if (1e-6f > (fLength = Magnitude(vUp))) {
        vUp = UniVector(0.0f, 1.0f, 0.0f) - vView.y * vView;
        if (1e-6f > (fLength = Magnitude(vUp))) {
            vUp = UniVector(0.0f, 0.0f, 1.0f) - vView.z * vView;
            fLength = Magnitude( vUp );
        }
    }

    vUp /= fLength;

    UniVector vRight = CrossProduct( vUp, vView );
    
	D3DMATRIX mat;
    uniMatrixIdentity(mat);
    mat._11 = vRight.x;    mat._12 = vUp.x;    mat._13 = vView.x;
    mat._21 = vRight.y;    mat._22 = vUp.y;    mat._23 = vView.y;
    mat._31 = vRight.z;    mat._32 = vUp.z;    mat._33 = vView.z;

    mat._41 = - DotProduct( vFrom, vRight );
    mat._42 = - DotProduct( vFrom, vUp );
    mat._43 = - DotProduct( vFrom, vView );

	D3DMATRIX matrixRoll;
	uniMatrixRotateZ(matrixRoll, roll * (_PI / 180.f));
	uniMatrixMultiply(mat, mat, matrixRoll);

    uniSetTransformView(mat);

    fabs(farPlane - nearPlane);

    fabs(sin(fov / 2.f));

	float w = aspect * (float) (cos(fov / 2.0f) / sin(fov / 2.0f));
	float h =   1.0f * (float) (cos(fov / 2.0f) / sin(fov / 2.0f));
    float Q = farPlane / (farPlane - nearPlane);

    ZeroMemory(&mat, sizeof(D3DMATRIX));
    mat._11 = w;
    mat._22 = h;
    mat._33 = Q;
    mat._34 = 1.0f;
    mat._43 = -Q * nearPlane;

    uniSetTransformProjection(mat);
}

Camera::setLoc(float x, float y, float z) {
	xLoc = x;
	yLoc = y;
	zLoc = z;
}

Camera::setTar(float x, float y, float z) {
	xTar = x;
	yTar = y;
	zTar = z;
}

Camera::setRoll(float r) {
	roll = r;
}
