/*

	Pyramid DX8 System - Texture Management Code


	(c) 2001, Robert Jan Bruinier

*/

#include "Unicorn.h"

TextureContainer * textureList = NULL;

#define FOREACHTEXTURE(ptc) for( TextureContainer* ptc = textureList; NULL != ptc; ptc = ptc->next)

TextureContainer * uniTextureFind(char * naam) {
    FOREACHTEXTURE (ptcTexture) {
        if (lstrcmpi(naam, ptcTexture->name ) == 0)
            return ptcTexture;
    }
    return NULL;
}

IDirect3DTexture8 * uniTextureGet(char * naam) {
    FOREACHTEXTURE (ptcTexture) {
        if (lstrcmpi(naam, ptcTexture->name ) == 0)
            return ptcTexture->d3d8Texture;
    }
    return NULL;
}

void uniTextureCreate(char * naam, unsigned int * source, dword width, dword height, DWORD flags) {
#ifdef UNI_LOGHTML
	uniLog("   Uploading texture %s to memory (%ix%ix%i)\n", naam, width, height, uniSystem->usedTextureBpp);
#endif

	TextureContainer * actTexture = new TextureContainer();

	lstrcpy(actTexture->name, naam);

	if (textureList)
		textureList->prev = actTexture;

	actTexture->prev = NULL;
	actTexture->next = textureList;
	textureList = actTexture;

	actTexture->width  = width;
	actTexture->height = height;

	actTexture->toBig        = false;
	actTexture->renderTarget = false;

	actTexture->d3d8Texture    = NULL;
	actTexture->textureTemp    = NULL;
	actTexture->textureZBuffer = NULL;

	if (width > uniSystem->maxTextureWidth) {
		actTexture->texWidth = uniSystem->maxTextureWidth;
		actTexture->toBig = true;
	} else
		actTexture->texWidth = width;

	if (height > uniSystem->maxTextureHeight) {
		actTexture->texHeight = uniSystem->maxTextureWidth;
		actTexture->toBig = true;
	} else
		actTexture->texHeight = height;

	int levels = 1;

	if (flags & UNI_MIPMAP)
		levels = 0;
	if (flags & UNI_MIPMAP2)
		levels = 2;
	if (flags & UNI_MIPMAP3)
		levels = 3;
	if (flags & UNI_MIPMAP4)
		levels = 4;
	if (flags & UNI_MIPMAP5)
		levels = 5;
	if (flags & UNI_MIPMAP6)
		levels = 6;
	if (flags & UNI_MIPMAP7)
		levels = 7;
	if (flags & UNI_MIPMAP8)
		levels = 8;

	if (levels != 1)
		actTexture->mipmap = true;

	D3DFORMAT format = uniSystem->textureFormat;
	D3DPOOL   pool   = D3DPOOL_MANAGED;

	DWORD usage = 0;
	if (flags & UNI_RENDERTARGET) {
		levels = 1;
		format = uniSystem->d3d8PP.BackBufferFormat;
#ifdef UNI_LOCKABLE_RENDERTARGET_TEXTURES
		if (D3D_OK != uniSystem->d3d8Device->CreateTexture(actTexture->texWidth, actTexture->texHeight, levels, usage, 
														 format, D3DPOOL_SYSTEMMEM, &actTexture->textureTemp))
			uniQuit("Failed to create secundary texture");
#endif
		usage = D3DUSAGE_RENDERTARGET;
		actTexture->renderTarget = true;
		actTexture->mipmap = false;
		pool   = D3DPOOL_DEFAULT;
		if (D3D_OK != uniSystem->d3d8Device->CreateDepthStencilSurface(actTexture->texWidth, actTexture->texHeight,
												uniSystem->d3d8PP.AutoDepthStencilFormat,
												D3DMULTISAMPLE_NONE,
												&actTexture->textureZBuffer))
			uniQuit("Failed to create texture zbuffer");
	}

	actTexture->pool   = pool;
	actTexture->usage  = usage;
	actTexture->format = format;
	actTexture->levels = levels;

	if (D3D_OK != uniSystem->d3d8Device->CreateTexture(actTexture->texWidth, actTexture->texHeight, levels, usage, 
													 format, pool, &actTexture->d3d8Texture))
		uniQuit("UNI Says: failed to create texture");

	if (!(flags & UNI_RENDERTARGET) && source != NULL)
		uniTextureUpdate(naam, source);
	else
		actTexture->data = NULL;
}

