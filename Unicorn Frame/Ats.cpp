// V1.00 (09-10-2001)

#include "Main.h"

#include <math.h>


TextureGenerator::TextureGenerator(int iWidth, int iHeight)
{
    width    = iWidth;
    height   = iHeight;
    halfX    = width >> 1;
    halfY    = height >> 1;
    nrPixels = width * height;
    clipWidth  = width  - 1;
    clipHeight = height - 1;

    nrLayers = 0;

    layerTemp1 = new unsigned int[nrPixels];
    layerTemp2 = new unsigned int[nrPixels];

    memset(layerTemp1, 0, nrPixels * 4);
    memset(layerTemp2, 0, nrPixels * 4);

    for (int i = 0; i < 15; i++)
    {
        if ((int)(pow(2, i)) == width)
        {
            shiftX = i;
            break;
        }
    }

    srand(uniTimer());
}

TextureGenerator::~TextureGenerator()
{
    for (int i = 0; i < nrLayers; i++)
    {
        delete [] layers[i];
    }

    delete [] layerTemp1;
    delete [] layerTemp2;
}

void TextureGenerator::AddLayer()
{
    layerActive = new unsigned int[nrPixels];
    memset(layerActive, 0, nrPixels * 4);

    layers[nrLayers] = layerActive;

    nrLayers++;
}

unsigned int TextureGenerator::rand(unsigned int _x) {
	long adder = randTemp;
	long x = (_x << 13) ^ _x;
	return ((x + adder) * (x * x * 15731 + 789221) + 1376312589) / 12359;
}




/*

	LAYER FILTERS

*/

// cleart een (channel van) layer
void TextureGenerator::FilterClear(unsigned int *target, int channel)
{
	if (channel == CHANNEL_ALL)
    {
		memset(target, 0, nrPixels * 4);
	}
    else
    {
		int shift = CHANTOSHIFT(channel);
	    for (int i = 0; i < nrPixels; i++)
        {
		    target[i] &= (0xffffffff - (0xff << shift));
	    }
	}
}

// copieert een (channel van) layer in een andere layer
void TextureGenerator::FilterCopy(unsigned int *target, unsigned int *source, int channel)
{
    if (target == source)
    {
        return;
    }

	if (channel == CHANNEL_ALL)
    {
		memcpy(target, source, nrPixels * 4);
	}
    else
    {
		int shift = CHANTOSHIFT(channel);
	    for (int i = 0; i < nrPixels; i++)
        {
		    target[i] &= (0xffffffff - (0xff << shift));
			target[i] |= ((source[i] >> shift) & 0xff) << shift;
	    }
	}
}

// telt een (channel van) layer op bij andere (channel van) layer
void TextureGenerator::FilterAdd(unsigned int *target, unsigned int *source, int channel)
{
	if (channel == CHANNEL_ALL)
    {
        int aantal = nrPixels;
        __asm
        {
            mov       ecx, [aantal]
            mov       edi, [target]
            mov       esi, [source]
        FilterAddLoop:
            pxor      mm0, mm0
            movd      mm1, [esi]
            movd      mm2, [edi]
		    punpcklbw mm1, mm0
		    punpcklbw mm2, mm0
		    paddw     mm1, mm2
		    packuswb  mm1, mm0
		    movd      [edi], mm1
            add       edi, 4
            add       esi, 4
            dec       ecx
            jnz       FilterAddLoop
            emms
        }
	}
    else
    {
	    for (int i = 0; i < nrPixels; i++)
        {
			int shift = CHANTOSHIFT(channel);
			int t = (target[i] >> shift) & 0xff;
			int d = (source[i] >> shift) & 0xff;
			t += d;
			if (t > 0xff) t = 0xff;
		    target[i] &= (0xffffffff - (0xff << shift));
			target[i] |= t << shift;
	    }
	}
}

// trekt een (channel van) layer af van andere (channel van) layer
void TextureGenerator::FilterSub(unsigned int *target, unsigned int *source, int channel)
{
	if (channel == CHANNEL_ALL)
    {
        int aantal = nrPixels;
        __asm
        {
            mov       ecx, [aantal]
            mov       edi, [target]
            mov       esi, [source]
        FilterSubLoop:
            pxor      mm0, mm0
            movd      mm1, [esi]
            movd      mm2, [edi]
		    punpcklbw mm1, mm0
		    punpcklbw mm2, mm0
		    psubw     mm2, mm1
		    packuswb  mm2, mm0
		    movd      [edi], mm2
            add       edi, 4
            add       esi, 4
            dec       ecx
            jnz       FilterSubLoop
            emms
        }
	}
    else
    {
	    for (int i = 0; i < nrPixels; i++) {
			int shift = CHANTOSHIFT(channel);
			int t = (target[i] >> shift) & 0xff;
			int d = (source[i] >> shift) & 0xff;
			t -= d;
			if (t < 0x0) t = 0x0;
		    target[i] &= (0xffffffff - (0xff << shift));
			target[i] |= t << shift;
	    }
	}
}

