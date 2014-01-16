#include "Main.h"

long ScapeEffect::GenSemiRand(long _x, long &adder)
{
	long bla = _x;
	adder   += bla;
	bla     += adder + (adder << 2);
	return ((bla << 6) + adder);
}

void ScapeEffect::CreateShape(EXTRASHAPE_GEN ExtraFunction, Vertex *vertex, float fSoftness, 
							  float fDensity, long seed)
{
	int i, j, k, p, k2, p2;

	long next = 0;

	float * heightMap = new float[256 * 256];

	memset(heightMap, 0, 256 * 256 * 4);

	heightMap[0] = 128;

	for (p = 256; p > 1; p = p2) 
	{
		p2 = p >> 1;
		k  = p * 8 + 20;
		k2 = k >> 1;
		for (i = 0; i < 256; i += p) 
		{
			for (j = 0; j < 256; j += p) 
			{
				int a, b, c, d;
				a = heightMap[(i << 8) + j];
				b = heightMap[(((i + p) & 255) << 8) + j];
				c = heightMap[(i << 8) + ((j + p) & 255)];
				d = heightMap[(((i + p) & 255) << 8) + ((j + p) & 255)];
				heightMap[(i << 8) + ((j + p2) & 255)] =
					((a + c) >> 1) + (GenSemiRand(next++, seed) % k - k2);
				heightMap[(((i + p2) & 255) << 8) + ((j + p2) & 255)]=
					((a + b + c + d) >> 2) + (GenSemiRand(next++, seed) % k - k2);
				heightMap[(((i + p2) & 255) << 8) + j]=
					((a + b) >> 1) + (GenSemiRand(next++, seed) % k - k2);
			}
		}
	}

	if (ExtraFunction != NULL)
	{
		ExtraFunction(heightMap);
	}

	float lowest = 0, highest = 0;

	for (k = 0; k < 4; k++) 
	{
		for (i = 0; i < 256 * 256; i += 256) 
		{
			for (j = 0; j < 256; j++) 
			{
				
				float height = (heightMap[((i + 256) & 0xFF00) + j] + heightMap[i + ((j + 1) & 0xFF)] +
					 		    heightMap[((i - 256) & 0xFF00) + j] + heightMap[i + ((j - 1) & 0xFF)]) / 4.f;
				if (height < lowest)
					lowest = height;
				if (height > highest)
					highest = height;
				heightMap[i + j] = height;
			}
		}
	}


	// van 0 tot 255 normaliseren
	for (i = 0; i < 256 * 256; i += 256) 
	{
		for (j = 0; j < 256; j++) 
		{
			float height = heightMap[i + j];
			height = ((height - lowest) / (highest - lowest)) * 255.f;			
			heightMap[i + j] = height;
		}
	}


	float mapStepX = (256.f / (nrSegmentsX + 1)) * fDensity;
	float mapStepY = (256.f / (nrSegmentsZ + 1)) * fDensity;
	for (int y = 0; y < nrSegmentsZ + 1; y++)
	{
		for (int x = 0; x < nrSegmentsX + 1; x++)
		{
			int index = (y * (nrSegmentsX + 1)) + x;

			float vx = vertex[index].x;
			float vy = vertex[index].y;
			float vz = vertex[index].z;

			int indexX = ((int) (x * mapStepX)) & 255;
			int indexY = ((int) (y * mapStepY)) & 255;

			float h = heightMap[(indexY << 8) + indexX];

			vy += h / fSoftness;

			vertex[index].x = vx;
			vertex[index].y = vy;
			vertex[index].z = vz;
		}
	}

	delete heightMap;		
}

