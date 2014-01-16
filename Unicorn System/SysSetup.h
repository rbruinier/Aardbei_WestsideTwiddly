/*

	Pyramid DX8 System - System Setup File


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/ 

#ifndef __SYSSETUP_H__
#define __SYSSETUP_H__

/* - DEBUG --------------------------------------------------------------------------------- */

/* Als je mem leaks wil vinden, compile met dit en debug met F5 */
//#define UNI_FULL_DEBUG


/* - LOGHTML ------------------------------------------------------------------------------- */

/* Comment dit als je geen html log functie in systeem wilt hebben  */
//#define UNI_LOGHTML


/* - SYSTEM -------------------------------------------------------------------------------- */

/* UNI_OWN_WINMAIN_AND_WINDOW:
		Als je een eigen winmain/window/windowproc wilt gebruiken moet je dit definen.
		Hou er rekening mee dat voor dat uniInit aanroept unicorn->windowHandle met de 
		juiste window gevuld is. Je moet dus ook zelf een systeem aanmaken met de naam
		unicorn -> System * unicorn = new System();
*/
//#define UNI_OWN_WINMAIN_AND_WINDOW

/* UNI_RESIZABLE_WINDOW
		Als je een resizable window wilt hebben. Voor de sniekjes onder ons. Kan me echt
		geen reden bedenken waarom je dit erin wilt hebben, maar ok als jij er blij van
		wordt...
*/
//#define UNI_RESIZABLE_WINDOW


/* - TEXTURES ------------------------------------------------------------------------------ */

/* Als je naar textures wilt renderen en daarna zelf nog bij de data wilt komen moet
   je dit definen. Als je dit niet defined kan je nog altijd renderen naar textures, je
   kan er alleen zelf niet aankomen. (wat je toch niet wilt, daar dat fucking traag is) */
//#define UNI_LOCKABLE_RENDERTARGET_TEXTURES


/* - BITMAP ------------------------------------------------------------------------------- */

/* Als je bitmaps (schermresolutie grootte) wilt renderen gebruik dit */
//#define UNI_BITMAP_SUPPORT


/* - SCRIPT -------------------------------------------------------------------------------- */

/* Hiermee kan je het volledige script systeem eruit slopen (behalve effect class). */
#define UNI_SCRIPT_SYS

/* Als je een script systeem voor mp3's erbij wilt hebben, define dit. */
//#define UNI_SCRIPT_MP3_SUPPORT

/* Als je een script systeem voor module's (xm's) erbij wilt hebben, define dit. */
#define UNI_SCRIPT_MODULE_SUPPORT

/* Als je scripts van schijf wilt lezen */
#define UNI_SCRIPT_LOAD_FROM_FILE


/* - FILES --------------------------------------------------------------------------------- */

/* Hiermee kan je het volledige file systeem eruit slopen */
//#define UNI_FILE_SYS

/* Hiermee kan je het pak-file systeem eruit slopen */
//#define UNI_PAK_SYS

/* Hiermee kan je de JPEG lader van 1kb eruit slopen */
//#define UNI_LOAD_JPEG

/* Hiermee kan je de RAW lader eruit slopen */
//#define UNI_LOAD_RAW

/* - MODULE -------------------------------------------------------------------------------- */

/* UNI_TINY_FMOD:
		Hiermee kan je het tiny fmod systeem aanzetten (voor intro's)
*/
#define UNI_TINY_FMOD

#endif