// vermedigvuldigt een (channel van) layer met andere (channel van) layer
void TextureGenerator::FilterMul(unsigned int *target, unsigned int *source, int channel)
{
	if (channel == CHANNEL_ALL)
    {
        int aantal = nrPixels;
        __asm
        {
            mov       ecx, [aantal]
            mov       edi, [target]
            mov       esi, [source]
        FilterMulLoop:
            pxor      mm0, mm0
            movd      mm1, [esi]
            movd      mm2, [edi]
		    punpcklbw mm1, mm0
		    punpcklbw mm2, mm0
		    pmullw    mm1, mm2
		    psrlw     mm1, 8
		    packuswb  mm1, mm0
		    movd      [edi], mm1
            add       edi, 4
            add       esi, 4
            dec       ecx
            jnz       FilterMulLoop
            emms
        }
	}
    else
    {
	    for (int i = 0; i < nrPixels; i++) {
			int shift = CHANTOSHIFT(channel);
			int t = (target[i] >> shift) & 0xff;
			int d = (source[i] >> shift) & 0xff;
			t = (t * d) >> 8;
			if (t < 0x0) t = 0x0;
		    target[i] &= (0xffffffff - (0xff << shift));
			target[i] |= t << shift;
	    }
	}
}

// TODO: per channel filter
// copieert de pixel alleen als het niet nul is
void TextureGenerator::FilterNotNullCopy(unsigned int *target, unsigned int *source, int channel)
{
	if (channel == CHANNEL_ALL)
    {
	    for (int i = 0; i < nrPixels; i++)
        {
//			int ta = A(target[i]);
			int tr = R(target[i]);
			int tg = G(target[i]);
			int tb = B(target[i]);
//			int sa = A(source[i]);
			int sr = R(source[i]);
			int sg = G(source[i]);
			int sb = B(source[i]);
			if ((sr != 0) || (sg != 0) || (sb != 0))
            {
//				ta = sa;
				tr = sr;
				tg = sg;
				tb = sb;
            }
			target[i] = (tr << 16) | (tg << 8) | tb;
	    }
	}
    else
    {
	    for (int i = 0; i < nrPixels; i++)
        {
//			int shift = CHANTOSHIFT(channel);
	    }
	}
}

// doet een van de 5 mogelijkheden
void TextureGenerator::FilterFill(unsigned int *target, unsigned int *source, int channel, int hoe)
{
    switch (hoe)
    {
        case TEXGEN_COPY:
		    FilterCopy(target, source, channel);
            break;
        case TEXGEN_ADD:
		    FilterAdd(target, source, channel);
            break;
        case TEXGEN_SUB:
		    FilterSub(target, source, channel);
            break;
        case TEXGEN_MUL:
		    FilterMul(target, source, channel);
            break;
        case TEXGEN_NOTNULLCOPY:
		    FilterNotNullCopy(target, source, channel);
            break;
    }
}

// TODO: per channel mixen
void TextureGenerator::FilterMix(unsigned int * target, unsigned int * source, int alpha, int channel)
{
        unsigned int maxAlpha = 0xffffffff;
        unsigned int newAlpha = ((unsigned int)alpha << 24) | (alpha << 16) | (alpha << 8) | alpha;

        int aantal = nrPixels;
        __asm
        {
            mov       ecx, [aantal]
            mov       edi, [target]
            mov       esi, [source]
        FilterMixLoop:
            pxor      mm0, mm0
            movd      mm1, [esi]
            movd      mm2, [edi]
            movd      mm3, [maxAlpha]
            movd      mm4, [newAlpha]
		    punpcklbw mm1, mm0
		    punpcklbw mm2, mm0
		    punpcklbw mm3, mm0
		    punpcklbw mm4, mm0
            psubw     mm3, mm4
            pmullw    mm2, mm3
		    psrlw     mm2, 8
            pmullw    mm1, mm4
		    psrlw     mm1, 8
            paddw     mm1, mm2
		    packuswb  mm1, mm0
		    movd      [edi], mm1
            add       edi, 4
            add       esi, 4
            dec       ecx
            jnz       FilterMixLoop
            emms
        }

  //	} else {
//	    for (int i = 0; i < resX * resY; i++) {
  //			ChanToShift(channel);
	//    }
	//}
}

// TODO: alle channels
void TextureGenerator::FilterShade(unsigned int * target, unsigned int * source, int lightning, int channel)
{
/*
		punpcklbw mm3, mm0
		pcmpeqw   mm1, mm1
		movq      mm0, mm1
		psubw     mm0, mm2
		pmullw    mm4, mm0
		paddw     mm4, mm5
		psrlw     mm4, 8
		pxor      mm0, mm0
		packuswb  mm4, mm0
		movd      [final], mm4
		emms
*/
	if (channel == CHANNEL_ALL)
    {
/*

        unsigned int newLight = ((unsigned int)lightning << 24) | (lightning << 16) |
                                (lightning << 8) | lightning;

        int aantal = nrPixels;
        __asm
        {
            mov       ecx, [aantal]
            mov       edi, [target]
            mov       esi, [source]
        @FilterShadeLoop:
            pxor      mm0, mm0
            movd      mm1, [esi]
            movd      mm2, [edi]
            movd      mm3, [newLight]
		    punpcklbw mm1, mm0
		    punpcklbw mm2, mm0
		    punpcklbw mm3, mm0
            paddw     mm1, mm3
            pmullw    mm2, mm1
		    psrlw     mm2, 8
		    packuswb  mm2, mm0
		    movd      [edi], mm2
            add       edi, 4
            add       esi, 4
            dec       ecx
            jnz       @FilterShadeLoop
            emms
        }
*/

	    for (int i = 0; i < nrPixels; i++)
        {
//			int sa = A(source[i]) + lightning;
			int sr = R(source[i]) + lightning;
			int sg = G(source[i]) + lightning;
			int sb = B(source[i]) + lightning;
//			int ta = (A(target[i]) * sa) >> 8;
			int tr = (R(target[i]) * sr) >> 8;
			int tg = (G(target[i]) * sg) >> 8;
			int tb = (B(target[i]) * sb) >> 8;
//			if (ta > 255) ta = 255;
			if (tr > 255) tr = 255;
			if (tg > 255) tg = 255;
			if (tb > 255) tb = 255;
			target[i] = (tr << 16) | (tg << 8) | tb;
	    }
	}
    else
    {
        int shift = CHANTOSHIFT(channel);
	    for (int i = 0; i < nrPixels; i++)
        {
			int ss = ((source[i] >> shift) & 255) + lightning;
//			int ta = (A(target[i]) * ss) >> 8;
			int tr = (R(target[i]) * ss) >> 8;
			int tg = (G(target[i]) * ss) >> 8;
			int tb = (B(target[i]) * ss) >> 8;
//			if (ta > 255) ta = 255;
			if (tr > 255) tr = 255;
			if (tg > 255) tg = 255;
			if (tb > 255) tb = 255;
			target[i] = (tr << 16) | (tg << 8) | tb;
	    }
    }
}



