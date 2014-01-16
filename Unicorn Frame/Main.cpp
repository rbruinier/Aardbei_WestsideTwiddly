/*
	..... (64KB) code

	- tree met vogels / spinnen
	- axon
*/

#include "Main.h"

#define PLAY_WITH_MUSIC
#define PLAY_WITH_SCRIPT
#define PLAY_WITH_FADEOUTBAR

#ifdef PLAY_WITH_SCRIPT
#ifndef PLAY_WITH_MUSIC
#define PLAY_WITH_MUSIC
#endif
#endif

//extern unsigned char aardbei3_[26860];
extern unsigned char script_usc[];
extern unsigned char aardbei_[7950];
extern unsigned char aardbei3_[26860];
//extern unsigned char aardbei4_[26860];
extern unsigned char aardbei5_[26870];
 
Effect *scape03;
Effect *scape05;
Effect *linkin;
Effect *eye;
Effect *eye02;
Effect *spikey;
Effect *sphery01;
Effect *sphery03;
Effect *universe;
Effect *kokkels;
Effect *clouds;
Effect *poem;
Effect *background01;
Effect *background02;

Font   *font;

Effect *flashWhite;
Effect *flashBlack;
Effect *fadeWhite;
Effect *fadeBlack;
Effect *flashWhiteFast; 
Effect *flashBlackFast;
Effect *fadeWhiteFast;
Effect *fadeBlackFast;
Effect *clearBuffers;
Effect *noise01;

BlurMotion	   *blurMotion;
BlurRadial	   *blurRadial;

#define EFFECT_STARFIELD 		10
#define EFFECT_VULCAN			11
#define EFFECT_LIQUID			12
#define	EFFECT_KOKKELS			13
#define EFFECT_SPHERY01			14
#define EFFECT_SPHERY03			15
#define EFFECT_HOSPITAL			16
#define EFFECT_SWEET			17
#define EFFECT_EYE01			18
#define EFFECT_EYE02			19
#define EFFECT_ATOM				20
#define EFFECT_UNIVERSE			21
#define EFFECT_BACKGROUND01		22
#define EFFECT_BACKGROUND02		23
#define EFFECT_CLOUDS			24
#define EFFECT_POEM				25

#define EFFECT_WHITEFL			50	
#define EFFECT_BLACKFL			51
#define EFFECT_WHITEFD			52
#define EFFECT_BLACKFD			53
#define EFFECT_WHITEFLFAST		54
#define EFFECT_BLACKFLFAST		55
#define EFFECT_WHITEFDFAST		56
#define EFFECT_BLACKFDFAST		57

#define EFFECT_NOISECONST		58
#define EFFECT_NOISEFL			59

#define EFFECT_CLEARBUFFERS		60

dword clearColor = 0;

bool precalcMusic = false;

