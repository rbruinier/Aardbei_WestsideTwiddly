/*

	Pyramid DX8 System - Script System (source file)


	(c) 2001, Robert Jan Bruinier

*/


/*
	SCRIPT_MUSICSTART:
		pat1

	SCRIPT_RESETALL:
		pat1, row1

	SCRIPT_SEND:
		handle, pat1, row1, 4 bytes of data

	SCRIPT_NONE:

	SCRIPT_RESET:
		handle, pat1, row1

	SCRIPT_RUN:
		handle, pat1, row1, pat2, row2, layer

	SCRIPT_QUIT:
		pat1, row1
*/

#include "Unicorn.h"

#ifdef UNI_SCRIPT_SYS

class ScriptLayer {
public:
	int		handle;
	int     oldHandle;
};

class ScriptMessage {
public:
	byte pat1, pat2;
	byte row1, row2;
	byte layer;
	byte handle;
	byte task;
	byte extra;
}; 


#define SCRIPT_MUSICSTART	249
#define SCRIPT_RESETALL		250
#define SCRIPT_SEND			251
#define SCRIPT_NONE			252
#define SCRIPT_RESET		253
#define SCRIPT_RUN			254
#define SCRIPT_QUIT			255

ScriptLayer		 scrLayer[256];
ScriptMessage	 *scrMessage;
int				 scrNrLayers   = 0;
int				 scrNrMessages = 0;

Effect			 *scrEffect[256];
int				 scrNrEffects = 0;

bool			 musicIsPlaying = false;

void scriptRegisterEffect(Effect * effect, int handle) 
{
	if (scrNrEffects == 0)
	{
		for (int i = 0; i < 256; i++)
		{
			scrEffect[i] = NULL;
		}
	}
#ifdef UNI_LOGHTML
	if (!scrNrEffects)
		uniLog("\n");
	uniLog("   Registering effect %i\n", handle);
#endif
	scrEffect[handle] = effect;
	scrNrEffects++;
}

#ifdef UNI_SCRIPT_LOAD_FROM_FILE

void scriptInitFile(char *filename) 
{
	DWORD tmp;
	HANDLE FH;

	FH = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (FH != INVALID_HANDLE_VALUE) {
		int size = GetFileSize(FH, &tmp);
		byte *data = new byte[size];
		ReadFile(FH, data, size, &tmp, NULL );
		CloseHandle( FH );
		scriptInitMXM(data);
	}
}

#endif

void scriptInitMXM(byte script[]) {
#ifdef UNI_LOGHTML
	uniLog("\n   Initialising script (MODULE)\n");
#endif

	scrNrMessages = *(dword*)script;
	scrMessage = (ScriptMessage*) ((byte*)script + 4);

	for (int i = 0; i < 256; i++) 
	{
		if (scrEffect[i])
		{
			scrEffect[i]->scriptTimer = 0;
		}
	}

	for (i = 0; i < 256; i++) 
	{
		scrLayer[i].handle    = 0;
		scrLayer[i].oldHandle = 0;
	}
}

void scriptInitEffects() {
	for (int i = 0; i < 256; i++) 
	{
		if (scrEffect[i])
		{
			if (!scrEffect[i]->Init())
				uniQuit("Failed to initialize effects");
		}
	}
} 

void scriptRunMXM() 
{
	int pat1, pat2, row1, row2, handle, layer, run, extra;
	int pos, row, sync;

	dword timer = uniTimer();

	pos  = -1;
	row  = -1;
	sync = -1;

	if (musicIsPlaying)
	{
		fmodGetInfo(pos, row, sync);
	}

	for (int i = 0; i < 256; i++) 
	{
		scrLayer[i].oldHandle = scrLayer[i].handle;
		scrLayer[i].handle = 0;
	}

	for (i = 0; i < scrNrMessages; i++) 
	{
		int msg = scrMessage[i].task;

		switch(msg) 
		{

		case SCRIPT_NONE:
		break;
		
		case SCRIPT_QUIT:
			pat1 = scrMessage[i].pat1;
			row1 = scrMessage[i].row1;
			if (((pos >= pat1) && (row >= row1)) || (pos > pat1))
			{
				for (int j = 0; j < 256; j++)
				{
					if (scrEffect[j])
					{
						scrEffect[j]->DeInit();
						delete scrEffect[j];
					}
				}
				uniQuit();
			}
		break;

		case SCRIPT_RESETALL:
			pat1 = scrMessage[i].pat1;
			row1 = scrMessage[i].row1;
			if (((pos >= pat1) && (row >= row1)) || (pos > pat1))
			{
				for (int j = 0; j < 256; j++)
				{
					if (scrEffect[j])
					{
						scrEffect[j]->scriptTimer = timer;
					}
				}
				scrMessage[i].task = SCRIPT_NONE;
			}
		break;

		case SCRIPT_MUSICSTART:
			pat1 = scrMessage[i].pat1;
			fmodPlay(pat1);
			scrMessage[i].task = SCRIPT_NONE;
			musicIsPlaying = true;
		break;

		case SCRIPT_RUN:
			handle = scrMessage[i].handle;
			pat1   = scrMessage[i].pat1;
			row1   = scrMessage[i].row1;
			pat2   = scrMessage[i].pat2;
			row2   = scrMessage[i].row2;
			layer  = scrMessage[i].layer;
			run = false;
			if ((pos >= pat1) && (pos <= pat2)) run = true;
			if ((pos == pat1) && (row  < row1)) run = false;
			if ((pos == pat2) && (row >= row2)) run = false;
			if (run)
				scrLayer[layer].handle = handle;				
		break;

		case SCRIPT_SEND:
			handle = scrMessage[i].handle;
			pat1   = scrMessage[i].pat1;
			row1   = scrMessage[i].row1;
			pat2   = scrMessage[i].pat2;
			row2   = scrMessage[i].row2;
			layer  = scrMessage[i].layer;
			extra  = scrMessage[i].extra;
			if (((pos >= pat1) && (row >= row1)) || (pos > pat1)) {
				scrEffect[handle]->Receive((pat2 << 24) + (row2 << 16) + (layer << 8) + extra, timer - scrEffect[handle]->scriptTimer);	
				scrMessage[i].task = SCRIPT_NONE;
			}
		break;

		case SCRIPT_RESET:
			pat1   = scrMessage[i].pat1;
			row1   = scrMessage[i].row1;
			handle = scrMessage[i].handle;
			if (((pos >= pat1) && (row >= row1)) || (pos > pat1)) 
			{
				scrEffect[handle]->scriptTimer = timer;				
				scrMessage[i].task = SCRIPT_NONE;
			}
		break;

		}

	}

	for (i = 0; i < 256; i++) 
	{
		handle = scrLayer[i].handle;
		if (handle != 0)
		{
			if (handle != scrLayer[i].oldHandle)
			{
				if (scrEffect[handle]->resetOnPlay)
				{
					scrEffect[handle]->scriptTimer = timer;
				}
			}

			scrEffect[handle]->Do(timer - scrEffect[handle]->scriptTimer);
		}
	}
}

#endif

Effect::Effect() 
{
	resetOnPlay = true;
}

Effect::~Effect() {
}

bool Effect::Init() 
{
	return true;
}

void Effect::Do( dword timer ) 
{
}

void Effect::DeInit() 
{
}

void Effect::Receive(dword param, dword timer) 
{
}