/*

	GENERATION FILTERS

*/

// standaard ouderwets plasmatje    byte          byte          byte         byte         byte         byte
void TextureGenerator::FilterPlasma(int aantal_x, int aantal_y, int shift_x, int shift_y, int channel, int hoe)
{
	FilterClear(layerTemp1, CHANNEL_ALL);

	int index = 0;

    if (channel == CHANNEL_ALL)
    {
	    for (int y = 0; y < height; y++) {
		    for (int x = 0; x < width; x++) {
                int c = (int)(127 + 63.f * sin(2.f * aantal_x * (x + shift_x) * _PIDIV256) +
                                    63.f * sin(2.f * aantal_y * (y + shift_y) * _PIDIV256));
                layerTemp1[index++] = (c << 16) | (c << 8) | c;
		    }
        }
    }
    else
    {
    	int shift = CHANTOSHIFT(channel);
	    for (int y = 0; y < height; y++) {
		    for (int x = 0; x < width; x++) {
                int c = (int)(127 + 63.f * sin(2.f * aantal_x * (x + shift_x) * _PIDIV256) +
                                    63.f * sin(2.f * aantal_y * (y + shift_y) * _PIDIV256));
                layerTemp1[index++] = (c << shift);
            }
		}
	}

	FilterFill(layerActive, layerTemp1, channel, hoe);
}

// genereer botte cell structuur   byte         byte     dword          byte         byte
void TextureGenerator::FilterCells(int hoeveel, int mul, unsigned int iRand, int channel, int hoe)
{
	FilterClear(layerTemp2, CHANNEL_ALL);

	float muller = (1.f / 256) * mul * 10;

	for (int y = 0; y < height; y++)
    {
		for (int x = 0; x < width; x++)
        {
			float xc = (float) x - halfX;
			float yc = (float) y - halfY;
			float dist = muller * (float) sqrt((xc * xc) + (yc * yc));
			if (dist > 255) dist = 255;
			layerTemp2[(y * width) + x] = (int) dist;
		}
	}

	randTemp = iRand;

	for (int o = 0; o < nrPixels; o++)
    {
		layerTemp1[o] = 255;
    }

	int randnext = 0;

	for (int a = 0; a < hoeveel; a++)
    {
		int x = rand(randnext++) & clipWidth;
        int y = rand(randnext++) & clipHeight;
		int next = 0;
		for (int yy = y - halfY; yy < y + halfY; yy++)
        {
			for (int xx = x - halfX; xx < x + halfX; xx++)
            {
				int u = xx & (width - 1);
				int v = yy & (height - 1);
				DWORD col = layerTemp2[next++];
				if (layerTemp1[(v * width) + u] > col)
                {
					layerTemp1[(v * width) + u] = col;
                }
			}
		}
	}

    if (channel == CHANNEL_ALL) {
	    for (int  i = 0; i < nrPixels; i++)
        {
		    layerTemp1[i] |= (layerTemp1[i] << 24) | (layerTemp1[i] << 16) | (layerTemp1[i] << 8);
        }
    }
    else {
	    int shift = CHANTOSHIFT(channel);
	    for (int  i = 0; i < nrPixels; i++)
        {
		    layerTemp1[i] = (layerTemp1[i] << 24) | (layerTemp1[i] << shift);
        }
    }

	FilterFill(layerActive, layerTemp1, channel, hoe);
}

// onderdeel van fractal plasma code
void TextureGenerator::FillmapFrac(unsigned int * mapje, int size, float ampli) {
	 static int costab[256];

	FilterClear(mapje, CHANNEL_ALL);

	int p0,p1;
	float muller = ampli / 256;
	int y, x;

	int next=0;

	for (y = 0; y < height; y += size)
    {
		for (x = 0; x < width; x += size)
        {
			mapje[(y * width) + x] = (int) ((rand(next++) & 255) * muller);
			mapje[(y * width) + x] <<= 16;
		}
	}

	for (int g = 0; g < size; g++)
    {
		costab[g] = (int) (256.f * ((1 - (float)cos(1.0 * g * _PI / size)) * 0.5));
    }

	for (y = 0; y < height; y += size)
    {
		for (x = 0; x < width; x++)
        {
			int xmin = x & ((width - 1) - (size - 1));
			int xmax = (xmin + size) & (width - 1);
			p0 = mapje[(y * width) + xmin];
			p1 = mapje[(y * width) + xmax];
			mapje[(y * width) + x] = p0 + ((costab[x&(size-1)]*(p1-p0))>>8);
		}
	}

	for (y = 0; y < height; y++)
    {
		if (y | (y & ((height - 1) - (size - 1))))
        {
			int ymin = y & ((height - 1) - (size - 1));
			int ymax = (ymin + size) & (height - 1);
			ymin *= width;
			ymax *= width;
			for (x = 0; x < width; x++)
            {
				p0 = mapje[(ymin) + x];
				p1 = mapje[(ymax) + x];
				mapje[(y * width) + x] = p0 + ((costab[y&(size-1)]*(p1-p0))>>8);
			}
		}
	}
}