bool ScapeEffect::GetHeightAtPosition(Vertex *vertex, float x, float z, float &height)
{
	height = 0;

	if ((x < left) || (x > right) || (z < top) || (z > bottom))
	{
		return false;
	}

	float indexX = (((x - left) / (right - left)) * (float)(nrSegmentsX + 1));
	float indexY = (((z - top)  / (bottom - top)) * (float)(nrSegmentsZ + 1));

	float indexX1 = (float) floor(indexX);
	float indexY1 = (float) floor(indexY);

	float indexX2 = (float) ceil(indexX);
	float indexY2 = (float) ceil(indexY);

	indexX1 = fmod(indexX1, nrSegmentsX);
	indexX2 = fmod(indexX2, nrSegmentsX);

	indexY1 = fmod(indexY1, nrSegmentsZ);
	indexY2 = fmod(indexY2, nrSegmentsZ);

	int yy1 = (int)indexY1 * (nrSegmentsX + 1);
	int yy2 = (int)indexY2 * (nrSegmentsX + 1);
	
	int index1 = yy1 + (int)indexX1;
	int index2 = yy1 + (int)indexX2;
	int index3 = yy2 + (int)indexX1;
	int index4 = yy2 + (int)indexX2;

	float height1 = vertex[index1].y;
	float height2 = vertex[index2].y;
	float height3 = vertex[index3].y;
	float height4 = vertex[index4].y;

	float factX = 1.0f - (indexX - indexX1);
	float factY = 1.0f - (indexY - indexY1);

	height = (((height1 * factX) + (height2 * (1.0f - factX))) * factY) +
		     (((height3 * factX) + (height4 * (1.0f - factX))) * (1.0f - factY));

	return true;
}

dword *ScapeEffect::CreateTexture(int width, int height, dword *texture1, dword *texture2, dword *texture3,
				     dword *texture4, float startY, float scaleY)
{
	dword *textures[4];

	textures[0] = texture1;
	textures[1] = texture2; 
	textures[2] = texture3;
	textures[3] = texture4;

	int textureW = width;
	int textureH = height;

	dword *textureFinal = new dword[textureW * textureH];

	float xStep = (right - left) / (textureW);
	float zStep = (bottom - top) / (textureH);
	int offset = 0;
	int x, y;	

	for (y = 0; y < textureH; y++) {
		for (x = 0; x < textureW; x++) {			
			dword rr[4], gg[4], bb[4];
			int index = ((y & 255) << 8) + (x & 255);
			for (int co = 0; co < 4; co++) {
				rr[co] = (textures[co][index] >> 16) & 255;
				gg[co] = (textures[co][index] >>  8) & 255;
				bb[co] = (textures[co][index]) & 255;
			}
			float pr[4] = {0, 0, 0, 0};
			float height;

			if (!GetHeightAtPosition(scene->object[0]->vertex, left + (xStep * x), top + (zStep * y), height))
			{
				height = 0;
			}

			height = ((height + startY) / 120.f) * scaleY;

			if (height < 0.25f) {
				pr[0] = 0.25f - height;
				pr[1] = height;
				
			} else
			if (height < 0.50f) {
				pr[1] = 0.50f - height;
				pr[2] = height - 0.25f;
				
			} else
			if (height < 0.75f) {
				pr[2] = 0.75f - height;
				pr[3] = height - 0.50f;				
			} else {
				pr[3] = 1.00f - height;
				pr[4] = height - 0.75f;				
			}
 
			for (int i = 0; i < 4; i++)
				pr[i] /= 0.25f;
			
			dword finalRR = (rr[0] * pr[0]) + (rr[1] * pr[1]) + (rr[2] * pr[2]) + (rr[3] * pr[3]);
			dword finalGG = (gg[0] * pr[0]) + (gg[1] * pr[1]) + (gg[2] * pr[2]) + (gg[3] * pr[3]);
			dword finalBB = (bb[0] * pr[0]) + (bb[1] * pr[1]) + (bb[2] * pr[2]) + (bb[3] * pr[3]);

			textureFinal[offset++] = (finalRR << 16) | (finalGG << 8) | (finalBB);
		}
	}

	return textureFinal;
}