void uniTextureUploadData(void * target, dword * source, dword pixels, D3DFORMAT format) {
	if ((format == D3DFMT_A1R5G5B5) || (format == D3DFMT_X1R5G5B5)) {
		unsigned short* pDst32 = (unsigned short*) target;
		__asm {
				mov		ecx, [pixels]
				mov		esi, [source]
				mov		edi, [pDst32]

				align 8

			loop16bit:
				lodsd

				mov		ebx, eax
				and		ebx, 10000000000000000000000000000000b
				shr		ebx, 24 + 7 - 15
				mov		edx, ebx

				mov		ebx, eax
				and		ebx, 00000000111110000000000000000000b
				shr		ebx, 16 + 3 - 10
				add		edx, ebx

				mov		ebx, eax
				and		ebx, 00000000000000001111100000000000b
				shr		ebx, 8 + 3 - 5
				add		edx, ebx

				and		eax, 00000000000000000000000011111000b
				shr		eax, 3
				add		eax, edx

				stosw

				dec		ecx
				jnz		loop16bit
		}
		return;
	}

	if ((format == D3DFMT_A8R8G8B8) || (format == D3DFMT_X8R8G8B8)) {
		dword* pDst32 = (dword*) target;
		__asm {
				mov		ecx, [pixels]

				mov		esi, [source]
				mov		edi, [pDst32]

			align 8

				rep		movsd
		}
		return;
	}

	if ((format == D3DFMT_A4R4G4B4)) {
		unsigned short* pDst32 = (unsigned short*) target;
		__asm {
				mov		ecx, [pixels]
				mov		esi, [source]
				mov		edi, [pDst32]

				align 8

			loopa4r4g4b4bit:
				lodsd

				mov		ebx, eax
				and		ebx, 11110000000000000000000000000000b
				shr		ebx, 24 + 4 - 12
				mov		edx, ebx

				mov		ebx, eax
				and		ebx, 00000000111100000000000000000000b
				shr		ebx, 16 + 4 - 8
				add		edx, ebx

				mov		ebx, eax
				and		ebx, 00000000000000001111000000000000b
				shr		ebx, 8 + 4 - 4
				add		edx, ebx

				and		eax, 00000000000000000000000011110000b
				shr		eax, 4
				add		eax, edx

				stosw

				dec		ecx
				jnz		loopa4r4g4b4bit
		}
		return;
	}

	if ((format == D3DFMT_A8R3G3B2)) {
		unsigned short* pDst32 = (unsigned short*) target;
		__asm {
				mov		ecx, [pixels]
				mov		esi, [source]
				mov		edi, [pDst32]

				align 8

			loopa8r3g3b2bit:
				lodsd

				mov		ebx, eax
				and		ebx, 11111111000000000000000000000000b
				shr		ebx, 24 + 0 - 8
				mov		edx, ebx

				mov		ebx, eax
				and		ebx, 00000000111000000000000000000000b
				shr		ebx, 16 + 5 - 5
				add		edx, ebx

				mov		ebx, eax
				and		ebx, 00000000000000001110000000000000b
				shr		ebx, 8 + 5 - 2
				add		edx, ebx

				and		eax, 00000000000000000000000011000000b
				shr		eax, 6
				add		eax, edx

				stosw

				dec		ecx
				jnz		loopa8r3g3b2bit
		}
		return;
	}

	if ((format == D3DFMT_R5G6B5)) {
		unsigned short* pDst32 = (unsigned short*) target;
		__asm {
				mov		ecx, [pixels]
				mov		esi, [source]
				mov		edi, [pDst32]

				align 8

			loopr5g6b5bit:
				lodsd

				mov		ebx, eax
				and		ebx, 00000000111110000000000000000000b
				shr		ebx, 16 + 3 - 11
				mov		edx, ebx

				mov		ebx, eax
				and		ebx, 00000000000000001111110000000000b
				shr		ebx, 8 + 2 - 5
				add		edx, ebx

				and		eax, 00000000000000000000000011111000b
				shr		eax, 3
				add		eax, edx

				stosw

				dec		ecx
				jnz		loopr5g6b5bit
		}
		return;
	}
}

void uniCalcMipMap(dword * target, int resXNew, int resYNew, dword * source, int resXOrg, int resYOrg) {
	int divX = resXOrg / resXNew;
	int divY = resYOrg / resYNew;
	dword clipper = 0xfcfcfcfc;
	for (int y = 0; y < resYNew; y++) {
		for (int x = 0; x < resXNew; x++) {
			target[(y * resXNew) + x] = 
				((source[((y * divY) * resXOrg) + (x * divX)] & clipper) >> 2) +
				((source[((y * divY) * resXOrg) + (x * divX) + 1] & clipper) >> 2) +
				((source[(((y * divY) + 1) * resXOrg) + (x * divX)] & clipper) >> 2) +
				((source[(((y * divY) + 1) * resXOrg) + (x * divX) + 1] & clipper) >> 2);
		}
	}
}