// genereerd bot fractal plasma
void TextureGenerator::FilterFractalPlasma(int factor, unsigned int iRand, int channel, int hoe) {
	randTemp = iRand;

	if (factor < 2)
		factor = 2;

	FillmapFrac(layerTemp1, pow(2, factor), pow(2, factor));
	int size = pow(2, factor - 1);
	float val = pow(2, factor - 1);
	while (size > 0)
    {
		FillmapFrac(layerTemp2, size, val);
		FilterAdd(layerTemp1, layerTemp2, CHANNEL_ALL);
		size >>= 1;
		val /= 2.0;
	}

    if (channel == CHANNEL_ALL)
    {
	    for (int i = 0; i < nrPixels; i++)
        {
            int c = (layerTemp1[i] >> 16);
		    layerTemp1[i] = (c << 16) | (c << 8) | c;
	    }
    }
    else
    {
	    int shift = CHANTOSHIFT(channel);
	    for (int i = 0; i < nrPixels; i++)
        {
            int c = layerTemp1[i] >> 16;
		    layerTemp1[i] =  (c << shift);
	    }
    }

	FilterFill(layerActive, layerTemp1, channel, hoe);
}

// draws text                    // byte ptr  byte      dword               byte      short   short   byte     byte       byte      byte
void TextureGenerator::FilterText(char * txt, int font, unsigned int color, int size, int xx, int yy, bool aa, int style, int clip, int hoe) {
    int i;

	FilterClear(layerTemp1, CHANNEL_ALL);

    char *fontlist[]={"Arial", "Courier New", "Times New Roman", "Symbol"};

    HDC tempdc=CreateCompatibleDC(GetDC(NULL));

    HBITMAP tempbmp=CreateCompatibleBitmap(GetDC(NULL), width, height);
    SelectObject(tempdc,tempbmp);

	unsigned int quality;
	if (aa)
    {
		quality = ANTIALIASED_QUALITY;
    }
	else
    {
		quality = 0;
    }

    HFONT fontje = CreateFont(size*2,0,0,0,50*(style&15),style>>4,0,0,0,0,0,quality,DEFAULT_PITCH,fontlist[font]);
    SelectObject(tempdc,fontje);
    SetBkMode(tempdc,TRANSPARENT);

	color = (R(color) << 0) | (G(color) << 8) | (B(color) << 16);

    SetTextColor(tempdc, color);

	RECT r = {0, 0, width, height};
	FillRect(tempdc, &r, CreateSolidBrush(0));

    TextOut(tempdc, xx, yy, txt, strlen(txt));

    if (clip)
    {
        TextOut(tempdc, xx - width, yy, txt, strlen(txt));
        TextOut(tempdc, xx - width, yy - height, txt, strlen(txt));
        TextOut(tempdc, xx, yy - height, txt, strlen(txt));
    }

	BITMAPINFOHEADER lpbi;
    ZeroMemory(&lpbi,sizeof(BITMAPINFOHEADER));
	lpbi.biSize        = sizeof(BITMAPINFOHEADER);
	lpbi.biWidth       = width;
    lpbi.biHeight      = height;
	lpbi.biPlanes      = 1;
	lpbi.biBitCount    = 32;
	lpbi.biCompression = BI_RGB;

    for(i = 0; i < height; i++)
    {
        GetDIBits(tempdc, tempbmp, height-1 - i, 1, &layerTemp1[i * width], (LPBITMAPINFO)&lpbi, DIB_RGB_COLORS);
    }

	FilterFill(layerActive, layerTemp1, CHANNEL_ALL, hoe);

    DeleteObject(fontje);
    DeleteObject(tempbmp);
    DeleteDC(tempdc);
}

