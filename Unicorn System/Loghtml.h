/*

	Pyramid DX8 System - HTML Log Code (header file)


	Remarks:
		-


	Updates:
		- 22-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/


#ifndef __LOGHTML_H__
#define __LOGHTML_H__

#ifdef UNI_LOGHTML

void uniStartLog();
void uniStopLog();
void uniLog(char *text, ...);

#endif

#endif