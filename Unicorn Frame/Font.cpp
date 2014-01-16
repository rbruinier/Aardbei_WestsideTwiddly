#include "Main.h"
		
float charData[30][2] =
{
{3, 0},			// a
{22, 0},		// b
{41, 0},		// c
{59, 0},		// d
{78, 0},		// e
{96, 0},		// f
{110, 0},		// g
{129, 0},		// h
{147, 0},		// i
{147, 0},		// j
{158, 0},		// k
{175, 0},		// l
{187, 0},		// m
{212, 0},		// n
{231, 0},		// o
{250, 0},		// p
{270, 0},		// q
{288, 0},		// r
{302, 0},		// s
{320, 0},		// t
{333, 0},		// u
{354, 0},		// v
{371, 0},		// w
{396, 0},		// x
{414, 0},		// y
{432, 0},		// z
{449, 0},		// !
{462, 0},		// ?
{480, 0},		// ,
{492, 9},		// .
};

bool Font::Init()
{
	spriteDrawer = new SpriteDrawer();

	for (int i = 0; i < 29; i++)
	{
		charData[i][1] = (charData[i + 1][0] - charData[i][0]);
	}
	charData[8][1] = 176 - 163; 

	return true;
}
 

void Font::Do(char *tekst, float x, float y, dword color)
{
	uniSetTexture("font01", 0);
	uniSetTexture(NULL, 1);

	spriteDrawer->Start(UNI_ADD);

	float x1, x2, x3, x4;
	float y1, y2, y3, y4;
	float u1, u2, u3, u4;
	float v1, v2, v3, v4;

	x1 = x3 = x;

	y1 = y2 = y;
	y3 = y4 = y + 32;

	for (int i = 0; i < strlen(tekst); i++)
	{
		if (((tekst[i] >= 'a') && (tekst[i] <= 'z')) || (tekst[i] == '!'))
		{
			unsigned char current = tekst[i];
			if (current == '!')
			{
				current -= 7;
			}
			else
			{
				current -= 97;
			}
			u1 = u3 = charData[current][0] / 512.f;
			u2 = u4 = u1 + (charData[current][1] / 512.f);
			v1 = v2 = 0;
			v3 = v4 = 1;

			x2 = x4 = x1 + charData[current][1];

	
			spriteDrawer->Draw2D(x1, y1, u1, v1, color,
			                     x2, y2, u2, v2, color,
					    		 x3, y3, u3, v3, color,
						    	 x4, y4, u4, v4, color);

			x1 += charData[current][1];
			x3 += charData[current][1];
		}
		if (tekst[i] == ' ')
		{
			x1 += 13;
			x3 += 13;
		}
	}


	spriteDrawer->Stop();
}

void Font::DeInit()
{
	delete spriteDrawer;
}