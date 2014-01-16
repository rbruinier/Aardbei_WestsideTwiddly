#ifndef __MAIN_H_
#define __MAIN_H_

// system
#include "..\Unicorn System\Unicorn.h"
#include "..\Unicorn Engine\Engine.h"

// extra system stuff
#include "Ats.h"
#include "Common.h"
#include "ScapeEffect.h"
#include "SmokeSys.h"

// effects
#include "Scape03.h"
#include "Scape05.h"
#include "Font.h"
#include "Linkin.h"
#include "Eye.h"
#include "Eye02.h"
#include "Hospital.h"
#include "Sphery01.h"
#include "Sphery02.h"
#include "Sphery03.h"
#include "Starfield.h"
#include "Atom.h"
#include "Universe.h"
#include "Kokkels.h"
#include "Clouds.h"
#include "Poem.h"
#include "Background01.h"
#include "Background02.h"
#include "Blurs.h"

// data
#include "Textures.h"

// effecten die overal gebruikt mogen worden
extern BlurMotion	   *blurMotion;
extern BlurRadial	   *blurRadial;
extern BlurHorizontal  *blurHorizontal;

extern Effect *flashWhite;
extern Effect *flashBlack;
extern Effect *fadeWhite;
extern Effect *fadeBlack;
extern Effect *flashWhiteFast; 
extern Effect *flashBlackFast;
extern Effect *fadeWhiteFast;
extern Effect *fadeBlackFast;
extern Effect *clearBuffers;
extern Effect *noise01;

extern dword clearColor;

#endif