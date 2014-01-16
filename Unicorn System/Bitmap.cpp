/*

	Pyramid DX8 System - Bitmap Code (source file)


	(c) 2001, Robert Jan Bruinier

*/

#include "Unicorn.h"

#ifdef UNI_BITMAP_SUPPORT

BitmapContainer * bitmapList = NULL;

#define FOREACHBITMAP(ptc) for (BitmapContainer * ptc = bitmapList; NULL != ptc; ptc = ptc->next)

void uniBitmapCreate(char * name, unsigned int * source, dword width, dword height, dword flags) {
#ifdef UNI_LOGHTML
	uniLog("   Uploading bitmap %s to memory (%ix%ix%i)\n", name, width, height, uniSystem->usedTextureBpp);
#endif

	BitmapContainer * actBitmap = new BitmapContainer();

	lstrcpy(actBitmap->name, name);

	if (bitmapList)
		bitmapList->prev = actBitmap;

	actBitmap->prev = NULL;
	actBitmap->next = bitmapList;
	bitmapList = actBitmap;

	actBitmap->width  = width;
	actBitmap->height = height;

	actBitmap->d3d8Surface    = NULL;

	if (D3D_OK != uniSystem->d3d8Device->CreateImageSurface(width, height, 
															uniSystem->d3d8PP.BackBufferFormat,
															&actBitmap->d3d8Surface))
		uniQuit("Failed to create image surface");

	D3DLOCKED_RECT d3dlr;

	if (D3D_OK != actBitmap->d3d8Surface->LockRect(&d3dlr, NULL, D3DLOCK_NOSYSLOCK))
		uniQuit("Failed to lock surface");

	uniTextureUploadData(d3dlr.pBits, source, width * height, uniSystem->d3d8PP.BackBufferFormat);

	if (D3D_OK != actBitmap->d3d8Surface->UnlockRect())
		uniQuit("Failed to unlock surface");
}

void uniBitmapDraw(char * name) {
	if (name == NULL)
		return;

	BitmapContainer * bitmap = NULL;

    FOREACHBITMAP (ptcBitmap) {
        if (lstrcmpi(name, ptcBitmap->name ) == 0) {
			bitmap = ptcBitmap;
			break;
		}
    }

	if (bitmap != NULL) {
		if (D3D_OK != uniSystem->d3d8Device->CopyRects(bitmap->d3d8Surface, NULL, 0,
			                                           uniSystem->surfaceBackBuffer,
													   NULL))
			uniQuit("Failed to render bitmap");
	}
}

void uniBitmapDestroy(char * name) {
	if (bitmapList) {
		FOREACHBITMAP (ptcBitmap) {
			if (lstrcmpi(name, ptcBitmap->name ) == 0) {

				if (ptcBitmap->prev)
					ptcBitmap->prev->next = ptcBitmap->next;
				else
					bitmapList = ptcBitmap->next;

				if (ptcBitmap->next)
					ptcBitmap->next->prev = ptcBitmap->prev;

				if (ptcBitmap->d3d8Surface != NULL)
					ptcBitmap->d3d8Surface->Release();

				delete ptcBitmap;

				return;
			}
		}
	}
}

void uniBitmapDestroyAll() {
	if (bitmapList != NULL) {
		FOREACHBITMAP (ptcBitmap) {
			if (ptcBitmap->d3d8Surface != NULL)
				ptcBitmap->d3d8Surface->Release();
		}
		do {
			BitmapContainer * next_ptr = bitmapList->next;
			delete bitmapList;
			bitmapList = next_ptr;
		} while (bitmapList != NULL);
	}
}


#endif