#ifdef UNI_LOCKABLE_RENDERTARGET_TEXTURES

dword * uniTextureLock(char * naam) {
	TextureContainer * actTexture = NULL;
    FOREACHTEXTURE (ptcTexture) {
        if (!lstrcmpi(naam, ptcTexture->name ) )
            actTexture = ptcTexture;
    }

	IDirect3DSurface8 * surfaceTemp;
	if (D3D_OK != actTexture->textureTemp->GetSurfaceLevel(0, &surfaceTemp))
		uniQuit("failed to get texture surface (for setting it to render target)");

	IDirect3DSurface8 * textureSurface;
	if (D3D_OK != actTexture->d3d8Texture->GetSurfaceLevel(0, &textureSurface))
		uniQuit("failed to get texture surface (for setting it to render target)");

	uniSystem->d3d8Device->CopyRects(textureSurface, NULL, 0, surfaceTemp, NULL);

	D3DLOCKED_RECT d3dlr;
	if (D3D_OK != surfaceTemp->LockRect(&d3dlr, NULL, D3DLOCK_NOSYSLOCK))
		uniQuit("failed to lock texture");

	textureSurface->Release();
	surfaceTemp->Release();

	return (dword*) d3dlr.pBits;
}

void uniTextureUnlock(char * naam) {
	TextureContainer * actTexture = NULL;
    FOREACHTEXTURE (ptcTexture) {
        if (!lstrcmpi(naam, ptcTexture->name ) )
            actTexture = ptcTexture;
    }

	IDirect3DSurface8 * surfaceTemp;
	if (D3D_OK != actTexture->textureTemp->GetSurfaceLevel(0, &surfaceTemp))
		uniQuit("failed to get texture surface (for setting it to render target)");

	IDirect3DSurface8 * textureSurface;
	if (D3D_OK != actTexture->d3d8Texture->GetSurfaceLevel(0, &textureSurface))
		uniQuit("failed to get texture surface (for setting it to render target)");

	if (D3D_OK != surfaceTemp->UnlockRect())
		uniQuit("failed to unlock texture");

	if (D3D_OK != uniSystem->d3d8Device->UpdateTexture(actTexture->textureTemp, actTexture->d3d8Texture))
		uniQuit("failed to update texture");

	textureSurface->Release();
	surfaceTemp->Release();
}

#endif

void uniTextureUpdate(char * naam, unsigned int * source) {
	TextureContainer * actTexture = NULL;
    FOREACHTEXTURE (ptcTexture) {
        if (!lstrcmpi(naam, ptcTexture->name ) )
            actTexture = ptcTexture;
    }

	int width  = actTexture->width;
	int height = actTexture->height;

	int texWidth  = actTexture->texWidth;
	int texHeight = actTexture->texHeight;

	if (actTexture->toBig) {
		uniCalcMipMap(source, texWidth, texHeight, source, width, height);
	}

	actTexture->data = source;

	D3DLOCKED_RECT d3dlr;

	if (!actTexture->mipmap) {
		if (D3D_OK != actTexture->d3d8Texture->LockRect(0, &d3dlr, NULL, D3DLOCK_NOSYSLOCK))
			uniQuit("Failed to lock texture");

		uniTextureUploadData(d3dlr.pBits, source, texWidth * texHeight, uniSystem->textureFormat);

		if (D3D_OK != actTexture->d3d8Texture->UnlockRect(0))
			uniQuit("Failed to unlock texture");
	} else {
		dword * mipmap = new dword[texWidth * texHeight];
		memcpy(mipmap, source, texWidth * texHeight * 4);
		int newResX = texWidth;
		int newResY = texHeight;
		for (dword iLevel = 0; iLevel < actTexture->d3d8Texture->GetLevelCount(); iLevel++) {
			if (D3D_OK != actTexture->d3d8Texture->LockRect(iLevel, &d3dlr, NULL, D3DLOCK_NOSYSLOCK))
				uniQuit("Failed to lock texture");

			uniTextureUploadData(d3dlr.pBits, mipmap, newResX * newResY, uniSystem->textureFormat);

			if (D3D_OK != actTexture->d3d8Texture->UnlockRect(iLevel))
				uniQuit("Failed to unlock texture");

			newResX >>= 1;
			newResY >>= 1;

			if ((newResX != 0) && (newResY != 0))
				uniCalcMipMap(mipmap, newResX, newResY, source, texWidth, texHeight);
		}
		delete [] mipmap;
	}
}