// !!! GEBRUIKT ALPHA CHANNEL VAN COLOR VOOR OPTIE'S (FLAGS DUS)
// volledig identitiek aan psp5    short  short  short     byte         byte          byte         dword
void TextureGenerator::FilterBrush(int x, int y, int size, int density, int hardness, int opacity, unsigned int color)
{
    unsigned char options = A(color);

    bool wrap = options & 1;
    bool cont = options & 2;

	int half = size >> 1;

	int x1 = x - half;
	int x2 = x + half;
	int y1 = y - half;
	int y2 = y + half;


    if (!wrap)
    {
	    if (x1 < 0)
            x1 = 0;
	    if (y1 < 0)
            y1 = 0;

	    if (x2 > clipWidth)
            x2 = clipWidth;
	    if (y2 > clipHeight)
            y2 = clipHeight;
    }

	int rr = R(color);
	int gg = G(color);
	int bb = B(color);

    int colorTar;

    randTemp = 0;

    int next = 0;

	for (int yy = y1; yy <= y2; yy++)
    {
		for (int xx = x1; xx <= x2; xx++)
        {
			float di = sqrt(((x - xx) * (x - xx)) + ((y - yy) * (y - yy)));

			int tekenen = rand(next++) & 255;

			if ((di < half) && (tekenen <= density)) {
				int r = rr;
				int g = gg;
				int b = bb;

				int mix;


				if (di < ((half * hardness) >> 8))
					mix = 255;
				else
					mix = 255 - ((di - ((half * hardness) >> 8)) * (255.f / (half - ((half * hardness) >> 8))));

				mix -= 255 - opacity;

				if (mix < 0)
					mix = 0;

				if (mix > 255)
					mix = 255;

                if (wrap)
                {
					colorTar = layerActive[((yy & clipHeight) * width) + (xx & clipWidth)];
                }
                else
                {
					colorTar = layerActive[(yy * width) + xx];
                }

                int tr, tg, tb;
                int sr, sg, sb;

                tr = R(colorTar);
                tg = G(colorTar);
                tb = B(colorTar);

                if (cont)
                {
                    float m = (((mix / 255.f) * 140.f) + 32.f) / 32.f;
                    tr = ((tr - (128 + (255 - r))) * m) + (128 + (255 - r));
                    tg = ((tg - (128 + (255 - g))) * m) + (128 + (255 - g));
                    tb = ((tb - (128 + (255 - b))) * m) + (128 + (255 - b));
                }
                else
                {
				    tr = (tr * (255 - mix)) >> 8;
				    tg = (tg * (255 - mix)) >> 8;
				    tb = (tb * (255 - mix)) >> 8;
				    sr = (r * mix) >> 8;
				    sg = (g * mix) >> 8;
				    sb = (b * mix) >> 8;
				    tr += sr;
				    tg += sg;
				    tb += sb;
                }

                if (tr < 0) tr = 0;
                if (tg < 0) tg = 0;
                if (tb < 0) tb = 0;

                if (tr > 255) tr = 255;
                if (tg > 255) tg = 255;
                if (tb > 255) tb = 255;

				color = (tr << 16) | (tg << 8) | (tb);

                if (wrap)
                {
				    layerActive[((yy & clipHeight) * width) + (xx & clipWidth)] = color;
                }
                else
                {
				    layerActive[(yy * width) + xx] = color;
                }
			}
		}
	}
}

// genereert noise                 byte          byte          byte           dword               byte         byte
void TextureGenerator::FilterNoise(int strength, bool uniform, bool eenkleur, unsigned int iRand, int channel, int hoe)
{
	FilterClear(layerTemp1, CHANNEL_ALL);

	randTemp = iRand;

	int teller = 0;

	if (uniform) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int a = rand(teller++) & 255;
				int r = rand(teller++) & 255;
				int g = rand(teller++) & 255;
				int b = rand(teller++) & 255;
				if (eenkleur)
					layerTemp1[(y * width) + x] = (a << 16) | (a << 8) | (a);
				else
					layerTemp1[(y * width) + x] = (r << 16) | (g << 8) | (b);
			}
		}
        FilterMix(layerActive, layerTemp1, strength, channel);
	} else {
		for (int i = 0; i < (((nrPixels) * strength) >> 8); i++) {
				int a = (strength * (rand(teller++) & 255)) >> 8;
				int r = (strength * (rand(teller++) & 255)) >> 8;
				int g = (strength * (rand(teller++) & 255)) >> 8;
				int b = (strength * (rand(teller++) & 255)) >> 8;
				int x = rand(teller++) & clipWidth;
				int y = rand(teller++) & clipHeight;
				if (eenkleur)
					layerTemp1[(y * width) + x] = (a << 16) | (a << 8) | (a);
				else
					layerTemp1[(y * width) + x] = (r << 16) | (g << 8) | (b);
		}
		FilterFill(layerActive, layerTemp1, channel, hoe);
	}
}

void TextureGenerator::FilterRectangle(short x1, short y1, short x2, short y2, unsigned int color, unsigned char hoe)
{
	FilterClear(layerTemp1, CHANNEL_ALL);

	for (int y = y1; y <= y2; y++)
    {
		for (int x = x1; x <= x2; x++)
        {
			layerTemp1[(y * width) + x] = color;
		}
	}

	FilterFill(layerActive, layerTemp1, CHANNEL_ALL, hoe);
}




/*

	BLUR FILTERS

*/

