/*

	Pyramid DX8 Engine - Object code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "Engine.h"
 
Object::Object(int nrVertices, int nrFaces, char *texture) {
	this->nrVertices = nrVertices;
	this->nrFaces    = nrFaces;

	if (nrVertices)
		vertex	=  new Vertex[nrVertices];

	if (nrFaces)
		face  	=  new Face[nrFaces];

	dynamic		  = false;
	finished	  = false;
	updated		  = false;
	alphatest	  = false;
	specular	  = false;
	customCodeSet = false;

	writeZBuffer  = true;

	alphatestRef      = 128;
	alphatestFunction = D3DCMP_GREATER;

	transMode = UNI_NONE;

	indexUV1	 = 0;
	indexUV2     = 1;

	cull   = CULL_CW;
	shade  = SHADE_NONE;
	method = METHOD_SOLID;

	xDeg = yDeg = zDeg = 0;
	xLoc = yLoc = zLoc = 0;
	xScl = yScl = zScl = 1;

	texture1 = NULL;
	texture2 = NULL;

	setTexture1(texture);
	setTexture2(NULL);

	uniMatrixIdentity(objMatrix);

	setDiffuse(1, 1, 1, 1);
	setAmbient(1, 1, 1, 1);
	setEmissive(0, 0, 0, 0);
	setSpecular(0, 0, 0, 0, 0);

	vBuffer = NULL;
	iBuffer = NULL;
}

void Object::finish(bool dynamic) {
	if (!finished) {		
		this->dynamic = dynamic;

		if (shade != SHADE_NONE)
			this->dynamic = true;

		normals();

		dword flags = D3DUSAGE_WRITEONLY;

		if (this->dynamic)
			flags |= D3DUSAGE_DYNAMIC;

		vBuffer = new UniVBuffer((dword*) vertex, sizeof(Vertex), nrVertices * sizeof(Vertex), 
							   	 ENGINE_VERTEX, flags);

		iBuffer = new UniIBuffer((dword*) face, sizeof(Face), nrFaces * sizeof(Face), 
								 D3DFMT_INDEX16, D3DUSAGE_WRITEONLY);

	}
	finished = true;
}

void Object::update() {
	if (!updated && finished) {
		if (dynamic)
			normals();
		if ((shade & SHADE_ENVMAP) || (shade & SHADE_PHONG))
			envmap();
		vBuffer->fill((dword*)vertex);
		updated = true;
	}
}

Object::~Object() {
	if (nrVertices > 0) 
		delete vertex;
	if (nrFaces    > 0) 
		delete face;
	if (dynamic && (nrVertices > 0))
		delete normalCounter;
}

void Object::render() {
	if (method == METHOD_NONE)
		return;

    D3DMATRIX matSpin;

	uniMatrixRotate(matSpin, xDeg * _PI / 180.f,
  						     yDeg * _PI / 180.f,
							 zDeg * _PI / 180.f);

	D3DMATRIX matScale;

	uniMatrixIdentity(matScale);

	matScale._11 = xScl;
	matScale._22 = yScl;
	matScale._33 = zScl;

	uniMatrixMultiply(matSpin, matSpin, matScale);

	matSpin._41 = xLoc;
	matSpin._42 = yLoc;
	matSpin._43 = zLoc;

    uniSetTransformWorld(matSpin);

	if (!finished)
		finish(false);

	if ((shade != SHADE_NONE) || dynamic)
		update();

	uniSetMaterial(material);

	if (texture1 != NULL) {
		uniSetTexture(texture1, 0);
		uniSetTextureStageState(0,	D3DTSS_COLOROP,			D3DTOP_MODULATE); 
		uniSetTextureStageState(0,	D3DTSS_TEXCOORDINDEX,	indexUV1); 
	} else {
		uniSetTextureStageState(0,	D3DTSS_COLOROP, D3DTOP_DISABLE);
	}

	if ((texture2 == NULL) && (shade & SHADE_PHONG))
		setTexture2("phongmap");

	if ((texture2 != NULL)) {
		uniSetTexture(texture2, 1);
		uniSetTextureStageState(1,	D3DTSS_COLOROP,			D3DTOP_ADD); 
		uniSetTextureStageState(1,	D3DTSS_TEXCOORDINDEX,	indexUV2); 
	} else {
		uniSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	}
 
	if (transMode != UNI_NONE) {
		uniSetRenderState(D3DRS_ALPHABLENDENABLE, true);
		switch (transMode) {
			case UNI_ADD:
				uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
				uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				uniSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
			case UNI_ALPHA:
				uniSetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
				uniSetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				uniSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
			}
	} 
	else 
	{
		uniSetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	if (alphatest) {
		uniSetRenderState(D3DRS_ALPHATESTENABLE, true);
		uniSetRenderState(D3DRS_ALPHAFUNC, alphatestFunction);
		uniSetRenderState(D3DRS_ALPHAREF,  alphatestRef); 
	} 
	else
	{
		uniSetRenderState(D3DRS_ALPHATESTENABLE, false);
	}

	uniSetRenderState(D3DRS_CULLMODE, cull);

	uniSetRenderState(D3DRS_ZWRITEENABLE, writeZBuffer); 

	if (specular)
		uniSetRenderState(D3DRS_SPECULARENABLE,  true);
	else
		uniSetRenderState(D3DRS_SPECULARENABLE,  false);

	if (shade & SHADE_FLAT)
		uniSetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	else
		uniSetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	if (customCodeSet)
		customCode();

	vBuffer->set();
	iBuffer->set();

	if (method & METHOD_SOLID) {
		uniSetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		uniSystem->d3d8Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, nrVertices, 0, nrFaces);
	}
	if (method & METHOD_LINE) {
		uniSetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		uniSystem->d3d8Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, nrVertices, 0, nrFaces);
	}
	if (method & METHOD_POINT) {
		uniSetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);

		uniSystem->d3d8Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, nrVertices, 0, nrFaces);
	}

	if (texture2 != NULL)
		uniSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	if (alphatest) 
	{
		uniSetRenderState(D3DRS_ALPHATESTENABLE, false);
	}

	if (texture1 == NULL)
	{
		uniSetTexture(texture1, 0);
		uniSetTextureStageState(0,	D3DTSS_COLOROP,			D3DTOP_MODULATE); 
		uniSetTextureStageState(0,	D3DTSS_TEXCOORDINDEX,	indexUV1); 
	}

	updated = false;
}

void Object::setLoc(float x, float y, float z) {
	xLoc = x;
	yLoc = y;
	zLoc = z;
}

void Object::setRot(float x, float y, float z) {
	xDeg = x;
	yDeg = y;
	zDeg = z;
}

void Object::setScl(float x, float y, float z) {
	xScl = x;
	yScl = y;
	zScl = z;
}

void Object::setDiffuse(float r, float g, float b, float a) {
	material.Diffuse.r  = r;     
	material.Diffuse.g  = g; 
    material.Diffuse.b  = b;     
	material.Diffuse.a  = a;
	alphatestRef        = 128 - (int)((1.f - a) * 128.f);
}

void Object::setAmbient(float r, float g, float b, float a) {
    material.Ambient.r  = r;     
	material.Ambient.g  = g; 
    material.Ambient.b  = b;     
	material.Ambient.a  = a;  
}

void Object::setEmissive(float r, float g, float b, float a) {
    material.Emissive.r  = r;     
	material.Emissive.g  = g; 
    material.Emissive.b  = b;     
	material.Emissive.a  = a;  
}

void Object::setSpecular(float r, float g, float b, float a, float p) {
    material.Specular.r = r;     
	material.Specular.g = g; 
    material.Specular.b = b;     
	material.Specular.a = a; 
    material.Power = p;	

	if (p != 0.0f)
		specular = true;
}

void Object::setTexture1(char *naam) 
{
	if (texture1 != NULL) 
		delete [] texture1;
	if (naam != NULL) {
		texture1 = new char[lstrlen(naam) + 1];
		lstrcpy(texture1, naam);
	} else {
		texture1 = NULL;
	}
}

void Object::setTexture2(char *naam) 
{
	if (texture2 != NULL) 
		delete texture2;
	if (naam != NULL) {
		texture2 = new char[lstrlen(naam) + 1];
		lstrcpy(texture2, naam);
	} else {
		texture2 = NULL;
	}
}

char *Object::getTexture1() 
{
	return texture1;
}

char *Object::getTexture2() 
{
	return texture2;
}

void Object::setCustomCode(UNI_CUSTOMIZE function) {
	customCode = function;
	customCodeSet = true;

}

void Object::scaleUV(float scaleu, float scalev) {
	for (int i = 0; i < nrVertices; i++) {
		vertex[i].tu1 *= scaleu;
		vertex[i].tv1 *= scalev;
		vertex[i].tu2 *= scaleu;
		vertex[i].tv2 *= scalev;
	}
}

void Object::envmap() {
    D3DMATRIX matWorld, matView, matWV;

	uniSystem->d3d8Device->GetTransform(D3DTS_VIEW,  &matView);
    uniSystem->d3d8Device->GetTransform(D3DTS_WORLD, &matWorld);

    uniMatrixMultiply(matWV, matWorld, matView);

    FLOAT m11 = matWV._11,   m21 = matWV._21,   m31 = matWV._31;
    FLOAT m12 = matWV._12,   m22 = matWV._22,   m32 = matWV._32;

	if (shade & SHADE_ENVMAP) {
		for (int i = 0; i < nrVertices; i++) {
			FLOAT nx = vertex[i].nx;
			FLOAT ny = vertex[i].ny;
			FLOAT nz = vertex[i].nz;
			vertex[i].tu1 = 0.5f * (1.0f + (nx * m11 + ny * m21 + nz * m31));
			vertex[i].tv1 = 0.5f * (1.0f - (nx * m12 + ny * m22 + nz * m32));
		}
	}
	if (shade & SHADE_PHONG) {
		for (int i = 0; i < nrVertices; i++) {
			FLOAT nx = vertex[i].nx;
			FLOAT ny = vertex[i].ny;
			FLOAT nz = vertex[i].nz;
			vertex[i].tu2 = 0.5f * (1.0f + (nx * m11 + ny * m21 + nz * m31));
			vertex[i].tv2 = 0.5f * (1.0f - (nx * m12 + ny * m22 + nz * m32));
		}
	}

/*  // dx8 envmappen: nog erg debug dus niet gebruiken!
	D3DMATRIX mat;
    mat._11 = 0.5f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f; 
    mat._21 = 0.0f; mat._22 =-0.5f; mat._23 = 0.0f; mat._24 = 0.0f; 
    mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 1.0f; mat._34 = 0.0f; 
    mat._41 = 0.5f; mat._42 = 0.5f; mat._43 = 0.0f; mat._44 = 1.0f; 
    unicorn->d3d8Device->SetTransform( D3DTS_TEXTURE0, &mat);
    unicorn->d3d8Device->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
    unicorn->d3d8Device->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
*/

}