void mainInit() {
	int i;


	uniInitSystem(800, 600, 32, 32, UNI_TNL);

	uniSetViewport(0, 90, 800, 510);

	if (strstr(uniSystem->params, "savegfx")) 
	{
		TGALoadAndSave = true;
	}
	TGALoadAndSave = true;

	if (strstr(uniSystem->params, "precalcmusic")) 
	{ 
		precalcMusic = true; 
	} 
	//precalcMusic = true; 

#ifdef PLAY_WITH_MUSIC
	fmodLoadTrack(aardbei5_, precalcMusic, SoundPrecalc);
#endif


	LoadAllTextures();

	scape03   = new Scape03();
	scape05   = new Scape05();
	linkin    = new Linkin();
	eye		  = new Eye();
	eye02	  = new Eye02();
	sphery01  = new Sphery01();
	sphery03  = new Sphery03();
	universe  = new Universe();
	kokkels   = new Kokkels();
	clouds	  = new Clouds();
	poem      = new Poem();
	background01 = new Background01();
	background02 = new Background02();

	flashWhite		= new FlashWhite();
	flashBlack		= new FlashBlack();
	fadeWhite		= new FadeWhite();
	fadeBlack		= new FadeBlack();
	flashWhiteFast	= new FlashWhiteFast();
	flashBlackFast	= new FlashBlackFast();
	fadeWhiteFast	= new FadeWhiteFast();
	fadeBlackFast	= new FadeBlackFast();
	clearBuffers    = new ClearBuffers();
	noise01         = new Noise01();

	blurMotion     = new BlurMotion();
	blurRadial     = new BlurRadial();
	
	srand(1);

	blurRadial->Init();
	blurMotion->Init();

#ifndef PLAY_WITH_SCRIPT

//	liquid->Init();
//	scape03->Init();
	ShowLoader(0.9f);
//	scape04->Init();
//	scape05->Init();
	ShowLoader(1.0f);
//	scape06->Init();
//	fastRoad->Init();
//	bricks->Init();
	linkin->Init();
	eye->Init();
//	boids->Init();
//	hospital->Init();
//	spikey->Init();
//	sphery01->Init();
//	sphery02->Init();
	sphery03->Init();
//	starfield->Init();
//	atom->Init();
	universe->Init();
	clouds->Init();
//	kokkels->Init();
//	tentacles->Init();
	background01->Init();
//	background02->Init();
	eye02->Init();

	poem->Init();


	noise01->Init();

#else

	scriptRegisterEffect(scape03,		EFFECT_VULCAN);
	scriptRegisterEffect(scape05,		EFFECT_LIQUID);
	scriptRegisterEffect(kokkels,		EFFECT_KOKKELS);

	scriptRegisterEffect(eye,			EFFECT_EYE01);
	scriptRegisterEffect(eye02,			EFFECT_EYE02);
	scriptRegisterEffect(sphery01,		EFFECT_SPHERY01);
	scriptRegisterEffect(sphery03,		EFFECT_SPHERY03);

	scriptRegisterEffect(clouds,		EFFECT_CLOUDS);
	scriptRegisterEffect(poem,			EFFECT_POEM);
 
	scriptRegisterEffect(linkin,		EFFECT_SWEET);

	scriptRegisterEffect(universe,		EFFECT_UNIVERSE);

	scriptRegisterEffect(background01,	EFFECT_BACKGROUND01);

	scriptRegisterEffect(flashWhite,	EFFECT_WHITEFL);
	scriptRegisterEffect(flashBlack,	EFFECT_BLACKFL);
	scriptRegisterEffect(fadeWhite,		EFFECT_WHITEFD);
	scriptRegisterEffect(fadeBlack,		EFFECT_BLACKFD);
	scriptRegisterEffect(flashWhiteFast,EFFECT_WHITEFLFAST);
	scriptRegisterEffect(flashBlackFast,EFFECT_BLACKFLFAST);
	scriptRegisterEffect(fadeWhiteFast,	EFFECT_WHITEFDFAST);
	scriptRegisterEffect(fadeBlackFast,	EFFECT_BLACKFDFAST);
	scriptRegisterEffect(noise01,		EFFECT_NOISEFL);
	scriptRegisterEffect(clearBuffers,  EFFECT_CLEARBUFFERS);

	scriptInitEffects();

//	scriptInitFile("script.usc");
	scriptInitMXM(script_usc);
#endif

#ifdef PLAY_WITH_FADEOUTBAR
	uniTimerReset();
	dword timer = uniTimer();
	while ((uniTimer() - timer) < 4000)
	{
		uniClearBuffers(UNI_BBF, 0);
		RenderLoader(1.0f);
		fadeBlack->Do((uniTimer() - timer) / 2);
		uniShowBuffers();
	}

	uniTextureUnSwapAll();
	uniTextureUnSwapAll();
	uniTextureUnSwapAll();

	_sleep(1000); 
#endif


#ifdef PLAY_WITH_MUSIC

#ifndef PLAY_WITH_SCRIPT 
	fmodPlay(0); 
#endif
#endif

	uniTimerReset(); 
}


dword noisetime;

