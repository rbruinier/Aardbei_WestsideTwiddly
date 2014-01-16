// V1.00 (09-10-2001)
#ifndef __FILTERS_H__
#define __FILTERS_H__

#define TEXGEN_HEADER1 "ATS0"
#define TEXGEN_HEADER2 "ATS1"

#define _PI       3.1415926535f
#define _PI2      6.2831853070f
#define _PIDIV256 0.0122718463f

#define A(color) (((color) >> 24) & 255)
#define R(color) (((color) >> 16) & 255)
#define G(color) (((color) >>  8) & 255)
#define B(color) (((color) >>  0) & 255)

#define CHANTOSHIFT(channel) 24 - (8 * channel)

#define CHANNEL_ALPHA	0
#define CHANNEL_RED		1
#define CHANNEL_GREEN	2
#define CHANNEL_BLUE	3
#define CHANNEL_ALL		4

// layer stuff
#define TEXGEN_CLEAR			10
#define TEXGEN_COPY				11
#define TEXGEN_ADD				12
#define TEXGEN_SUB				13
#define TEXGEN_MUL				14
#define TEXGEN_NOTNULLCOPY		15
#define TEXGEN_NEWLAYER         16
#define TEXGEN_CHANGELAYER      17
#define TEXGEN_MIX				18
#define TEXGEN_SHADE			19

// generation stuff
#define TEXGEN_PLASMA			30
#define TEXGEN_CELLS			31
#define TEXGEN_FRACTALPLASMA	32
#define TEXGEN_TEXT				33
#define TEXGEN_BRUSH			34
#define TEXGEN_NOISE			35
#define TEXGEN_RECTANGLE		36
//#define TEXGEN_WALL				37


// weird stuff (blurs/embosses)
#define TEXGEN_CONVOLUTION		70
#define TEXGEN_GAUSSIANBLUR		71
#define TEXGEN_MOTIONBLUR		72
#define TEXGEN_BOXBLUR			73
#define TEXGEN_EMBOSS			74
#define TEXGEN_SOBEL			75
#define TEXGEN_EDGE				76
#define TEXGEN_TOPLEFT			77
#define TEXGEN_DIRBLUR			78

// color stuff
#define TEXGEN_HSV				90
#define TEXGEN_CONTRAST         91
#define TEXGEN_BRIGHTNESS       92
#define TEXGEN_INVERT           93

// distortion stuff
#define TEXGEN_SINEDISTORT      110
#define TEXGEN_TILE             111


class TextureGenerator
{
public:

    TextureGenerator(int iWidth, int iHeight);
    ~TextureGenerator();

    void AddLayer();

    unsigned int rand(unsigned int _x);

    // common (needed) layer filters
    void FilterClear(unsigned int * target, int channel);
    void FilterCopy(unsigned int *target, unsigned int *source, int channel);
    void FilterAdd(unsigned int *target, unsigned int *source, int channel);
    void FilterSub(unsigned int *target, unsigned int *source, int channel);
    void FilterMul(unsigned int *target, unsigned int *source, int channel);
    void FilterNotNullCopy(unsigned int *target, unsigned int *source, int channel);
    void FilterFill(unsigned int *target, unsigned int *source, int channel, int hoe);
    void FilterMix(unsigned int * target, unsigned int * source, int alpha, int channel);
    void FilterShade(unsigned int * target, unsigned int * source, int lightning, int channel);
    unsigned int FilterBilPixel(unsigned int *mapje, float x_Real, float y_Real);

    // generation filters
    void FilterPlasma(int aantal_x, int aantal_y, int shift_x, int shift_y, int channel, int hoe);
    void FilterCells(int hoeveel, int mul, unsigned int iRand , int channel, int hoe);
    void FillmapFrac(unsigned int * mapje, int size, float ampli);
    void FilterFractalPlasma(int factor, unsigned int iRand, int channel, int hoe);
    void FilterText(char * txt, int font, unsigned int color, int size, int xx, int yy, bool aa, int style, int clip, int hoe);
    void FilterBrush(int x, int y, int size, int density, int hardness, int opacity, unsigned int color);
    void FilterNoise(int strength, bool uniform, bool eenkleur, unsigned int iRand, int channel, int hoe);
    void FilterRectangle(short x1, short y1, short x2, short y2, unsigned int color, unsigned char hoe);

    // blur filters
    void FilterGaussianBlur(unsigned int radius);
    void FilterMotionBlur(unsigned char iangle, unsigned char radius);
    void FilterDirBlur(unsigned int * source, int aantal, int channel);
    void FilterConvolution(int grid[3][3], int adder, int div);
    void FilterEmboss();
    void FilterBoxBlur();
    void FilterSobel();
    void FilterEdge();
    void FilterTopLeft();

