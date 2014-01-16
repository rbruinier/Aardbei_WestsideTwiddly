/*

	Pyramid DX8 System - HTML Log Code (source file)


	(c) 2001, Robert Jan Bruinier

*/


#include "Unicorn.h"

#ifdef UNI_LOGHTML

#include "Stdio.h"
#include "Time.h"

void uniStartLog() {
	if (uniSystem->log) {
		FILE *logfile;
		logfile = fopen(UNI_LOGNAME,"w+");
		fprintf(logfile, "<html>\n");
		fprintf(logfile, "<head>\n");
		fprintf(logfile, "<t1></t1>\n");
		fprintf(logfile, "</head>\n");
		fprintf(logfile, "<body>\n");
		fprintf(logfile, "<h1 align=center>Unicorn log file</h1>\n");
		time_t ltime = time( &ltime );
		fprintf(logfile, "<hr><br><b>Session started on %s<br><br><br></b>\n", ctime( &ltime ) );
		fprintf(logfile, "<pre>\n");
		fclose(logfile);
	}
}

void uniStopLog() {
	if (uniSystem->log) {
		FILE *logfile;
		logfile = fopen(UNI_LOGNAME,"a+");
		time_t ltime = time( &ltime );
		fprintf(logfile, "</pre>\n");
		fprintf(logfile, "<br><br><b>Session closed on %s</b><br><br><hr><br><br>\n", ctime( &ltime ) );
		fprintf(logfile, "</body>\n");
		fprintf(logfile, "</html>\n");
		fclose(logfile);

	}
}

void uniLog(char *tekst, ...) {
	if (uniSystem->log) {
		FILE *logfile;
		char str[800];
		va_list argptr;
		logfile = fopen(UNI_LOGNAME, "a+");
		va_start(argptr, tekst) ;
		vsprintf(str, tekst, argptr );
		va_end(argptr);
		fprintf(logfile, str);
		fclose(logfile);
	}
}

#endif