void mainRender(dword timer) {
#ifndef PLAY_WITH_SCRIPT
//	uniClearBuffers(UNI_BBF | UNI_ZBF, sin((float)uniTimer() / 200.f) * 100.f + 128);
	uniClearBuffers(UNI_BBF | UNI_ZBF, clearColor);
//	uniClearBuffers(UNI_BBF | UNI_ZBF, 0x153560);
//	uniClearBuffers(UNI_BBF | UNI_ZBF, 0x346020);

//	uniTextureSetRenderTarget("target01");

//	uniClearBuffers(UNI_BBF | UNI_ZBF, 0);

	
//	liquid->Do(timer);
//	scape03->Do(timer);
//	scape04->Do(timer);
//	scape05->Do(timer);
//	scape06->Do(timer);
//	eye->Do(timer);
//	fastRoad->Do(timer); 
//	bricks->Do(timer);
//	linkin->Do(timer);
//	boids->Do(timer);
//	hospital->Do(timer);
//	spikey->Do(timer);
//	sphery01->Do(timer);
//	sphery02->Do(timer);
//	atom->Do(timer);
//	universe->Do(timer);
//	kokkels->Do(timer);
//	tentacles->Do(timer);
//	background01->Do(timer);
	sphery03->Do(timer);
	//background02->Do(timer);
//	sphery03->Do(timer);
//	eye02->Do(timer);

//	clouds->Do(timer);
//	linkin->Do(timer);

	//starfield->Do(timer);
/*

	uniSetTexture("design01");
	
	SpriteDrawer sp;
	sp.Start(UNI_ADD);
	float xb =   0;
	float yb =  510 - 32;
	float xe = 800;
	float ye = yb + 32;
	int fade = 255;
	int clr = (fade << 16) | (fade << 8) | fade;
	float scroll = timer / 50000.f;
	sp.Draw2D(xb, yb, 0+scroll, 0, clr,
		      xe, yb, 1+scroll, 0, clr,
  			  xb, ye, 0+scroll, 1, clr,
   			  xe, ye, 1+scroll, 1, clr);
	yb = 90;
	ye = yb + 32;
	sp.Draw2D(xb, yb, 0-scroll, 0, clr,
		      xe, yb, 1-scroll, 0, clr,
  			  xb, ye, 0-scroll, 1, clr,
   			  xe, ye, 1-scroll, 1, clr);
	sp.Stop();

	sphery03->Do(timer);
*/
//	sphery03->Do(timer);

	//	flashWhiteFast->Do((timer % (441*8)) * 2);
/*
	int ord, row, sync;
	fmodGetInfo(ord, row, sync);

	if ((row % 16) == 0)
	{
		noisetime = timer;
	}
*/
//	if (row < 32)
//		noise01->Do(timer - noisetime);
//	flashBlackFast->Do(timer - noisetime);

//	flashBlackFast->Do(timer - noisetime);

/*
	blurRadial->Open();
//	sphery01->Do(timer);
	eye->Do(timer);
	blurRadial->Close();
	blurRadial->Do(timer);
*/
/*
	blurMotion->Open();
//	starfield->Do(timer);
//	universe->Do(timer);
//	eye->Do(timer);

	blurMotion->Close();
	blurMotion->Do(timer);
*/
/*
	blurRadial->Open();
	scape03->Do(timer);
	blurRadial->Close();
	blurRadial->Do(timer);
*/
/*
	blurHorizontal->Open();
	eye->Do(timer);
	blurHorizontal->Close();
	blurHorizontal->Do(timer);
*/




/*
	int fade = (cos(timer / 500.f) * (0x55 / 2.f)) + (0x55 / 2.f);
	int color = (fade << 24) | (fade << 16) | (fade << 8) | fade;
	font->Do("this is a", 595, 480, color);
	font->Do("this is a", 595 + sin(timer / 1000.f) * 7.f, 480, color);
	font->Do("this is a", 595 - sin(timer / 1000.f) * 7.f, 480, color);

	timer += 500 * _PI;
	fade = (cos(timer / 500.f) * (0x55 / 2.f)) + (0x55 / 2.f);
	color = (fade << 24) | (fade << 16) | (fade << 8) | fade;
	font->Do("beautiful world", 528, 480, color);
	font->Do("beautiful world", 528 + sin(timer / 1000.f) * 7.f, 480, color);
	font->Do("beautiful world", 528 - sin(timer / 1000.f) * 7.f, 480, color);
*/
/*
//	font->Do("dream and live", 10, 130, 0xffffffff);
	font->Do("dream and live", 10, 130, 0x80808080);
	font->Do("dream and live", 10 + sin(timer / 1000.f) * 3.f, 130 + cos(timer / 1000.f) * 3.f, 0x80808080);
//	font->Do("wake up and die", 60, 180, 0xffffffff);
	font->Do("wake up and die", 60, 180, 0x55555555);
	font->Do("wake up and die", 60 + sin(timer / 1000.f) * 5.f, 180, 0x55555555);
	font->Do("wake up and die", 60 - sin(timer / 1000.f) * 5.f, 180, 0x55555555);
*/
/*

	int fade = (cos(timer / 500.f) * (0x55 / 2.f)) + (0x55 / 2.f);
	int color = (fade << 24) | (fade << 16) | (fade << 8) | fade;
	font->Do("dance funky boy!", 400, 150, color);
	font->Do("dance funky boy!", 400 + sin(timer / 1000.f) * 7.f, 150, color);
	font->Do("dance funky boy!", 400 - sin(timer / 1000.f) * 7.f, 150, color);
*/
#else
	scriptRunMXM();
#endif
}

void mainExit() {
#ifdef PLAY_WITH_MUSIC
	fmodStop();
#endif

#ifndef PLAY_WITH_SCRIPT
//	liquid->DeInit();
//	scape03->DeInit();
//	scape04->DeInit();
//	scape05->DeInit();
//	scape06->DeInit();
//	fastRoad->DeInit();
//	bricks->DeInit();
//	linkin->DeInit();
//	font->DeInit();
//	boids->DeInit();
//	eye->DeInit();
//	hospital->DeInit();
//	spikey->DeInit();
//	sphery01->DeInit();
//	sphery02->DeInit();
//	sphery03->DeInit();
//	starfield->DeInit();
//	atom->DeInit();
//	universe->DeInit();
//	kokkels->DeInit();
//	tentacles->DeInit();
	
//	background01->DeInit();
	//background02->DeInit();

//	blurMotion->DeInit();
//	blurRadial->DeInit();
//	blurHorizontal->DeInit();

	noise01->DeInit();

//	delete liquid;
	delete scape03;
//	delete scape04;
	delete scape05;
//	delete scape06;
	delete linkin;
	delete font;
//	delete boids;
	delete eye;
	delete sphery01;
	delete sphery03;
	delete universe;
	delete kokkels;
	delete clouds;
	delete background01;
	delete background02;

	delete blurMotion;
	delete blurRadial;
	
#endif
}