void uniTextureSetRenderTarget(char * naam) {
	TextureContainer * actTexture = NULL;
    FOREACHTEXTURE (ptcTexture) {
        if (!lstrcmpi(naam, ptcTexture->name ) )
            actTexture = ptcTexture;
    }

	if (actTexture == NULL)
	{
		return;
	}

	if (actTexture->renderTarget) {
		
		IDirect3DSurface8 * textureSurface;
		if (D3D_OK != actTexture->d3d8Texture->GetSurfaceLevel(0, &textureSurface))
			uniQuit("Failed to get texture surface (for setting it to render target)");

		if (D3D_OK != uniSystem->d3d8Device->SetRenderTarget(textureSurface, actTexture->textureZBuffer))
			uniQuit("Failed to set new render target");

		textureSurface->Release();
	}
}

void uniTextureUnSwapAll() {
#ifdef UNI_LOGHTML
	uniLog("<br><b>\nShaking textures</b><br>\n");
#endif

	if (textureList) {
		FOREACHTEXTURE (ptcTexture) {
			uniSetTexture(ptcTexture->name);

			SpriteDrawer sd;

			float x1 = uniSystem->viewportX1;
			float y1 = uniSystem->viewportY1;
			float x2 = uniSystem->viewportX2;
			float y2 = uniSystem->viewportY2;

			sd.Start(0);
			sd.Draw2D(x1, y1, 0, 0, 0xffffff,
					  x2, y1, 1, 0, 0xffffff,
					  x1, y2, 0, 1, 0xffffff,
				      x2, y2, 1, 1, 0xffffff);
			sd.Stop();

			uniClearBuffers(UNI_BBF, 0);
		}
	}
}

void uniTextureDestroy(char * name) {
	if (textureList) {
		FOREACHTEXTURE (ptcTexture) {
			if (lstrcmpi(name, ptcTexture->name ) == 0) {

				if (ptcTexture->prev)
					ptcTexture->prev->next = ptcTexture->next;
				else
					textureList = ptcTexture->next;

				if (ptcTexture->next)
					ptcTexture->next->prev = ptcTexture->prev;

				if (ptcTexture->d3d8Texture)
					ptcTexture->d3d8Texture->Release();
				if (ptcTexture->textureTemp != NULL)
					ptcTexture->textureTemp->Release();
				if (ptcTexture->textureZBuffer != NULL)
					ptcTexture->textureZBuffer->Release();
//				if (ptcTexture->data)
//					delete ptcTexture->data;

				delete ptcTexture;

				return;
			}
		}
	}
}

void uniTextureDestroyAll() {
	if (textureList != NULL) {
		FOREACHTEXTURE (ptcTexture) {
			if (ptcTexture->d3d8Texture != NULL)
				ptcTexture->d3d8Texture->Release();
			if (ptcTexture->textureTemp != NULL)
				ptcTexture->textureTemp->Release();
			if (ptcTexture->textureZBuffer != NULL)
				ptcTexture->textureZBuffer->Release();
//			if (ptcTexture->data != NULL)
//				delete ptcTexture->data;
		}
		do {
			TextureContainer * next_ptr = textureList->next;
			delete textureList;
			textureList = next_ptr;
		} while (textureList != NULL);
	}

}

void uniTextureReleaseDynamics() {
	if (textureList != NULL) {
		FOREACHTEXTURE (ptcTexture) 
		{
			if (ptcTexture->pool == D3DPOOL_DEFAULT)
				ptcTexture->d3d8Texture->Release();
			if (ptcTexture->textureZBuffer != NULL)
				ptcTexture->textureZBuffer->Release();
		}
	}
}

void uniTextureRecreateDynamics() {
	if (textureList != NULL) {
		FOREACHTEXTURE (ptcTexture) {
			if (ptcTexture->pool == D3DPOOL_DEFAULT)
			{
				if (D3D_OK != uniSystem->d3d8Device->CreateTexture(ptcTexture->texWidth, ptcTexture->texHeight, ptcTexture->levels, ptcTexture->usage, 
																 ptcTexture->format, ptcTexture->pool, &ptcTexture->d3d8Texture))
					uniQuit("Failed to recreate texture");
				if (D3D_OK != uniSystem->d3d8Device->CreateDepthStencilSurface(ptcTexture->texWidth, ptcTexture->texHeight,
																				uniSystem->d3d8PP.AutoDepthStencilFormat,
																				D3DMULTISAMPLE_NONE,
																				&ptcTexture->textureZBuffer))
					uniQuit("Failed to recreate texture zbuffer");
			}
		}
	}	
}