// own code... most near to adobe filter
void TextureGenerator::FilterGaussianBlur(unsigned int radius)
{
	FilterClear(layerTemp1, CHANNEL_ALL);

	int x, y;
	int rx, ry;
	int offset=0;
	int aantal=0;
	int redbuf, greenbuf, bluebuf;
    int xstart = 9, xend = 9, ystart = 9, yend = 9;

	float filter[19 * 19];
	for (y = 0; y < 19; y++)
	{
		for (x = 0; x < 19; x++)
		{
			float bX = x - 9;
			float bY = y - 9;
			float length = sqrt(bX * bX + bY * bY) * 1.08f;
            float temp = length * (4.f / ((radius + 1) / 16.f));
            if (temp > 5)
                temp = 5;
			float value = 100 - exp(temp
            );
			if (value < 0)
				value = 0;
			if (value > 100)
				value = 100;
			filter[(y * 19) + x] = value;
		}
	}

	for (y = 0; y < 19; y++)
	{
		for (x = 0; x < 19; x++)
		{
            if (filter[(y * 19) + x] > 0.f)
            {
                if (x < xstart)
                    xstart = x;
                if (x > xend)
                    xend = x;
                if (y < ystart)
                    ystart = y;
                if (y > yend)
                    yend = y;
            }
        }
    }

    xstart -= 9;
    ystart -= 9;
    xend   -= 9;
    yend   -= 9;
/*
    xstart = -3;
    xend = 3;
    ystart = -3;
    yend = 3;
  */
	for (y = 0; y < height; y++)
    {
		for (x = 0; x < width; x++)
        {
			float rdF = 0, grF = 0, blF = 0;
			float total = 0;
//			int filterOffset = 0;
			for (ry = ystart; ry <= yend; ry++)
            {
				for (rx = xstart; rx <= xend; rx++)
                {
					float value = filter[((ry + 9) * 19) + (rx + 9)];
					if (value > 0.0f)
					{
						int posX = x + rx;
						int posY = y + ry;
						int index = (((posY) & clipHeight) * width) + ((posX) & clipWidth);
						rdF += R(layerActive[index]) * value;
						grF += G(layerActive[index]) * value;
						blF += B(layerActive[index]) * value;
						total += value;
					}
				}
			}
            if (total == 0)
                total = 1;
			redbuf   = (rdF / total);
			greenbuf = (grF / total);
			bluebuf  = (blF / total);
			layerTemp1[offset] = ((redbuf << 16) + (greenbuf << 8) + bluebuf);
			offset++;
		}
	}

	FilterCopy(layerActive, layerTemp1, CHANNEL_ALL);
}

void TextureGenerator::FilterMotionBlur(unsigned char iangle, unsigned char radius)
{
	FilterClear(layerTemp1, CHANNEL_ALL);

	float angle = iangle;
	int x, y;
	int offset = 0;
	int i;
	int redbuffer, greenbuffer, bluebuffer;
	int color;
	float rx, ry;
	float ctempangle, stempangle;
    int numprut;

	ctempangle = cos(_PI * angle / 127.0f);
	stempangle = sin(_PI * angle / 127.0f);

	for (y = 0; y < height; y++)
    {
		for (x = 0; x < width; x++)
        {
//            alphabuffer = 0;
			redbuffer   = 0;
			greenbuffer = 0;
			bluebuffer  = 0;
            numprut     = 0;
			for (i = -radius; i <= radius; i++)
            {
			    rx = ((float) x + (float) i * ctempangle);
				ry = ((float) y - (float) i * stempangle);
				while (rx > (clipWidth)) rx -= width;
				while (ry > (clipHeight)) ry -= height;
				while (rx < 0) rx += width;
				while (ry < 0) ry += height;

				color = FilterBilPixel(layerActive, rx, ry);

//				alphabuffer += A(color);
				redbuffer   += R(color);
				greenbuffer += G(color);
				bluebuffer  += B(color);
				numprut++;
            }

//            alphabuffer /= numprut;
            redbuffer   /= numprut;
            greenbuffer /= numprut;
            bluebuffer  /= numprut;

            layerTemp1[offset] = (redbuffer << 16) + (greenbuffer << 8) + (bluebuffer);

            offset++;
        }
	}

	FilterCopy(layerActive, layerTemp1, CHANNEL_ALL);
}

void TextureGenerator::FilterDirBlur(unsigned int * source, int aantal, int channel)
{
    int shift = CHANTOSHIFT(channel);
    int red, green, blue;
    int next = 0;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            red = green = blue = 0;
            int a       = (source[next] >> shift) & 255;
            float angle = (a) * (_PI / 128.f);
            float sa    = sin(angle);
            float ca    = cos(angle);
            for (int i = 0; i < aantal; i++) {
                float u = (float) x + (2.f * i) * sa;
                float v = (float) y + (2.f * i) * ca;
                int col = FilterBilPixel(layerActive, u, v);
//                alpha += A(col);
                red   += R(col);
                green += G(col);
                blue  += B(col);
            }
//            alpha /= aantal;
            red   /= aantal;
            green /= aantal;
            blue  /= aantal;
            layerTemp1[next++] = (red << 16) | (green << 8) | blue;
        }
    }

    FilterCopy(layerActive, layerTemp1, CHANNEL_ALL);
}

void TextureGenerator::FilterConvolution(int grid[3][3], int adder, int div)
{
	FilterClear(layerTemp1, CHANNEL_ALL);

	int r, g, b;
    unsigned int col;

	for (int y = 0; y < height; y++)
    {
		for (int x = 0; x < width; x++)
        {
			r = g = b = 0;
			for (int j = 0; j < 3; j++)
            {
				for (int i = 0; i < 3; i++)
                {
					int mul = grid[j][i];
					col = layerActive[(((x - 1) + i) & clipWidth) + ((((y - 1) + j) & clipHeight) * width)];
//					a += mul * A(col);
					r += mul * R(col);
					g += mul * G(col);
					b += mul * B(col);
				}
			}
//			a /= div;
			r /= div;
			g /= div;
			b /= div;
//			a += adder;
			r += adder;
			g += adder;
			b += adder;
//			if (a > 255) a = 255;
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			if (r < 0) r = 0;
			if (g < 0) g = 0;
			if (b < 0) b = 0;
			layerTemp1[(y * width) + x] = (r << 16) | (g << 8) | b;
		}
	}

	FilterCopy(layerActive, layerTemp1, CHANNEL_ALL);
}

