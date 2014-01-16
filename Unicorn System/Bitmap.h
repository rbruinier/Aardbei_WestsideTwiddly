/*

	Pyramid DX8 System - Bitmap Code (header file)


	Remarks:
		- Can be used to show 2d non texture bitmaps


	Updates:
		- 08-07-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/

#ifndef __BITMAP_H_
#define __BITMAP_H_

#ifdef UNI_BITMAP_SUPPORT

struct BitmapContainer {
	char				name[50];
	char				*filename;

	dword				width;
	dword				height;

	dword				texWidth;
	dword				texHeight;

	IDirect3DSurface8	*d3d8Surface;

	dword				usage;
	D3DPOOL				pool;
	D3DFORMAT			format;

	unsigned int		*data;
	
	BitmapContainer	*prev;
	BitmapContainer	*next;
};

//void uniTextureUnSwapAll();

void uniBitmapCreate(char * name, unsigned int * source, dword width, dword height, dword flags = 0);
void uniBitmapDestroyAll();

void uniBitmapDraw(char * name);
void uniBitmapDestroy(char * name);

#endif

#endif
