/*

	Pyramid DX8 System - Script Code (header file)


	Remarks:
		-


	Updates:
		- 27-06-01 (rob):
			File created.


	(c) 2001, Robert Jan Bruinier

*/


#ifndef __SCRIPT_H__
#define __SCRIPT_H__

class Effect 
{
public:
	Effect();
	virtual			~Effect();
	virtual	bool	Init();
	virtual	void	Do(dword Timer);
	virtual	void	DeInit();
	virtual void	Receive(dword Message, dword timer);

	dword			scriptTimer;

	bool			resetOnPlay;
};

#ifdef UNI_SCRIPT_SYS

void scriptInitMXM(byte script[]);
void scriptRunMXM();

#ifdef UNI_SCRIPT_LOAD_FROM_FILE
void scriptInitFile(char *filename);
#endif

void scriptInitEffects();
void scriptRegisterEffect(Effect * effect, int handle);

#endif

#endif