void TextureGenerator::FilterEmboss()
{
	int grid[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
	FilterConvolution(grid, 128, 1);
}

void TextureGenerator::FilterBoxBlur()
{
	int grid[3][3] = {{ 0, 1, 0}, { 1, 4, 1}, { 0, 1, 0}};
	FilterConvolution(grid, 0, 8);
}

void TextureGenerator::FilterSobel()
{
	int grid[3][3] = {{ -1, -2, -1}, {  0,  0,  0}, {  1,  2,  2}};
	FilterConvolution(grid, 0, 1);
}

void TextureGenerator::FilterEdge()
{
	int grid[3][3] = {{ -1, -1, -1}, { -1, 9, -1}, { -1, -1, -1}};
	FilterConvolution(grid, 0, 1);
}

void TextureGenerator::FilterTopLeft()
{
	int grid[3][3] = {{-1, -1, 1},{-1, 0, 1},{-1, 1, 1}};
	FilterConvolution(grid, 128, 1);
}





/*

	COLOR FILTERS

*/

// onderdeel van hsv filter
unsigned int TextureGenerator::hsv_to_rgb(float h, float s, float v)
{
	float r, g, b;

    if (s == 0.0)
    {
        r = v;
        g = v;
        b = v;
    }
    else
    {
        float f, p, q, t;
        float i;

        while (h >= 360.0f)
            h -= 360.0f;

        while (h < 0.0f)
            h += 360.0f;

        h /= 60.0f;
        i = floor(h);
        f = h - i;
        p = v * (1.0f - s);
        q = v * (1.0f - (s * f));
        t = v * (1.0f - (s * (1.0f - f)));

        int o = i;
        switch(o) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }
    }

    int ir, ig, ib;

    ir = r;
    ig = g;
    ib = b;

    if (ir > 255)
        ir = 255;
    if (ig > 255)
        ig = 255;
    if (ib > 255)
        ib = 255;

   return (ir << 16) | (ig << 8) | ib;
}

// onderdeel van hsv filter
void TextureGenerator::rgb_to_hsv(float r, float g, float b, float* h, float* s, float* v)
{
    float max, min;
    float temp;

    if (r > g)
        max = r;
    else
        max = g;
    if (b > max)
        max = b;

    if (r < g)
        min = r;
    else
        min = g;
    if (b < min)
        min = b;

    *v = max;

    *s = 0.0f;
    if (max != 0.0f)
        *s = ((max - min) / max);

    if (*s == 0.0f)
        *h = -1;
    else {
        float delta = max - min;
        float maxr = max - r;
        float maxg = max - g;
        float maxb = max - b;

        if (r == max)
            *h = ((maxb) - (maxg)) / delta;
        else if (g == max)
            *h = 2.0f + (((maxr) - (maxb)) / delta);
        else if (b == max)
            *h = 4.0f + (((maxg) - (maxr)) / delta);

        *h *= 60.0f;
        while (*h < 0.0f)
            *h += 360.0f;
        while (*h >= 360.0f)
            *h -= 360.0f;
   }
}

// roteer rgb waarden
void TextureGenerator::FilterHsv(int rotate, int sat) {
	float hhh, sss, vvv;

	float rot = rotate;
	rot = rot * (360.0 / 256.0);

	float satter = (1.0 / 255.0) * (255 - sat);

	for (int y = 0; y < nrPixels; y++) {
		unsigned int col = layerActive[y];
//        int a = A(col);
		int r = R(col);
		int g = G(col);
		int b = B(col);
		rgb_to_hsv((float)r, (float)g, (float)b, &hhh, &sss, &vvv);
		hhh += rot;
		sss *= satter;
		layerActive[y] = hsv_to_rgb(hhh, sss, vvv);
	}
}

void TextureGenerator::FilterContrast(int hoeveel)
{
    float m = hoeveel / 32.f;
    for (int i = 0; i < nrPixels; i++)
    {
        unsigned int col = layerActive[i];
        int r = ((R(col) - 128.f) * m) + 128;
        int g = ((G(col) - 128.f) * m) + 128;
        int b = ((B(col) - 128.f) * m) + 128;

        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;

        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 255)
            b = 255;

        layerActive[i] = (r << 16) | (g << 8) | b;
    }
}

void TextureGenerator::FilterBrightness(int hoeveel)
{
    for (int i = 0; i < nrPixels; i++)
    {
        unsigned int col = layerActive[i];
        int r = R(col);
        int g = G(col);
        int b = B(col);

        if (hoeveel < 128)
        {
            r = (r * hoeveel) >> 7;
            g = (g * hoeveel) >> 7;
            b = (b * hoeveel) >> 7;
        }
        else
        {
            r = r + (((255.f - r) / 128.f) * (hoeveel - 128));
            g = g + (((255.f - g) / 128.f) * (hoeveel - 128));
            b = b + (((255.f - b) / 128.f) * (hoeveel - 128));
        }

        layerActive[i] = (r << 16) | (g << 8) | b;
    }
}

void TextureGenerator::FilterInvert()
{
    for (int i = 0; i < nrPixels; i++)
    {
        layerActive[i] ^= 0xffffffff;
    }
}

void TextureGenerator::FilterSineDistort(float nrX, float nrY, float ampX, float ampY, float shiftX, float shiftY)
{
    int t = 0;

    for (int y = 0 ; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float u = (float) x + (ampX * sin(2.f * nrX * _PIDIV256 * (y + shiftX)));
            float v = (float) y + (ampY * cos(2.f * nrY * _PIDIV256 * (x + shiftY)));
            layerTemp1[t++] = FilterBilPixel(layerActive, u, v);
        }
    }

    FilterCopy(layerActive, layerTemp1, CHANNEL_ALL);
}