void Object::normals() {
	int i;
	if (!finished)
		normalCounter = new int[nrVertices];
	for (i = 0; i < nrVertices; i++) {
		vertex[i].nx = 0; 
		vertex[i].ny = 0; 
		vertex[i].nz = 0; 
		normalCounter[i]   = 0;
	}
    for (i = 0; i < nrFaces; i++) {
        int a = face[i].a;
        int b = face[i].b;
        int c = face[i].c;
        float relx1 = vertex[b].x - vertex[a].x;
        float rely1 = vertex[b].y - vertex[a].y;
        float relz1 = vertex[b].z - vertex[a].z;
        float relx2 = vertex[c].x - vertex[a].x;
        float rely2 = vertex[c].y - vertex[a].y;
        float relz2 = vertex[c].z - vertex[a].z;
        float nx = (rely1 * relz2) - (relz1 * rely2);
        float ny = (relz1 * relx2) - (relx1 * relz2);
        float nz = (relx1 * rely2) - (rely1 * relx2);
		float ll = 1.f / (float) sqrt((nx * nx) + (ny * ny) + (nz * nz));
		nx *= ll;
		ny *= ll;
		nz *= ll;
		vertex[a].nx += nx;
		vertex[a].ny += ny;
		vertex[a].nz += nz;
		normalCounter[a]++;
		vertex[b].nx += nx;
		vertex[b].ny += ny;
		vertex[b].nz += nz;
		normalCounter[b]++;
		vertex[c].nx += nx;
		vertex[c].ny += ny;
		vertex[c].nz += nz;
		normalCounter[c]++;
    }
	for (i = 0; i < nrVertices; i++) {
		float blah = -1.f / normalCounter[i];
		vertex[i].nx *= blah; 
		vertex[i].ny *= blah; 
		vertex[i].nz *= blah; 
	}
	if (!dynamic)
		delete normalCounter;
}

