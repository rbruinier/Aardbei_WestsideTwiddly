/*

	Pyramid DX8 System - Texture Management Code


	Remarks:
		- Does not delete texture data allocated by demo (the data mem block)


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

struct TextureContainer 
{
	char				name[50];

	dword				width;
	dword				height;

	dword				texWidth;
	dword				texHeight;

    IDirect3DTexture8   *d3d8Texture;
	IDirect3DTexture8   *textureTemp;
	IDirect3DSurface8	*textureZBuffer;

	bool				mipmap;
	bool				toBig;
	bool				renderTarget;

	dword				usage;
	D3DPOOL				pool;
	D3DFORMAT			format;
	int					levels;

	unsigned int		*data;
	
	TextureContainer	*prev;
	TextureContainer	*next;
};

void uniTextureUnSwapAll();

void uniTextureCreate(char * naam, unsigned int * source, dword width, dword height, DWORD flags);
void uniTextureDestroyAll();

void uniTextureUploadData(void * target, dword * source, dword pixels, D3DFORMAT format);
void uniTextureUpdate(char * naam, unsigned int * source);

void uniTextureDestroy(char * name);

void uniTextureSetRenderTarget(char * naam);

#ifdef UNI_LOCKABLE_RENDERTARGET_TEXTURES

void uniTextureUnlock(char * naam);
dword * uniTextureLock(char * naam);

#endif

IDirect3DTexture8 * uniTextureGet(char * naam);
TextureContainer * uniTextureFind(char * naam);

void uniTextureReleaseDynamics();
void uniTextureRecreateDynamics();

#endif