void TextureGenerator::FilterTile(float repX, float repY)
{
    int t = 0;

    for (int y = 0 ; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            layerTemp1[t++] = FilterBilPixel(layerActive, x * repX, y * repY);
        }
    }

    FilterCopy(layerActive, layerTemp1, CHANNEL_ALL);
}

unsigned int TextureGenerator::FilterBilPixel(unsigned int *mapje, float x_Real, float y_Real)
{
/*
    int u1 = ((int) floor(x_Real)) & (width  - 1);
    int v1 = ((int) floor(y_Real)) & (height - 1);

    int u2 = (u1 + 1) & (width  - 1);
    int v2 = (v1 + 1) & (height - 1);

    int c1 = mapje[(v1 * width) + u1];
    int c2 = mapje[(v1 * width) + u2];
    int c3 = mapje[(v2 * width) + u1];
    int c4 = mapje[(v2 * width) + u2];

    int r1 = R(c1);
    int g1 = G(c1);
    int b1 = B(c1);

    int r2 = R(c2);
    int g2 = G(c2);
    int b2 = B(c2);

    int r3 = R(c3);
    int g3 = G(c3);
    int b3 = B(c3);

    int r4 = R(c4);
    int g4 = G(c4);
    int b4 = B(c4);

    float fu1 = (float) 1.0f - (x_Real - u1);
    float fv1 = (float) 1.0f - (y_Real - v1);

    float fu2 = 1.0f - fu1;
    float fv2 = 1.0f - fv1;

    int rf = (((r1 * fu1) + (r2 * fu2)) * fv1) +
             (((r3 * fu1) + (r4 * fu2)) * fv2);

    int gf = (((g1 * fu1) + (g2 * fu2)) * fv1) +
             (((g3 * fu1) + (g4 * fu2)) * fv2);

    int bf = (((b1 * fu1) + (b2 * fu2)) * fv1) +
             (((b3 * fu1) + (b4 * fu2)) * fv2);

    return (rf << 16) | (gf << 8) | bf;
*/
	// mijn eigen multi res crap (MMX > 1.5 jaar geleden voor het laatst gedaan :(

    char shift = shiftX;
    int clipX = clipWidth;
    int clipY = clipHeight;
	int   fixedU, fixedV;
	float mul256 = 256.f;
	int   fractU, fractV;
	unsigned int final;

	__asm
	{
		// converteer x_real en y_real naar 8.8 fixed point getallen
		fld       [x_Real]
		fld       [y_Real]
		fmul      [mul256]
		fistp     [fixedV]
		fmul      [mul256]
		fistp     [fixedU]

		// converteer de .8 bits (factor naar bruikbaar formaat (4x in dword dus)
		mov       eax, [fixedU]
		mov       ebx, [fixedV]
		and       eax, 255
		and       ebx, 255
		mov       ecx, eax
		mov       edx, ebx
		shl       eax, 8
		shl       ebx, 8
		add       eax, ecx
		add       ebx, edx
		shl       eax, 8
		shl       ebx, 8
		add       eax, ecx
		add       ebx, edx
		mov       [fractU], eax
		mov       [fractV], ebx

		// laat de pixels in mem (van mm4 tot en met mm7)
		pxor      mm0, mm0

		mov       eax, [fixedU]
        shr       eax, 8
        and       eax, [clipX]
		mov       ebx, [fixedV]
		shr       ebx, 8
        and       ebx, [clipY]
		push      ebx
		mov       cl,  [shift]
		shl       ebx, cl

		mov       esi, mapje

		push      eax
		push      ebx
		add       ebx, eax
		movd      mm4, [esi + ebx * 4]	// pixel linksboven

		pop       ebx
		inc       eax
		and       eax, [clipX]
		add       ebx, eax
		movd      mm5, [esi + ebx * 4]	// pixel rechtsboven

		pop       eax

		pop       ebx
		inc       ebx
		and       ebx, [clipY]
		shl       ebx, cl
		push      ebx

		add       ebx, eax
		movd      mm6, [esi + ebx * 4]	// pixel linksonder

		pop       ebx
		inc       eax
		and       eax, [clipX]
		add       ebx, eax
		movd      mm7, [esi + ebx * 4]	// pixel rechtsonder

		punpcklbw mm4, mm0
		punpcklbw mm5, mm0
		punpcklbw mm6, mm0
		punpcklbw mm7, mm0


		// zet de fract shit in MMX mem
		movd      mm2, [fractU]
		movd      mm3, [fractV]

		punpcklbw mm2, mm0
		punpcklbw mm3, mm0

		pcmpeqw   mm1, mm1
		punpcklbw mm1, mm0
		movq      mm0, mm1

		psubw     mm0, mm2
		psubw     mm1, mm3

		// tussenstand :
		// mm0 = factor voor links
		// mm1 = factor voor boven
		// mm2 = factor voor rechts
		// mm3 = factor voor onder


		// bilineare shit doen dan maar:

		pmullw    mm4, mm0
		pmullw    mm5, mm2
		paddw     mm4, mm5
		psrlw     mm4, 8
		pmullw    mm4, mm1


		pmullw    mm6, mm0
		pmullw    mm7, mm2
		paddw     mm6, mm7
		psrlw     mm6, 8
		pmullw    mm6, mm3

		paddw     mm4, mm6
		psrlw     mm4, 8

		pxor      mm0, mm0

		packuswb  mm4, mm0

		movd      [final], mm4

		emms
	}

	return final;
}