    // color filters
    unsigned int hsv_to_rgb(float h, float s, float v);
    void rgb_to_hsv(float r, float g, float b, float* h, float* s, float* v);
    void FilterHsv(int rotate, int sat);
    void FilterContrast(int hoeveel);
    void FilterBrightness(int hoeveel);
    void FilterInvert();

    // distortion filters
    void FilterSineDistort(float nrX, float nrY, float ampX, float ampY, float shiftX, float shiftY);
    void FilterTile(float repX, float repY);

    int nrLayers;

    unsigned int *layers[128];

    unsigned int *layerActive;

private:
    unsigned int *layerTemp1;
    unsigned int *layerTemp2;

    int width;
    int height;
    int nrPixels;
    int clipWidth;
    int clipHeight;

    int halfX;
    int halfY;

    int shiftX;

    unsigned int randTemp;
};

// opslag crap

#pragma pack(1)

class Filter
{
public:
    unsigned char command;
};

// 1 byte + 1
class FilterChangeLayer : public Filter
{
public:
    unsigned char layerNr;
};

// 2 bytes + 1
class FilterCopy : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char channel;
};

// 2 bytes + 1
class FilterAdd : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char channel;
};

// 2 bytes + 1
class FilterSub : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char channel;
};

// 2 bytes + 1
class FilterMul : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char channel;
};

// 2 bytes + 1
class FilterNotNull : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char channel;
};

// 3 bytes + 1
class FilterMix : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char alpha;
    unsigned char channel;
};

// 2 bytes + 1
class FilterAlphaMix : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char channel;
};

// 3 bytes + 1
class FilterShade : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char lightning;
    unsigned char channel;
};

// 6 bytes + 1
class FilterPlasma : public Filter
{
public:
    unsigned char aantalX;
    unsigned char aantalY;
    unsigned char shiftX;
    unsigned char shiftY;
    unsigned char channel;
    unsigned char method;
};

// 8 bytes + 1
class FilterCells : public Filter
{
public:
    unsigned char density;
    unsigned char strength;
    unsigned int seed;
    unsigned char channel;
    unsigned char method;
};

// 7 bytes + 1
class FilterFractalPlasma : public Filter
{
public:
    unsigned char factor;
    unsigned int seed;
    unsigned char channel;
    unsigned char method;
};

// 142 + 1
class FilterText : public Filter
{
public:
    char text[128];
    unsigned char font;
    unsigned char size;
    unsigned short x;
    unsigned short y;
    unsigned char aa;
    unsigned char style;
    unsigned char clip;
    unsigned int color;
    unsigned char method;
};

// 13 bytes + 1
class FilterBrush : public Filter
{
public:
    signed short x;
    signed short y;
    unsigned short size;
    unsigned char density;
    unsigned char hardness;
    unsigned char opacity;
    unsigned int color;     // !!! gebruikt alpha channel byte voor opties
};

// 9 bytes + 1
class FilterNoise : public Filter
{
public:
    unsigned char strength;
    unsigned char uniform;
    unsigned char multi;
    unsigned int seed;
    unsigned char channel;
    unsigned char method;
};

// 13 bytes + 1
class FilterRectangle : public Filter
{
public:
    unsigned short x1;
    unsigned short y1;
    unsigned short x2;
    unsigned short y2;
    unsigned int color;
    unsigned char method;
};

// 1 byte + 1
class FilterGaussianBlur : public Filter
{
public:
    unsigned char radius;
};

// 2 bytes + 1
class FilterMotionBlur : public Filter
{
public:
    unsigned char angle;
    unsigned char radius;
};

// 3 bytes + 1
class FilterDirBlur : public Filter
{
public:
    unsigned char sourceNr;
    unsigned char aantal;
    unsigned char channel;
};

// 2 bytes + 1
class FilterHsv : public Filter
{
public:
    unsigned char rotation;
    unsigned char saternation;
};

// 1 byte + 1
class FilterBrightness : public Filter
{
public:
    unsigned char strength;
};

// 1 byte + 1
class FilterContrast : public Filter
{
public:
    unsigned char strength;
};

// 6 bytes + 1
class FilterSineDistort : public Filter
{
public:
    unsigned char nrX;
    unsigned char nrY;
    unsigned char ampX;
    unsigned char ampY;
    unsigned char shiftX;
    unsigned char shiftY;
};

// 2 bytes + 1
class FilterTile : public Filter
{
public:
    unsigned char repX;
    unsigned char repY;
};

#pragma pack()

#endif