void Object::optimize(float dif) {
	for (int i = 0; i < nrFaces; i++) {
		int a = face[i].a;
		int b = face[i].b;
		int c = face[i].c;
		Vertex v1 = vertex[a];
		Vertex v2 = vertex[b];
		Vertex v3 = vertex[c];
		for (int j = 0; j < nrVertices; j++) {
			Vertex v = vertex[j];
			if (a != j) {
				if ((v1.x >= (v.x - dif)) && (v1.x <= (v.x + dif)) &&
					(v1.y >= (v.y - dif)) && (v1.y <= (v.y + dif)) &&
					(v1.z >= (v.z - dif)) && (v1.z <= (v.z + dif)))
					a = j;
			}
			if (b != j) {
				if ((v2.x >= (v.x - dif)) && (v2.x <= (v.x + dif)) &&
					(v2.y >= (v.y - dif)) && (v2.y <= (v.y + dif)) &&
					(v2.z >= (v.z - dif)) && (v2.z <= (v.z + dif)))
					b = j;
			}
			if (c != j) {
				if ((v3.x >= (v.x - dif)) && (v3.x <= (v.x + dif)) &&
					(v3.y >= (v.y - dif)) && (v3.y <= (v.y + dif)) &&
					(v3.z >= (v.z - dif)) && (v3.z <= (v.z + dif)))
					c = j;
			}
		}
		face[i].a = a;
		face[i].b = b;
		face[i].c = c;
	}
}

